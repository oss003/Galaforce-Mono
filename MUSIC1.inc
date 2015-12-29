;----------------------------------------------
;MUSIC1
;----------------------------------------------

StartTune
 	bit sound_flag
	bpl L3067

	lda MUSPTR,y
	sta L3064+1
	sta L3065+1
	sta L3066+1

	lda MUSPTR+1,y
	sta L3064+2
	sta L3065+2
	sta L3066+2

	lda #0
	sta $f3
tune_loop:
	ldx $f3
L3064:
	lda $ffff,x
	beq L3067
	pha
	inx
L3065:
	lda $ffff,x
	pha
	inx
L3066:
	lda $ffff,x
	tay
	pla
	tax
	pla
	jsr sound

	inc $f3
	inc $f3
	inc $f3
	jmp tune_loop
L3067
	rts

MUSPTR:      ;  0    2    4     6       8
	.word START,DIE,OVER,NEXTZONE,BONUS

Refresh:
	rts
MusicTest:
	rts
