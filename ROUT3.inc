;----------------------------------------------
;ROUT3
;----------------------------------------------

;----------------------------------------------
;move_my_base:
;
; Check controls to move myship or automove in
; demomode
;
; Input : -
; Output: -
;----------------------------------------------

move_my_base
	LDA myst		; Ship visible?
	BPL ok_to_move		; Yes, check if move
	RTS

ok_to_move
	JSR process_demo	;

	LDY #0
	STY temp3		; Reset key/joystick buffer X-dir
	STY temp3+1		; Reset key/joystick buffer Y-dir

	BIT demo_flag		; Check if in demomode
	BPL manual_control	; No, check controls

	INY			; Demo left
	BIT demo_direction
	BPL dem_right
	INY			; Demo right
dem_right
 	STY temp3		; Save demo move
	JMP possible_move

manual_control
	BIT key_joy_flag	; Check if keyboard/joystick controls
	BMI use_keyboard

	lda #0
	sta temp3
	jsr joyget
	and #$f
	tax
	lda joy_set,x
	jmp possible_move

;	LDX #1			; Check joystick left
;	JSR check_joy
;	ROL temp3
;
;	LDX #1			; Check joystick right
;	JSR check_joy2
;	ROL temp3
;
;	LDX #2			; Check joystick up
;	JSR check_joy
;	ROL temp3+1
;
; 	LDX #2			; Check joystick down
;	JSR check_joy2
;	ROL temp3+1
;
;	JMP possible_move

joy_set:
	.byte 0,4,8,12,1,5,9,13,2,6,10,14,3,7,11,15

use_keyboard
 	LDX #$3a		; Check left = 'Z' key
	JSR check_key
 	ROL temp3

 	LDX #$38		; Check right = 'X' key
	JSR check_key
	ROL temp3

	LDX #$1b		; Check up = ';' key
	JSR check_key
	ROL temp3+1

	LDX #$1e		; Check down = '.' key
	JSR check_key
	ROL temp3+1

possible_move
	JSR pokmypos		; Prepare myship for plotting sprite

	LDY temp3		; Read key buffer X-dir
	LDA key_press_relx,Y	; Get relative X-offset
	CLC			; Add to current X-pos
	ADC myx

	CMP #29			; Check right border screen
	BCS bad_x_position	; Jump if border reached
	STA myx			; Save new X-pos

bad_x_position
	LDY temp3+1		; Read key buffer Y-dir
	LDA key_press_rely,Y	; Get relative Y-offset
	CLC			; Add to current Y-pos
	ADC myy

	CMP #124-16		; Check myship top border
	BCC bad_y_pos		; Jump if border reached
	CMP #184-16		; Check myship bottom border
	BCS bad_y_pos		; Jump if border reached
good_y_pos
	STA myy			; Save new Y-pos

bad_y_pos
	LDY myy			; Get myship Y-pos
	LDX myx			; Get myship X-pos
	JSR xycalc		; Calculate screenaddress
	STA screen2+1
	STX screen2

	EOR screen+1		; Check hb screen if ship has moved
	BNE ship_has_moved	; Yes, jump
	txa			; Check lb screen if ship has moved
	EOR screen
	BEQ ship_hasnt_moved	; No, jump

ship_has_moved
	LDA #myheight		; Set spriteheight
	STA temp1+1

 	LDA graph+36		; Set spritegraphics address
	TAX
	LDY graph+37
	STY temp1
	JMP sprite		; Move sprite

ship_hasnt_moved
	LDA demo_direction	; Update auto demo move
	EOR #$80
 	STA demo_direction
	JMP delay

;----------------------------------------------
;Data for relative X- and Y- offset myship
;----------------------------------------------

key_press_rely
	.db 0,3,-3,0

key_press_relx
	.db 0,1,$FF,0

;----------------------------------------------
;print_scores:
;
; Print score digit X on screen from score_base
; 0<=X<= 6 Myscore
; 7<=X<=13 Highscore
; Digits can be left- or right aligned because a
; digit is 6 pixels width
;
; Input : X      = digit to be printed
;         screen = screen address digit
; Output: C=1 position=even, C=0 position=odd
;----------------------------------------------

print_scores
	STX temp1		; Save digitnr
	lda score_pos,x
	sta screen
	lda dig_filter1,x
	sta filter1
	lda dig_filter2,x
	sta filter2

	LDY score_base,X	; Get digit
	LDX mult24tab,Y		; Get lb graphdata address digit
	STX paddr+1
	stx paddr1+1

	ldx temp1
	clc			; Digit is right aligned
	lda paddr+1		; Add 80 to graphdata address
	adc addr_pos,x
	sta paddr+1
	clc
	lda paddr1+1
	adc addr_pos,x
	sta paddr1+1

;even_digit
	LDX #7			; Copy digit to screen
pscore1
	ldy scrtab,x
	lda (screen),Y
	and filter1
paddr
	ora digits,X		; Digit graphics left byte
	sta (screen),Y
	iny
	lda (screen),Y
	and filter2
paddr1
	ora digits+$100,X	; Digit graphics right byte
	sta (screen),y
	DEX
	BPL pscore1

	LDA temp1		; Return carry if odd pos
	ror a
	ldx temp1
	RTS

score_pos	.db 5,6,8,9,11,12,14
		.db 21,22,24,25,27,28,30
dig_filter1	.db $00,$f0,$00,$f0,$00,$f0,$00
		.db $00,$f0,$00,$f0,$00,$f0,$00
dig_filter2	.db $0f,$00,$0f,$00,$0f,$00,$0f
		.db $0f,$00,$0f,$00,$0f,$00,$0f
addr_pos	.db 0,80,0,80,0,80,0
		.db 0,80,0,80,0,80,0
filter1	.db 0
filter2	.db 0

;----------------------------------------------
;Graphic datapointer digits
;----------------------------------------------

mult24tab
	.db 0*8,1*8,2*8
 	.db 3*8,4*8,5*8
	.db 6*8,7*8,8*8
	.db 9*8

;--------------------------------------------------
;init_score:
;
; Reset myscore=0 and print myscore + high score
;
; Input : -
; Output: -
;--------------------------------------------------

highscore_pos	equ $8014

init_score
	JSR reset_score_to_0	; Reset myscore to 0 and print 0
				; Set hb screenpos highscore 

poke_hi_scr
	LDA #<highscore_pos	; Set lb screenpos hiscore
	STA screen

	LDX #7			; Plot digit on screen
	STX temp2		; Save odd/even flag
inscr1
	LDA score_base,X	; Get digit hiscore
	BNE inscr2		; 

	LDY temp2		; If digit=0 skip plotting
	BNE inscr3
inscr2
	LDA #0
	STA temp2
	JSR print_scores	; Print digit
inscr3
	bcs not_odd		; If screenpos=odd, add 2 to screenpos
	inc screen
not_odd
	inc screen		; Add 1 to screenpos
inscr4
	INX			; Next digit
	CPX #14			; All digits printed?
	BNE inscr1

	RTS

;--------------------------------------------------
;add_to_score:
;
; Add value, depending off alien graphic nr, to myscore
;
; Input : -
; Output: -
;--------------------------------------------------
add_to_score
 	LDA #<myscore_pos	; Set myscore screenaddress
	STA screen
	LDA #>myscore_pos
	STA screen+1

 	LDA algra,X		; Get alien graphic
	SEC
	SBC #12
 	LSR A
	TAX
 	LDA alien_score,X	; Get score for hitting alien

	LDX #5			; Set myscore digit 5
addscr2
	STA temp1		; Save points to add
;	dec screen
;	dec screen

	LDA temp1		; Get points to add
	CLC
	ADC myscore,X		; Add to myscore
	CMP #10
	BCC addscr3
	SBC #10
	STA myscore,X
	JSR print_scores	; Print digit on screen

	LDA #1
 	DEX
	BPL addscr2

 	JMP reset_score_to_0
addscr3
	STA myscore,X
	CPX #2
	BNE not_ten_thousands
	BIT extra_life_flag
	BMI not_ten_thousands
	CMP #2
	BNE not_ten_thousands	; extra life at >=20000
	SEC
	ROR extra_life_flag
	TXA
	PHA
	JSR liveson
	INC lives
	JSR liveson
	LDX #<xlife
	LDY #>xlife
	JSR mksound
	PLA
	TAX
not_ten_thousands
	JMP print_scores

;--------------------------------------------------
; Points awarded for aliens
; (divided by ten)
;--------------------------------------------------

alien_score
	.db 2,2,2,2
	.db 4,8,4,4
	.db 6,8
	.db 8,8

;--------------------------------------------------
;--------------------------------------------------

xlife
	.dw $13
	.dw 3
 	.dw 129
	.dw 30

;--------------------------------------------------
;--------------------------------------------------

check_new_high
	BIT demo_flag
	BMI not_new_high
	LDX #$FF
new_high1
	INX
	CPX #7
	BEQ not_new_high
new_high2
	LDA myscore,X
	CMP hiscore,X
	BEQ new_high1
	BCC not_new_high
copy_to_high
	LDA myscore,X
	STA hiscore,X
	INX
	CPX #7
	BCC copy_to_high
not_new_high
	RTS

;--------------------------------------------------
;reset_score_to_0:
;
; Erase, reset and print myscore
;
; Input : -
; Output: screen = screenaddress last digit myscore
;         X      = 6, last digit myscore
;--------------------------------------------------

myscore_pos	equ $8005
myscore_last	equ myscore_pos+9

reset_score_to_0
	LDY #6			; Reset myscore to 0
	LDA #0
zero_my_score
	STA myscore,Y
	DEY
	BPL zero_my_score

clr_next_line
	lda #clrscr_chr		; Load background color
	LDX #10			; Set width myscore in bytes
clr_score_screen
	STA myscore_pos,X	; Clear byte on screen
	DEX			; Next byte
	BPL clr_score_screen

	clc			; Goto next line
	lda clr_score_screen+1
	adc #$20
	sta clr_score_screen+1
	cmp #<myscore_pos	; Check if last line reached
	bne clr_next_line

	LDA #<myscore_pos	; Set screenaddr to last digit myscore
	STA screen
	LDA #>myscore_pos
	STA screen+1
	LDX #6			; Set pointer to last digit myscore
	JMP print_scores	; Plot digit

;--------------------------------------------------
;toggle_demo_direct:
;
; Revert direction in demomode
;
; Input : -
; Output: -
;--------------------------------------------------

toggle_demo_direct
	LDA demo_direction
	EOR #$80
	STA demo_direction
	RTS

 