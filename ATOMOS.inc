green		equ $ff
yellow		equ $ff
blue		equ $ff
red		equ $ff

wait_sync	equ $fe66
read_key	equ $fe71

clrscr_chr	equ $00 ; blue

;Clear 4 color mode

clear4a:
	lda #$f0
	sta $b000

clr_screen:
	lda #<$8000
	sta $80
	lda #>$8000
	sta $81
	lda #clrscr_chr
clr_loop:
	sta ($80),y
	iny
	bne clr_loop
	inc $81
	ldx $81
	cpx #>$9800
	bne clr_loop
	rts

;------------------------------------------------
; Scankey A
; - A = scancode same as $FE71
; - Exit -> A=0   = key pressed
;           A=$ff = key not pressed
;------------------------------------------------

scan_key:
	cmp #$80 ; Scan CTRL
	bne sk10
	lda $b001
	and #$40
	bne sk4
	jmp sk13
sk10:
	cmp #$81 ; Scan REPT
	bne sk11
	lda $b002
	and #$40
	bne sk4
	jmp sk13

sk11:
	cmp #$82 ; Scan SHIFT
	bne sk12
	lda $b001
	and #$80
	bne sk4
	jmp sk13

sk12:
	ldx #0
sk1:
	cmp #10
	bcc sk5
	inx
	sec
	sbc #10
	jmp sk1
sk5:
	eor #$ff
	clc
	adc #10
	sta keyrow

	lda #1
	sta keycol

	lda $b000
	and #$f0
	ora #$d0
	ora keyrow
	sta $b000
sk3:
	cpx #0     ; Set keycol mask
	beq sk2
	dex	
	asl keycol
	jmp sk3
sk2:
	lda $b001  ; Read keycol
	and keycol
	bne sk4
sk13:
	lda #0
	sec
	rts
sk4:
	lda #$ff
	clc
	rts

keyrow:  .db 0
keycol:  .db 0
