;----------------------------------------------
;FLAGS
;----------------------------------------------

;----------------------------------------------
;liveson:
;
; Plot ship for every live from right to left
;
; Input : -
; Output: For every life a ship plotted
;----------------------------------------------

ship_pos	equ $961e

liveson
	LDX lives		; Save lives
	stx tmplives
	BEQ noliveson		; Check if no lives left

	LDA #>ship_pos		; Set startaddress screen first ship
	STA addres+1
	LDA #<ship_pos
	STA addres
liveon0
	LDX #0			; Plot ship as 8 x 8 sprite
liveon1
	ldy scrtab,x
	LDA shipgra,X
	EOR (addres),Y
	STA (addres),Y		; Left sprite byte

	iny
	LDA shipgra+8,X
	EOR (addres),Y
	STA (addres),Y		; Right sprite byte

	inx			; Next pixelrow
	cpx #8
	bne liveon1

	dec addres		; Calc next screenaddress
	dec addres

	dec tmplives		; Check if all ships plotted 
	BNE liveon0
noliveson
	RTS

tmplives	.db 0

;----------------------------------------------
;flagson:
;
; Draw flags to see wavenr 
; The flags are signed 1, 5 and 10 levels
;
; Input : -
; Output: Flags represent wavenr
;----------------------------------------------

flag_top_pos	equ $9600
flag_bot_pos	equ $9700

flagson
	LDY curwave
	INY
	STY temp		; Set wavenr

	LDA #>flag_top_pos	; Set top position flag
	STA addres+1
	LDA #<flag_top_pos
	STA addres

	LDA #>flag_bot_pos	; Set bottom position flag
	STA addres1+1
	LDA #<flag_bot_pos
	STA addres1
flag0
	LDA temp
	BEQ nomoflags		; Check all flags drawn

	CMP #10			; Check to draw flag with 10
	BCC flag1

	SEC			; Decrement wave with 10
	SBC #10
	STA temp

	LDX #0			; Flag 10 data at #0
flago0
	LDY #0			; Reset sprite line counter
	sty flgcounter
flago1
	ldy flgcounter
	LDA flaggra,X
	EOR (addres),Y
	STA (addres),Y		; Plot flagsprite top left

	LDA flaggra+16,X
	EOR (addres1),Y
	STA (addres1),Y		; Plot flagsprite bottom left

	iny
	lda flaggra+8,x
	EOR (addres),Y
	STA (addres),Y		; Plot flagsprite top right

	LDA flaggra+24,X
	EOR (addres1),Y
	STA (addres1),Y		; Plot flagsprite bottom right

	inx
	clc
	lda flgcounter
	adc #$20
	sta flgcounter		; Next pixel line
	BNE flago1

	inc addres		; Next address screen position
	inc addres
	inc addres1
	inc addres1

	BNE flag0
nomoflags
	RTS

flgcounter	.db 0

flag1
	CMP #5			; Check to draw flag with 5
	BCC flag2

	SEC			; Decrement wave with 5
	SBC #5
	STA temp

	LDX #32			; Flag 5 data at #27
	BNE flago0		; Draw flag
flag2
	SEC 			; Decrement wave with 1
	SBC #1
	STA temp

	LDX #64			; Flag 1 data at #54
	BNE flago0		; Draw flag

;----------------------------------------------
;Spritedata flags and lives
;----------------------------------------------

flaggra
	.db $1f,$1f,$1f,$13,$13,$13,$13,$13	; Flag 10
	.db $00,$c0,$fc,$03,$33,$33,$33,$03
	.db $1f,$1f,$1f,$10,$10,$10,$10,$10
	.db $fc,$c0,$00,$00,$00,$00,$00,$00

	.db $10,$1f,$1f,$1c,$1c,$1c,$1f,$1c	; Flag 5
	.db $00,$00,$c0,$30,$fc,$3c,$3c,$30
	.db $1f,$1f,$10,$10,$10,$10,$10,$10
	.db $c0,$00,$00,$00,$00,$00,$00,$00

	.db $10,$1c,$1f,$1c,$1c,$1c,$1c,$1c	; Flag 1
	.db $00,$00,$00,$c0,$f0,$fc,$f0,$c0
	.db $1f,$1c,$10,$10,$10,$10,$10,$10
	.db $00,$00,$00,$00,$00,$00,$00,$00

shipgra
	.db $00,$00,$00,$13,$3f,$3c,$3f,$03	; Life
	.db $40,$c0,$c0,$f1,$3f,$0f,$ff,$30
