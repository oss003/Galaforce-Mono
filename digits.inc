;----------------------------------------------
;DIGITS
;
; Storage of digits 0-9 for the score
; Digits are stored in a 2 x 8 x 8 format for fast access
; A digit is 5 x 8 coloured pixels + 1 pixel space
; A digit has a left- and a right byte 
; Because a character is 1.5 bytes width a digit
; can be plotted aligned left or right
;----------------------------------------------
digits

digits_l					; left digit left aligned
	.db $ff,$c0,$c0,$c3,$c3,$c3,$ff,$00 ; 0
	.db $0c,$0c,$0c,$3c,$3c,$3c,$3c,$00 ; 1
	.db $ff,$00,$00,$ff,$f0,$f0,$ff,$00 ; 2
	.db $ff,$c3,$03,$3f,$03,$c3,$ff,$00 ; 3
	.db $ff,$c3,$c3,$c3,$ff,$0f,$0f,$00 ; 4
	.db $ff,$c0,$c0,$ff,$03,$c3,$ff,$00 ; 5
	.db $ff,$c0,$c0,$ff,$c0,$c0,$ff,$00 ; 6
	.db $ff,$00,$00,$03,$03,$03,$03,$00 ; 7
	.db $3f,$33,$33,$ff,$c3,$c3,$ff,$00 ; 8
	.db $ff,$c0,$c0,$ff,$03,$03,$03,$00 ; 9
digits_r					; left digit right aligned
	.db $0f,$0c,$0c,$0c,$0c,$0c,$0f,$00 ; 0
	.db $00,$00,$00,$03,$03,$03,$03,$00 ; 1
	.db $0f,$00,$00,$0f,$0f,$0f,$0f,$00 ; 2
	.db $0f,$0c,$00,$03,$00,$0c,$0f,$00 ; 3
	.db $0f,$0c,$0c,$0c,$0f,$00,$00,$00 ; 4
	.db $0f,$0c,$0c,$0f,$00,$0c,$0f,$00 ; 5
	.db $0f,$0c,$0c,$0f,$0c,$0c,$0f,$00 ; 6
	.db $0f,$00,$00,$00,$00,$00,$00,$00 ; 7
	.db $03,$03,$03,$0f,$0c,$0c,$0f,$00 ; 8
	.db $0f,$0c,$0c,$0f,$00,$00,$00,$00 ; 9
end_digits_r
	.blkb (256-end_digits_r+digits_l),0

digits1_l					; right digit left aligned
	.db $c0,$c0,$c0,$c0,$c0,$c0,$c0,$00 ; 0
	.db $00,$00,$00,$00,$00,$00,$00,$00 ; 1
	.db $c0,$c0,$c0,$c0,$00,$00,$c0,$00 ; 2
	.db $00,$00,$00,$c0,$c0,$c0,$c0,$00 ; 3
	.db $00,$00,$00,$00,$c0,$00,$00,$00 ; 4
	.db $c0,$00,$00,$c0,$c0,$c0,$c0,$00 ; f
	.db $c0,$c0,$00,$c0,$c0,$c0,$c0,$00 ; c
	.db $c0,$c0,$c0,$c0,$c0,$c0,$c0,$00 ; 7
	.db $00,$00,$00,$c0,$c0,$c0,$c0,$00 ; 8
	.db $c0,$c0,$c0,$c0,$c0,$c0,$c0,$00 ; 3
digits1_r					; right digit right aligned
	.db $fc,$0c,$0c,$3c,$3c,$3c,$fc,$00 ; 0
	.db $c0,$c0,$c0,$c0,$c0,$c0,$c0,$00 ; 1
	.db $fc,$0c,$0c,$fc,$00,$00,$fc,$00 ; 2
	.db $f0,$30,$30,$fc,$3c,$3c,$fc,$00 ; 3
	.db $f0,$30,$30,$30,$fc,$f0,$f0,$00 ; 4
	.db $fc,$00,$00,$fc,$3c,$3c,$fc,$00 ; f
	.db $fc,$0c,$00,$fc,$0c,$0c,$fc,$00 ; c
	.db $fc,$0c,$0c,$3c,$3c,$3c,$3c,$00 ; 7
	.db $f0,$30,$30,$fc,$3c,$3c,$fc,$00 ; 8
	.db $fc,$0c,$0c,$fc,$3c,$3c,$3c,$00 ; 3




