;----------------------------------------------
;BOMBS2
;----------------------------------------------

;----------------------------------------------
;process_aliens_bombs:
;
; - Try to init a new alien bomb
; - Move alien bombs on screen
;----------------------------------------------

process_aliens_bombs

; Try to init a bomb (random)

	LDA albullact		; Check for max alien bullets
	CMP #almaxbull
	BEQ nobomb

	LDA aliensm1		; Check for 20 aliens
	CMP #20
	BCC prb2		; Jump if less then 20 aliens

	LDA algra+32		; If more, check if alien 32 = sprite 22
	CMP #22
	BNE prb2		; If not, jump

	LDA rand1+1		; Alien 32 = sprite 22, get rand+1
	AND #$F                 ; Filter between 0-15
	BNE prb2		; If not 0, jump

	LDA #32			; Forced jump
	BNE prb
prb2
	JSR rand		; Get random nr
bombrate
	AND #15			; Limit between 1-15
	BNE nobomb		; Jump if not 0

	LDA rand1+1		; Get new random nr
	AND #63			; Limit between 1-63
	CMP aliens
	BCS nobomb		; Jump if rand>=aliens
prb
	TAX			; Set alien pointer
	LDA alst,X		; Get status alien
	BPL nobomb		; If bit7 = 0, nobomb
	LSR A			; Check bit0
	BCS nobomb		; If set, nobomb
	JSR init_bomb		; Init bomb

; Process the alien bombs

nobomb
	LDA albullact		; Get nr of alien bombs
	BEQ none_move		; If no bomb, exit

	LDX #almaxbull-1
malbomb
	STX temp2
	LDA albullst,X		; Get alien bullet status
	BMI malbomb2		; If bit7 = 1, jump
malbomb3
	LDX temp2
	DEX			; Next alien
	BPL malbomb
none_move
	RTS

malbomb2
	LDA albullspeed,X	; Get alien bullet speed
	BMI always_move		; If bit7 = 1 move

	LDA #1			; Filter bit0
	BIT counter
	BNE malbomb3		; If set, next alien
always_move
	LDY albully,X		; Get Y-pos alien bullet
	LDA albullx,X		; Get X-pos alien bullet
	TAX
	STX temp2+1		; Save X
	JSR disp_bomb		; Display bomb

	LDX temp2
	LDA albully,X		; Get Y-pos alien bomb
	CLC			; Add 8 to Y-pos
	ADC #8
	STA albully,X		; Store Y-pos alien bomb
	cmp #192		; Check for bottom line of screen
	BCC malbomb4		; Jump if not
	LDA #0			; Else reset alien status
	STA albullst,X
	DEC albullact		; Dec actual alien bullets
	JMP malbomb3		; Next alien

malbomb4
	TAY			; Set Y-pos alien bomb

	LDA aliensm1		; Check if more then 20 aliens
	CMP #20
	BCS malbomb5		; More then 20 aliens, jump

	LDA curwave		; Get wavenr
	CMP #13
	BCC malbomb5		; Less then 13, jump

	CMP #29			
	BCS homi1		; More then 29, jump

	LDA albullspeed,X	; Get alien bullet speed
	BMI malbomb5		; If bit7 = 1, jump
homi1
	LDA rand1+1		; Get random nr
	BMI malbomb5		; If bit7 = 1, jump

	LDA myx			; Get myship X-pos
	CLC
	ADC #2			; Add 2
	CMP albullx,X
	BEQ malbomb5		; If bomb above ship, don't correct X-pos bomb
	BCS malbomb6		; If bomb left of ship, inc X-pos bomb
	DEC temp2+1		; If bomb right of ship, dec X-pos bomb
	DEC albullx,X
	JMP malbomb5
malbomb6
	INC temp2+1
	INC albullx,X
malbomb5
	LDX temp2+1		; Display bomb
	JSR disp_bomb
	JMP malbomb3		; Next alien

;----------------------------------------------
;init_bomb:
;
; Subroutine to init an alien bomb if possible!
;
; Input : -
; Output: -
;----------------------------------------------

init_bomb
	LDA curwave		; If wave<3, nobombs
	CMP #3
	BCC no_bomb_allowed

	LDY #almaxbull-1	; Loop all alien bombs
dropb1
	LDA albullst,Y		; Get status
	BPL dropb2		; If empty, use
	DEY			; Next bomb
	BPL dropb1

no_bomb_allowed
	RTS

dropb2
	LDA aly,X		; Get alien Y-pos
	CMP #16
	BCC no_bomb_allowed	; If in top row, no bomb

	CMP #192-16		; Was 240
	BCS no_bomb_allowed	; If at bottom row, no bomb

	clc
	adc #16			; Add 16 for bottom alien
;	AND #192-16		; Was $F8
	STA albully,Y		; Set Y-pos bomb
	STA temp2
	CMP #80			; Was 80
	BCC any_speed		; If less, new speed

	AND #$7F		; Reset bit7
	BCS slow_only		; Forced jump
any_speed
	JSR rand		; Get random nr
slow_only
	STA albullspeed,Y	; Store alien bullet speed

	LDA #$FF		; Mark alien status as visible
	STA albullst,Y

	INC albullact		; Inc alien bullets on screen

	LDA alx,X		; Get alien X-pos
	CLC
	ADC #1			; Point to middle of alien
	STA albullx,Y		; Set alien bullet X-pos
	TAX

	LDY temp2		; get alien bomb Y-pos

; Subroutine to display/erase
; a bomb.

disp_bomb
	JSR xycalc2		; Calculate screen address
	STA screen+1
	STX screen

	LDX #7			; Display bomb
disp_b2
	ldy scrtab,x
	LDA albomb,X
	EOR (screen),Y
	STA (screen),Y
	DEX
	BPL disp_b2

	ldy #$21		; Plot bulletwing right
	LDA #$40
	EOR (screen),Y
	STA (screen),Y

	RTS

;----------------------------------------------
;albomb graphic data
;----------------------------------------------

albomb
	.db $01,$05,$03,$03,$03,$03,$03,$03

