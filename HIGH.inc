;----------------------------------------------
;HIGH
;----------------------------------------------

;----------------------------------------------
;ahigh:
;
; Check if myscore in highscoretable
; If so shift highscores and highnames in table
; Input name
; Add myscore and myname to highscoretable
;----------------------------------------------

ahigh
	BIT demo_flag		;If demomode, skip reset highscoretable
	BMI ahdem

	LDX #0
	STX temp1
ah1
	LDA ti10,X		; Set pointer names
	STA temp2
	LDY ti7,X		; Set pointer score
	STY temp1+1

	LDX #$FF
ah2
	INY			; Compare myscore with hiscore
	INX
	CPX #7
	BEQ ah4
	LDA myscore,X
	AND #$7F
	CMP hsnum,Y
	BEQ ah2			; If digit myscore=highscore check next
	BCC ah3			; If digit myscore>hiscore next rank
	BNE ah4			; If digit myscore<hiscore rank found
ah3
	INC temp1		; Next rank
	LDX temp1
	CPX #8			; All ranks tested?
	BCC ah1
ahdem
	RTS
ah4 
	LDA temp1+1		; Check if myscore=rank8
	CMP #7*7-1
	BEQ ah5

	LDY #7*7-1		; Shift ranks down to insert myscore
ah6
	LDA hsnum,Y
	STA hsnum+7,Y
	DEY
	CPY temp1+1
	BNE ah6

	LDY #10*7-1		; Shift names down to insert myname
ah7
	LDA hstxt,Y
	STA hstxt+10,Y
	DEY
	CPY temp2
	BNE ah7
ah5
	LDX #0			; Reset myscorepointer
	LDY temp1+1		; Set highscorepointer
	INY
ah8
	LDA myscore,X		; Copy myscore into highscore table
	AND #$7F
	STA hsnum,Y
	INY
	INX
	CPX #7
	BNE ah8

	LDX #9			; Reset mynamepointer
	LDY temp2		; Set highnamepointer
	INY
	TYA
	PHA			; Save namepointer

	LDA #38			; Load space character
ah9
	STA hstxt,Y		; Store new highname
	STA zs,x		; Clear inputname
	INY
	DEX
	BPL ah9

	LDA temp1		; Set datapointer
	ASL A
	ASL A
	ASL A
	ASL A
	CLC
	ADC #4*8
	PHA			; Save scorepointer

	JSR pht			; Print highscore table

	LDY #36			; Print 'ENTER YOUR NAME'
	JSR prnstr

	PLA			; Get scorepointer
	TAY
	LDX #16			; Position name
	JSR xycalc2		; Calc screenaddress rankentry

	STA za+1		; Set screenaddress for inputstring
	STX za
	PLA			; Get namepointer
	TAX			; X=namepointer
	JSR gnam		; Get name

	LDY #36			; Erase 'ENTER YOUR NAME'
	JSR prnstr

	JMP pht			; Erase High score table

;---------------------------------
;pht:
;
; Print high score table on screen
;
; Input : -
; Output: High Score Table on screen
;---------------------------------

tablestartadr	equ $8401

pht
	LDA #0			; Set rank counter
	STA wavbase

	LDA #>tablestartadr	; Set string position
	STA zb+1
	LDA #<tablestartadr
	STA zb
ai1
	LDX wavbase
	INX
	STX zt			; Set rank nr

	DEX			; Set score pointer
	LDY ti7,X
	INY

	LDX #0
	STX wavbase+1		; Set 0 score
ai2
	LDA hsnum,Y		; Read score digit
	BNE ph2
	CMP wavbase+1		; Check if leading zero
	BNE ph2
	LDA #38			; Was leading zero so print space
	BNE ph3
ph2
	SEC			; Mark ending leading zero's
	ROR wavbase+1
ph3
	STA zt+2,X		; Store digit or space
	INY			; Next score digit
	INX			; Stringpointer to next char
	CPX #7			; Last score digit?
	BNE ai2			; no

	LDX wavbase
	LDY ti10,X		; Set name pointer
	INY
	LDX #0
ai3
	LDA hstxt,Y		; Read name char
	STA zt+10,X		; Store name char
	INY			; Next name char
	INX			; Stringpointer to next char
	CPX #10			; Last name char?
	BNE ai3			; No

	LDY #34			; Print rank,score and name
	JSR prnstr

	LDA zb+1		; Point down 2 lines
	CLC
	ADC #2
	STA zb+1

	INC wavbase		; Next rank nr
	LDA wavbase
	CMP #8
	BCC ai1
	RTS

;---------------------------------
;gnam:
;
; Input name (10 chars) for highscore table
;
; Input : X=namepointer
; Output: -
;---------------------------------

gnam
	LDY #0			; Set stringpointer to 10
	jmp gn1			; Forced entry
gn3
	jsr pit
 	LDA #7			; Beep
	JSR oswrch
gn1
	STX wavbase
	STY wavbase+1
gn6
	ldx #15			; Wait a while
	jsr delay2
	JSR movestars		; Move stars

	JSR read_key		; Read key
	cpy #$ff
	beq gn6

	TYA
	pha			; Save key

wait_release
	ldx #15			; Wait a while
	jsr delay2
	JSR movestars		; Move stars
	jsr read_key		; Wait until key is released
	cpy #$ff
	bne wait_release

	LDY #32			; Erase namestring 
	JSR prnstr

	pla			; Get key

	LDX wavbase
	LDY wavbase+1

	CMP #$D			; Return?
	BEQ gn2

	CMP #$0F		; Delete?
	BEQ hdel

	CPY #10			; Check for end of inputstring
	BNE gn10
	JMP gn3
gn10
	CMP #0			; Space?
	BNE gn7
	LDA #38			; Get space character
	BNE gn8			; Forced jump
gn7
	CMP #$21		; Check if >= 'A'
	BCC gn9
	CMP #$3a+1		; Check if <= 'Z'
	BCS gn3

	SBC #22			; Convert to SPFONT char
	BNE gn8
gn9
	CMP #$10		; Check if >= '0'
	BCC gn3
	CMP #$19+1		; Check if <= '9'
	BCS gn3

	SBC #15			; Convert to SPFONT digit
gn8
	STA zs,y		; Store char in string
	STA hstxt,X		; Store char in highscore name table
	JSR pit			; Print character

;	inc za			; Point to next char

	INX			; Inc table pointer
	INY			; Dec stringpointer
	jmp gn1
gn2
	LDY #32			; Erase namestring 
	JSR prnstr
	RTS

;---------------------------------
; Delete character
;---------------------------------

hdel
	CPY #0			; Check if end of string
	BEQ hd1
	dey
	dex			; If so, error bleep
hd1
	LDA #38
	STA zs,y		; Previous char
	STA hstxt,X		; Get char from highscore table
	jsr pit
	JMP gn1			; Exit

;---------------------------------
; Print namestring on screen
;---------------------------------

pit
	TXA
	PHA			; Save X
	TYA
	PHA			; Save Y

	LDY #32			; Print namestring 
	JSR prnstr

	PLA
	TAY			; Restore Y
	PLA
	TAX			; Restore X
	RTS

;---------------------------------
; Pointers to highscore tables
;---------------------------------

ti7				; Pointer to high scores
	.db 0-1,7-1,14-1,21-1
	.db 28-1,35-1,42-1,49-1

ti10				; Pointer to high score names
	.db 0-1,10-1,20-1,30-1
	.db 40-1,50-1,60-1,70-1
 
