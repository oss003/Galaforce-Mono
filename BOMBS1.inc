;----------------------------------------------
;BOMBS1
;----------------------------------------------

;----------------------------------------------
;process_my_bombs:
;
; Check if bullets of myship have to be moved or erased
; Check if RET/fire is pressed to plot new bullet
; Entry move_bomb plots or erases a bullet
;
; Input : -
; Output: -
;----------------------------------------------

process_my_bombs
	LDA bombdel		; Check if bombs on screen
	BEQ procmybmb

	DEC bombdel		; Decrement bomb counter
procmybmb
	LDA mybullact		; Get mybullets on screen counter
	BEQ bomb10		; Jump if none

	LDX #mymaxbull-1	; Check all bullets
bomb6
	STX temp2		; Check if bullet on screen
	LDA mybullst,X
	BMI bomb7		; If bullet on screen, move
bomb9
	LDX temp2
	DEX
	BPL bomb6		; Loop if not last bullet
	BMI bomb5		; Jump if all bullets handled

bomb7
	LDY mybully,X		; Get Y-pos bullet
	LDA mybullx,X
	TAX			; Get X-pos bullet
	JSR move_bomb		; Erase bullet

	LDX temp2		; Get bullet pointer
	LDA mybully,X		; Load Y-pos bullet
	SEC			; Substrat Y-pos with 8 pixels
	SBC #8
	STA mybully,X		; Store new Y-pos
	cmp #8
	BCS bomb8		; Check if top border reached

	DEC mybullact		; Decrement mybullets on screen counter
	LDA #0			; Mark bullet of screen
	STA mybullst,X
	BEQ bomb9		; Forced jump
bomb8
	TAY			; Get Y-pos bullet
	LDA mybullx,X
	TAX			; Get X-pos bullet
	JSR move_bomb		; Plot bullet
	JMP bomb9

bomb5
	LDA mybullact		; Get mybullets on screen counter
bomb10
	CMP #mymaxbull		; Check if max bullet reached
	BEQ bomb1		; If so, exit

 	LDA myst		; Check status myship
	BMI bomb1		; If blown, exit

	LDA demo_flag		; Check if in demomode
	BMI bomb11		; If yes, skip key/joystick controls

	BIT key_joy_flag	; Check to read fire or space
	BMI kbd_fire

;------------------------------
; Joystick read fire button
;------------------------------
	jsr joyget
	and #$10		; Bit4=fire button
	beq not_button
	bne bomb11

;	LDA #$80		; Read joystick fire
;	LDX #0
;	JSR osbyte
;	TXA
;	LSR A
;	BCC not_button
;	BCS bomb11
;------------------------------

kbd_fire
	LDX #$0d		; Check RET
	JSR check_key
	BEQ bomb11
not_button
	LDA #0			; Reset flag fire/RET not pressed
	STA bombdel
	BEQ bomb1		; Forced jump
bomb11
	LDA bombdel		; Check if new bullet fired
	BNE bomb1		; If so, exit

	LDX #mymaxbull-1	; Get bullet pointer
bomb2
	LDA mybullst,X		; Read bullet status
	BPL bomb3		; Find erased bullet
	DEX
	BPL bomb2
bomb1
	RTS
bomb3
	LDA #$FF		; Storage for new bullet found
	STA mybullst,X		; Mark as visible

	LDA #7			; Set bullets on screen????
	STA bombdel             ; WAS 3!

	STX temp2		; Save bullet pointer

	INC mybullact		; Inc. actual bullets counter

	LDA myx			; Get X-pos myship

;!!!!!!!!!!!!!!!!!!!!!!!!!
	CLC			; Add 1 to get fireposition
	ADC #1                  
;!!!!!!!!!!!!!!!!!!!!!!!!!

	STA mybullx,X		; Save X-pos bullet
	STA temp1		; Save X-pos

	LDA myy			; Get Y-pos ship
	SEC			; Substract 8
	SBC #8
;	AND #191
	STA mybully,X		; Save Y-pos bullet

	TAY			; Get Y-pos bullet
	LDX temp1		; Get X-pos bullet
	JSR move_bomb		; Plot bullet

	lda #1
	ldx #9
	ldy #1
	JMP mksound		; FX sound fire

;----------------------------------------------
;move_bomb:
;
; Plot/erase a bullet at position X,Y
;
; Input : X=X-pos bullet
;         Y=Y-pos bullet
; Output: -
;----------------------------------------------

move_bomb
	JSR xycalc2		; Calculate screenaddress
	STA screen+1
	STX screen

	LDX #7			; Plot bullet on screen
bomb4
	ldy scrtab,x
	LDA bombgra,X
	EOR (screen),Y
	STA (screen),Y
	DEX
	BPL bomb4

	ldy #$c1		; Plot rocketwing right
	LDA #$40
	EOR (screen),Y
	STA (screen),Y
mks2
	RTS

;----------------------------------------------
;firesound:
;
;
;----------------------------------------------

firesnd
	.db $12,2,115,4

;----------------------------------------------
;mksound:
;
; Play fire sound when a bomb is plot
;
; Input : -
; Output: -
;----------------------------------------------

mksound

 	bit sound_flag
	bpl mks2
	bit demo_flag
	bmi mks2

	jmp sound

; 	lda $b002    ; org
;	eor #4
;	sta $b002
;	rts

;----------------------------------------------
;Graphics data bullets
;----------------------------------------------

bombgra
	.db $03,$03,$03,$03,$03,$03
	.db $05,$01

