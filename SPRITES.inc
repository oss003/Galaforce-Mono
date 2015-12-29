;----------------------------------------------
;SPRITES
;----------------------------------------------

;----------------------------------------------
;sprite:
;
; Erase sprite at old pos and plot at new pos
; A sprite is 16 x 16 colour pixels
;
; Input : A       = lb graphicsdata old
;	  X       = lb graphicsdata new
;	  Y       = hb graphicsdata new
;	  temp1   = hb graphicsdata old
;	  screen  = screenaddress old
;	  screen2 = screenaddress new
;	  temp1+1 = spriteheight 
; Output: -
;----------------------------------------------

sprite
	STA gra1+1	; Set lb graphicsdata old
	STA gra2+1
	STA gra3+1
	STA gra4+1

	STX gra21+1	; Set lb graphicsdata new
	STX gra22+1
	STX gra23+1
	STX gra24+1

	LDA temp1
	STA gra1+2	; Set hb graphicsdata old
	STA gra2+2
	STA gra3+2
	STA gra4+2

	STY gra21+2	; Set hb graphicsdata new
	STY gra22+2
	STY gra23+2
	STY gra24+2

	LDX #0		; Reset bytecounter
	stx rowcount	; Reset rowcounter (0-15)

;	jsr wait_sync	; No snow --------------------------
display
	lda rowcount	; Set vert.screen offset
	and #7
	tay
	lda scrtab,y
	tay
gra1
	LDA $FFFF,X	; Plot 4 pixels left outside
	EOR (screen),Y
	STA (screen),Y
gra21
	LDA $FFFF,X
	EOR (screen2),Y
	STA (screen2),Y

	INX
	iny
gra2
	LDA $FFFF,X	; Plot 4 pixels left inside
	EOR (screen),Y
	STA (screen),Y
gra22
	LDA $FFFF,X
	EOR (screen2),Y
	STA (screen2),Y

	INX
	iny
gra3
	LDA $FFFF,X	; Plot 4 pixels right inside
	EOR (screen),Y
	STA (screen),Y
gra23
	LDA $FFFF,X
	EOR (screen2),Y
	STA (screen2),Y

	INX
	iny
gra4
	LDA $FFFF,X	; Plot 4 pixels right outside
	EOR (screen),Y
	STA (screen),Y
gra24
	LDA $FFFF,X
	EOR (screen2),Y
	STA (screen2),Y

	INX

	inc rowcount	; Next row

	lda rowcount	; Increment hb after 8 rows
	cmp #8
	bne nxt_row
	inc screen+1
	inc screen2+1
nxt_row
	dec temp1+1	; Check if sprite is plot
	BNE display
	JMP the_end
the_end
	RTS

rowcount	.db 0


