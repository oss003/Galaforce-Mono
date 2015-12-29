;----------------------------------------------
;CHARP
;----------------------------------------------

;----------------------------------------------
;prnstr:
;
; Printstring with pointer in Y
;
; Input : Y = Pointer to string in strdat
;             Must be even
; Output: String on screen with datapointer
;         in strdat 
;----------------------------------------------

prnstr
	LDA strdat,Y		; Read stringdata address
	STA data
	LDA strdat+1,Y
	STA data+1

	ldy #0
	sty chrcount
	LDA (data),Y		; Read screenaddress-lb
	STA addres
	INY
	LDA (data),Y		; Read screenaddress-hb
	STA addres+1
	INY	
	LDA (data),Y		; Read string length
	STA length
	INY
	LDA (data),Y		; Read colour
	STA colour

nxtchr
	jsr setcolour		; Set colour, char even=$c0, odd=$0c
	INY

prnchr
	LDA (data),Y		; Get character
	STY savey		; Save string pointer

	STA temp		; Multiply by 5
	ASL A
	ASL A
	ADC temp
	TAX			; X=chardata pointer

	LDA #5			; Set char width
	STA width
pixcolum
	LDY #7
	LDA spfont,X		; Get chardata
	STA temp		; Address of character set (spacey)

pixcolu
	LDA #0			; colour bits
	ASL temp		; Shift out bit
	BCC pixcol0

	sty tempy		; Get screen offset 
	pha
	lda scrtab,y
	tay
	pla

	EOR (addres),Y		; Put on screen
	STA (addres),Y
	ldy tempy
pixcol0
	DEY
	BPL pixcolu

	INX			; Inc chardatapointer
	DEC width		; Check if end of string
	BEQ chrdun

 	lsr pixcolu+1		; Shift to next colourbit
 	lsr pixcolu+1
 	LDA pixcolu+1		; Check if 4 bits are plotted
	bne pixcolum

	inc addres		; Point to next screenbyte
	jsr even
	jmp pixcolum

chrdun
	lda chrcount
	and #1
	beq lab1
	inc addres
lab1
	inc chrcount
	LDY savey
	DEC length
	BNE nxtchr
	RTS

setcolour
	lda chrcount
	and #1
	beq even
	LDA #$0c
	jmp odd
even
	lda #$c0
odd
	and colour
	STA pixcolu+1
	rts
scrtab
	.db 0,$20,$40,$60,$80,$a0,$c0,$e0
tempy
	.db 0
chrcount
	.db 0

;----------------------------------------------
;strdat:
;
; Pointers to stringdata
;----------------------------------------------

strdat
	.dw paustxt		;0
	.dw SCRtext		;2
	.dw HItext		;4
	.dw entering_wave	;6
	.dw gameover		;8
	.dw pressspace		;10
	.dw atomversion		;12
	.dw myname		;14
	.dw letter_S_Q		;16
	.dw letter_K_J		;18
	.dw pressspace2		;20
	.dw finish1		;22
	.dw finish2		;24
	.dw finish3		;26
	.dw finish4		;28
	.dw copyr		;30
	.dw za			;32
	.dw zb			;34
	.dw zc			;36
;	.dw demotxt		;38

;----------------------------------------------
;Stringdata
;
; Storage: (Word) Screenaddress
;          (Byte) Stringlength
;          (Byte) Color
;          (Byte) Stringdata
;----------------------------------------------

paustxt
	.dw $971e               ; 77,31
	.db 1,yellow
	.db 25                  ; 'P'

SCRtext
	.dw $8000               ; 6,0
	.db 3,red
	.db 28,12,27		; 'SCR'

HItext
	.dw $8011               ; 42,0
	.db 2,red
	.db 17,18		; 'HI'

entering_wave
	.dw $8904               ; 17,13
	.db 16,yellow
	.db 14,23,29,14		; ENTE
	.db 27,18,23,16		; RING
	.db 38,35,24,23		; _ZON
	.db 14,38               ; E_ 
wave_text
	.db 38,38               ; __
gameover
	.dw $8909               ; 27,11
	.db 9,red
	.db 16,10,22,14		; GAME
	.db 38,24,31,14		; _OVE
	.db 27			; R

pressspace
	.dw $8e02               ; 12,22
	.db 19,green
	.db 25,27,14,28		; PRES
 	.db 28,38,28,25		; S_SP
	.db 10,12,14,38		; ACE_
 	.db 24,27,38,15		; OR_F
	.db 18,27,14		; IRE

pressspace2
	.dw $900A               ; 30,24
	.db 7,green
	.db 29,24,38,25		; TO_P
	.db 21,10,34		; LAY

atomversion
	.dw $8904               ; 27,6
	.db 16,yellow
	.db 10,23,13,38		; AND_
	.db 20,14,14,28		; KEES
	.db 38,31,10,23		; _VAN
	.db 38,24,28,28		; _OSS

myname
	.dw $8704               ; 17,8
	.db 16,yellow
	.db 11,34,38,20		; BY_K
	.db 14,31,18,23		; EVIN
	.db 38,14,13,32		; _EDW
	.db 10,27,13,28		; ARDS

letter_S_Q
	.dw $971c               ; 74,31
	.db 1,green
sound_letter
	.db  28                 ; S

letter_K_J
	.dw $971a               ; 71,31
	.db 1,red
key_joy_letter
	.db  20                 ; K

finish1
	.dw $850B               ; 32,16
	.db 6,green
	.db 28,10,29,30		; SATU
	.db 27,23		; RN

finish2
	.dw $830B               ; 30,16
	.db 7,green
	.db 11,10,29,29		; BATT
	.db 14,27,34		; ERY

finish3
	.dw $870C               ; 34,16
	.db 5,green
	.db 29,30,27,11		; TURB
	.db 24			; 0

finish4

copyr
	.dw $9403               ; 15,28
	.db 17,red
	.db 28,30,25,14		; SUPE
	.db 27,18,24,27		; RIOR
	.db 38,28,24,15		; _SOF
	.db 29,32,10,27		; TWAR
	.db 14			; E

za
	.dw $FFFF
	.db 10,red
zs
	.db  38,38,38,38,38,38,38,38,38,38

zb
	.dw $8404
	.db 20,red
zt
	.db  0			; Rank 1 to 8
	.db  38			; Space
	.db  0,1,2,3,4,5,6	; Score
	.db  38  		; Space
	.db  0,1,2,3,4,5,6,7,8,9	; Name

zc
	.dw $8204               ; 16,5
	.db 15,yellow
	.db 14,23,29,14,27,38	; ENTER_
	.db 34,24,30,27,38	; YOUR_
	.db 23,10,22,14		; NAME

;demotxt
;	.dw $970C               ; 16,5
;	.db 6,yellow
;	.db 19,24,34,2,36,9	; JOY2.9
	