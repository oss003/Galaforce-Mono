;----------------------------------------------
;TITLE
;----------------------------------------------

;----------------------------------------------
;title
;
; Print title on screen in double width and height
;
; Input : -
; Output: Print title on screen in double height and width
;----------------------------------------------

text_pos	equ $8303
titcol		equ $50

title
	LDA #<text_pos		; temp2=upper line (8 pix) 
	STA temp2
	LDA #<(text_pos+$100)
	STA temp3		; temp3=lower line (8 pix)
	LDA #>text_pos
	STA temp2+1
	LDA #>(text_pos+$100)
	STA temp3+1

	lda #titcol		; Set colour
	sta titcolour

	LDX #0
	stx titflag		; Flag for inc.screenbyte
	STX temp4+1		; String pointer
gchr
	LDX temp4+1		; Get stringpointer
	LDA titnam,X		; Read string character
	BPL gch1
	BEQ gch1
	jmp tit6		; Stop if end of string
gch1:
	STA temp1		; Save character

	ASL A			; Calculate chardata pointer
	ASL A
	CLC
	ADC temp1
	TAX

	LDA #5			; Charwidth
	STA temp4
tit1
	LDA spfont,X		; Get chardata
	STA temp1

	LDA #4			; Set pixelcounter
	STA temp1+1
	LDY #0
tit2
	LSR temp1		; Shift out char pixel
	BCC tit3		; No pixel

	sty tempy		; Get screen offset 
	lda scrtab,y
	tay
	LDA titcolour		; Plot pixel line1
	EOR (temp2),Y
	STA (temp2),Y
	ldy tempy

	INY

	sty tempy		; Get screen offset 
	lda scrtab,y
	tay
	LDA titcolour		; Plot double pixel line1
	EOR (temp2),Y
	STA (temp2),Y
	ldy tempy

	DEY
tit3
	LDA #8			; Filter pixel for line2
	AND temp1
	BEQ tit4		; No pixel

	sty tempy		; Get screen offset 
	lda scrtab,y
	tay
	LDA titcolour		; Plot pixel line2
	EOR (temp3),Y
	STA (temp3),Y
	ldy tempy

	INY

	sty tempy		; Get screen offset 
	lda scrtab,y
	tay
	LDA titcolour		; Plot double pixel line2
	EOR (temp3),Y
	STA (temp3),Y
	ldy tempy

	DEY
tit4
	INY			; Set screenpointer 2 pixels down
	INY

	DEC temp1+1		; Last pixel for byte?
	BNE tit2

	JSR titrig		; Shift mask for second half

	INX			; Next string character

	DEC temp4		; Check if all charpixels are shifted
	BNE tit1

	JSR titrig		; Shift mask for first half
	INC temp4+1		; Increment stringpointer 
	jmp gchr		; always

titrig				; Shift color
	lda titcolour
	eor #$55
	sta titcolour
	lda titflag
	eor #$ff
	sta titflag
	bne tit6
	inc temp2
	inc temp3
tit6
	RTS

;----------------------------------------------
;Stringdata
;----------------------------------------------

titnam				; Title string
	.db 16,10,21,10		;GALA
	.db 15,24,27,12		;FORC
	.db 14,$FF		;E

titflag		.db 0
titcolour	.db 0

