;----------------------------------------------
;ALIENS3
;----------------------------------------------

;----------------------------------------------
;move_the_aliens:
;
; Move 6 aliens before processing
;
; Input : -
; Output: -
;----------------------------------------------

move_the_aliens
	LDA #process
	STA temp4		; Set looppointer=6

	LDX procst		; Get actual alien processed
move_aliens
	LDA alst,X		; Get alien status
	BMI move2		; If visible -> move aliens
	JSR delay		; Else wait a while
move5
 	INC procst		; Point to next alien
	LDX procst		; Get actual alien processed
	CPX aliens		; Last alien?
	BNE move3		; No, jump

	JSR process_aliens	; Process aliens
	JSR pause		; Wait a while

	LDX #0			; Point to first alien
	STX procst

	INC expldelay		; Inc explosion delay counter
move3
	DEC temp4		; Dec looppointer
	BNE move_aliens		; If not looped 6 times, jump

	RTS

;----------------------------------------------
;move2:
;
; If bit0 of alien status =1 then alien is exploding
; If multiplier<>0 then move alien
; If multiplier=0 init pattern and execute actions
;
; Input : A=Alien status
;         X=Actual processed alien
; Output: -
;----------------------------------------------

move2
	LSR A			; Check bit0 alien status (exploding)
	BCC move7		; Not exploding, jump
	JSR explosion		; If set, next explosion sprite
	JMP move5		; Return
move7
	LDA almult,X		; Check alien multiplier
	BNE move4		; Plot next sprite

	LDA alpatlow,X
	STA temp2		; Set lb alien pattern data
	LDA alpathigh,X
	STA temp2+1		; Set hb alien pattern data

	LDY alpatoff,X		; Set pattern data offset pointer
	INC alpatoff,X		; Inc offset pointer

	LDA (temp2),Y		; Get Action to be executed
				; If bit7=1 then set multiplier
				; Next data is action
	BMI branch_wont_reach	; If bit7=1 jump

	JMP move6		; Execute action

branch_wont_reach
	AND #$7F		; Clear bit7
	STA almult,X		; Set alien multiplier

	INY			; Point to next data
	INC alpatoff,X		; Inc offset pointer

	LDA (temp2),Y		; Get next pattern data byte
	LDY alpatreflect,X	; Check if movement has to be reversed
	BPL not_reverse		; If bit7=0, no reverse

	CMP #48	
	BCS flip_acute_angle	; If action >=48 jump

	TAY			; Save action
	AND #$F8		; Set bit0-2=0 (>7)
	STA temp1		; Save action

	TYA			; Get action
	AND #7			; Set bit3-7=0

	TAY			; Set pointer (bit0-2 action)
	LDA flip_table,Y	; Get direction
	ORA temp1		; Restore bits 3-7

flip_acute_angle
	EOR #7			; Invert bits0-2
not_reverse
	STA aldirect,X		; Set alien direction
move4
	LDA alx,X		; Get alien X-pos
	LDY aly,X		; Get alien Y-pos
	TAX
	JSR xycalc		; Calculate screenaddress
	STA screen+1
	STX screen

	LDX procst		; Get alien processed pointer
	DEC almult,X		; Dec alien multiplier

	LDY aldirect,X		; Get alien direction
	CPY #$40
	BCC moving_alien	; If <$40 jump

	JSR delay		; If >=$40 wait
	JMP move5		; Next alien

;----------------------------------------------
;move_alien:
;
; Add relative X/Y to actual alien X/Y and move alien
;
; Input : X=alien pointer
; Output: -
;----------------------------------------------

moving_alien
	CLC
	LDA alx,X		; Get alien X-pos
	ADC addrelx,Y		; Add relative-X
check_x_wrap
	CMP #32			; Check limit right
	BCC x_not_wrapped	; If not exceeded, jump
	SBC #32			; Wrap left
	JMP check_x_wrap	; Check again

x_not_wrapped
	STA alx,X		; Set X-pos alien

	CLC
	LDA aly,X		; Get alien Y-pos
	ADC addrely,Y		; Add relative-Y
	STA aly,X		; Set Y-pos alien

	TAY			; Save alien Y-pos
	LDA alx,X		; Get alien X-pos
dalg
	TAX			; Set alien X-pos
	JSR xycalc		; Calculate screenaddress
 	STA screen2+1
	STX screen2

	LDA #alheight
	STA temp1+1		; Set spriteheight

	LDX procst		; Get alien processed pointer
	LDY algra,X		; Set alien graphics pointer
	LDX graph,Y
	LDA graph+1,Y
	STA temp1
	TAY			; Set hb alien graphics
	TXA			; Set lb alien graphics
	JSR sprite		; Plot sprite
	JMP move5		; Next alien

;----------------------------------------------
;move6:
;
; Execute action
;
; Input : A=action
;         Y=pattern data offset pointer
;----------------------------------------------

move6
	INY			; Point to next data
	STY temp3+1		; Save pointer

	TAY			; Set action pointer
	LDA actiontab,Y
	STA temp1		; Set lb action routine
	LDA actiontab+1,Y
	STA temp1+1		; Set hb action routine

	LDY temp3+1		; Restore pointer
	JMP (temp1)		; Execute action

;----------------------------------------------
;explosion:
;
; Check if explosion delay is reached, if so
; plot explosion
;
; Input : -
; Output:
;----------------------------------------------

explosion
	LDA expldelay		; Check delay of 2 frames
	AND #1
	BNE expl4		; If not, exit

	LDA alx,X		; Get alien X-pos
	LDY aly,X		; Get alien Y-pos
	TAX
	JSR xycalc		; Calculate screenaddress
	STA screen+1
	STX screen
	STA screen2+1		; No move
	STX screen2

	LDA #alheight
	STA temp1+1		; Set sprite height

	LDX procst		; Get processtatus
	LDY algra,X		; Set alien graphics pointer
	LDA graph+1,Y		; Get hb alien graphics
	STA temp1

	LDA graph,Y		; Get lb alien graphics
	STA temp3

	CPY #12			; Check pointer
	BCC expl2

	LDA #0
	STA algra,X
	TAY
	BEQ expl3		; Forced jump
expl2
	INY
	INY
	TYA
	STA algra,X		; Alien- = explosion graphics

	CPY #12
	BCC expl3

	LDA #$C0		; Mark alien move
	STA screen2+1

	LDA #0
	STA alst,X		; Mark alien status
	DEC almove
expl3
	LDX graph,Y		; Get lb alien graphics
	LDA graph+1,Y		; Get hb alien graphics
	TAY
	LDA temp3		; Get explosion nr
	JMP sprite		; Plot explosion
expl4
	RTS

;----------------------------------------------
; Flip table for reverse patterns
; Values EORed because of EOR#7
; which is used after to flip
; the acute angled 'climbs'.
;----------------------------------------------

flip_table
	.db 7	; 0 EOR 7
	.db 4	; 3 EOR 7
	.db 5	; 2 EOR 7
 	.db 6	; 1 EOR 7
	.db 0	; 7 EOR 7
	.db 1	; 6 EOR 7
	.db 2	; 5 EOR 7
	.db 3	; 4 EOR 7
