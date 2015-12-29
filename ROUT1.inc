;----------------------------------------------
;ROUT1
;----------------------------------------------

;----------------------------------------------
;check_joy:
;
; Check if button of joystick is pressed
;
; Input : X=1 -> left
;         X=2 -> up
; Output: C=0 direction detected
;         C=1 direction not detected
;----------------------------------------------

check_joy
;	LDA #$80
;	JSR osbyte
;	CPY #$C0		; Check if button 1 pressed
	RTS

;----------------------------------------------
;check_joy2:
;
; Check if button of joystick is pressed
;
; Input : X=1 -> right
;         X=2 -> down
; Output: C=0 direction detected
;         C=1 direction not detected
;----------------------------------------------

check_joy2
;	LDA #$80
;	JSR osbyte
;	CPY #$40		; Check if button 2 pressed
;	ROR A
;	EOR #$80
;	ROL A
	RTS

;----------------------------------------------
;pause4:
;
; Entry pause4    -> Check if key 'P' is pressed
; Entry check_key -> check if key in X pressed
;
; Input : -
; Output: A=0  key pressed
;         A=-1 key not pressed
;----------------------------------------------

pause4
	LDX #$30		; Key 'P'

check_key
	TXA
	JSR scan_key
	RTS

;----------------------------------------------
;delay:
;
; Entry delay  -> waits +/- 3100 cycles
; Entry delay2 -> waits X * 1024 cycles
;
; Input : -
; Output: Wait X * 1024 cycles
;----------------------------------------------

delay
	LDX #3
delay2
	LDY #0
delay3
	DEY
	BNE delay3
	DEX
	BNE delay2
nopause
	RTS

;----------------------------------------------
;pause:
;
; Check if key 'S' is pressed and update status
; Check if key 'P' pressed and update status
; If 'P'is pressed, wait until 'P' is pressed again
;
; Input : -
; Output: Update 'S' and 'P' status
;----------------------------------------------

pause
	JSR sound_on_off	; Check 'S' and update status
	JSR pause4		; Check 'P'
	BNE nopause
	CLC
	ROR pause_flag
pause1
	JSR pause4		; Wait until 'P' is released
	BEQ pause1

	LDY #0			; Print 'P' status
	JSR prnstr
pause2
	JSR pause4		; Wait until 'P' is pressed
	BNE pause2
pause3
	JSR pause4		; Wait until 'P' is released
	BEQ pause3

	SEC
	ROR pause_flag

	LDY #0			; Erase 'P' status
	JMP prnstr

;----------------------------------------------
;rstall:
;
; Reset all game vars
; Entry seed_rnd -> Init rand1 with random values
;
; Input : -
; Output: Reset game vars + init random var rand1
;----------------------------------------------

rstall
 	LDA #0			; Reset gamevars
	STA myst
	STA almove
	STA procst
	STA mybullact
	STA albullact
	STA bombdel
	STA initact

	LDY #maxaliens-1	; Reset alien data
rst1
	STA alst,Y
	DEY
	BPL rst1

	LDX #maxpatt-1		; Reset alien status
rst2
 	STA initst,X
	DEX
	BPL rst2

	LDY #mymaxbull-1	; Reset mybullets
rst3
	STA mybullst,Y
	DEY
	BPL rst3

	LDY #almaxbull-1	; Reset alienbullets
rst4
	STA albullst,Y
	DEY
	BPL rst4

	LDA #14			; Set myship position
	STA myx
	LDA #176-16
	STA myy

seed_rnd
	LDA $b804		; Feed random var rand1
	ORA #1
	STA rand1
	LDA #240
	ORA #1
	STA rand1+1
	LDA #$FC
	ORA #1
	STA rand1+2
	RTS

;----------------------------------------------
;xycalc:
;
; Calculate screen address from X and Y
; 0<=X<=Screenwidth in bytes
; 8<=Y 
;
; Input : X,Y
; Output: X=lb and A=hb screenaddress
;----------------------------------------------

xycalc
	TXA
xycalc4
	CMP #29			; Check right border X
	BCC xycalc3
	SBC #29			; If exceed, display left
	JMP xycalc4
xycalc3
	TAX			; Save X

	CPY #8			; Check top border
	BCS xycalc2
	LDA #$C0		; Return flag upper border exceeded
	RTS
xycalc2
	sty temp1		; Convert X,Y to screenaddress
	ldy #0
	sty temp1+1
	asl temp1
	rol temp1+1
	asl temp1
	rol temp1+1
	asl temp1
	rol temp1+1
	asl temp1
	rol temp1+1
	asl temp1
	rol temp1+1
	clc
	txa
	adc temp1
	sta temp1
	lda temp1+1
	adc #$80
	sta temp1+1
	rts

;----------------------------------------------
;pokmypos:
;
; Prepare myship for plotting sprite
;
; Input : -
; Output: screen  -> screenaddress
;         temp1+1 -> spriteheight
;----------------------------------------------

pokmypos
	LDX myx			; Calculate screenaddress
	LDY myy
	JSR xycalc
	STX screen
	STA screen+1

	LDY #myheight		; Set spriteheight
	STY temp1+1
sound_exit
	RTS

;----------------------------------------------
;sound_on_off:
;
; Test if key S pressed after 15 loops
; If so, check keyboard/joystick and change status
; Entry display_sound_status -> print 'S' status
;
; Input : -
; Output: If 'S'or 'K' pressed, update status
;----------------------------------------------

sound_on_off
	INC counter_sound	; Wait for 15 loops 'S' pressed
	LDA counter_sound
	AND #$F
	BNE sound_exit

	JSR key_joy		; Check keyboard/joystick status

	LDX #$33		; Check if key 'S' pressed
	JSR check_key
	BNE sound_exit

	JSR display_sound_status ; Erase 'S' status
	LDA sound_letter	; Switch S to Q or vice versa
	EOR #6			; S EOR 6 = Q, Q EOR 6 =S
	STA sound_letter
	LDA sound_flag
	EOR #$80
 	STA sound_flag

display_sound_status
	LDY #16			; display 'S' or 'Q'
	JMP prnstr

;----------------------------------------------
;key_joy:
;
; Test if key K pressed, if so change status
; Entry display_key_joy_status -> print 'K' status
;
; Input : -
; Output: If 'K' pressed, update status
;----------------------------------------------

key_joy
	LDX #$2b		; K key
	JSR check_key
	BNE sound_exit

	JSR display_key_joy_status ; Erase 'K' status
	LDA key_joy_letter	; Switch K to J or vice versa
	EOR #7			;K EOR 7 = J, J EOR 7 = K
	STA key_joy_letter
	LDA key_joy_flag
	EOR #$80
key_joy2
	STA key_joy_flag

display_key_joy_status
	LDY #18			; display 'K' or 'J'
	JMP prnstr

;----------------------------------------------
;Storage relative X and Y movements aliens
;----------------------------------------------

addrelx
	.db 0,xstep,0,-xstep
	.db xstep,xstep,-xstep,-xstep
	.db 0,2*xstep,0,-2*xstep
	.db 2*xstep,2*xstep,-2*xstep,-2*xstep
	.db 0,3*xstep,0,-3*xstep
	.db 3*xstep,3*xstep,-3*xstep,-3*xstep
	.db 0,4*xstep,0,-4*xstep
	.db 4*xstep,4*xstep,-4*xstep,-4*xstep
	.db 0,5*xstep,0,-5*xstep
	.db 5*xstep,5*xstep,-5*xstep,-5*xstep
	.db 0,6*xstep,0,-6*xstep
	.db 6*xstep,6*xstep,-6*xstep,-6*xstep
	.db 2,2,-2,-2
	.db 2,2,-2,-2
addrely
	.db -ystep,0,ystep,0
	.db -ystep,ystep,ystep,-ystep
	.db -2*ystep,0,2*ystep,0
 	.db -2*ystep,2*ystep,2*ystep,-2*ystep
	.db -3*ystep,0,3*ystep,0
 	.db -3*ystep,3*ystep,3*ystep,-3*ystep
	.db -4*ystep,0,4*ystep,0
	.db -4*ystep,4*ystep,4*ystep,-4*ystep
	.db -5*ystep,0,5*ystep,0
	.db -5*ystep,5*ystep,5*ystep,-5*ystep
	.db -6*ystep,0,6*ystep,0
 	.db -6*ystep,6*ystep,6*ystep,-6*ystep
	.db -16,4,16,-4
	.db -4,16,4,-16
