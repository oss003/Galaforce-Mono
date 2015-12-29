;----------------------------------------------
;STARS
;----------------------------------------------

;----------------------------------------------
;starinit:
;
; Plot random 22 stars and store data
; Start at y = 8 to 184 every 8 pixels
; A star has 3 bytes storage:
;
; Byte 0 = lb screenaddress
; Byte 1 = hb screenaddress
; Byte 2 = colour
;
; Input : -
; Output: Plot 23 stars on screen (skip topline)
;----------------------------------------------

starinit
	LDX #23*3-3		; Star datapointer
	LDY #23			; Star counter

	LDA #0
	STA addres
	STA addres+1
	STA whichstar
starin0
	JSR starpos		; Get random screenaddress

	JSR rand		; Random position in line
	AND #$a0
	ORA addres
	CLC
	ADC stardat,X
	STA stardat,X

	LDA addres+1
	ADC stardat+1,X
	STA stardat+1,X

	inc addres+1		; Next line
gcol
	JSR rand		; Get colour (1-3)
	AND #3
	BEQ gcol

	STA stardat+2,X
	EOR (stardat,X)
	STA (stardat,X)

	DEX			; Increment data pointer
	DEX
	DEX

	DEY			; Decrement Starcounter
	BNE starin0

	RTS

;----------------------------------------------
;movestars:
;
; Move all stars 1 pixel down
; If staraddress crosses #100 line, switch on/off
; for blinking effect
;
; Input : -
; Output: Move 23 stars 1 pixel down
;         Shift star every 8 pixels for
;         blinking effect
;----------------------------------------------

movestars
;	rts

	LDY #23
	LDX #0
movestar
	LDA stardat+2,X		; Erase star
	EOR (stardat,X)
	STA (stardat,X)

	lda stardat,x		; Move star 1 pixel down
	clc
	ADC #$20
	STA stardat,X

	and #$e0		; Check crossing #100 line
	bne notofb

	inc stardat+1,x		; Yes, crossing
	clc
	lda stardat,x		; Move star to second half of screen
	adc #$10
	and #$1f
	sta stardat,x
notofb
	lda stardat+1,X		; Check if star leaves screen
	CMP #$98
	BNE staron
offbot
	JSR starpos		; Get random screenaddress
	JMP staron
doapix
staron
	LDA stardat+2,X		; Plot star
	EOR (stardat,X)
	STA (stardat,X)

	INX			; Datapointer to next star
	INX
	INX

	DEY			; Next star
	BNE movestar

;--- Check framerate ---

;L1B90:
;	lda #$40
;L1B98:
;	bit $b80d
;	beq L1B98
;	lda $b804

;-----------------------

	RTS

;----------------------------------------------
;starpos:
;
; Get random address between #8100-#811F
;
; Input : -
; Output: #8100<=stardat<=#811F
;----------------------------------------------

starpos	
	lda #$81
	STA stardat+1,X		; hb address =#81
	JSR rand
	AND #$1F
	STA stardat,X		; lb address 0-#1f
	RTS

;----------------------------------------------
;rand:
;
; Get random 3-byte nr
;
; Input : -
; Output: random 3 byte nr at rand1
;----------------------------------------------

rand
	LDA rand1
	AND #$48
	EOR $FC
	ADC #$38
	ASL A
	ASL A
	ROL rand1+2
	ROL rand1+1
	ROL rand1
	LDA rand1
	RTS
