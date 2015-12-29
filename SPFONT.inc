;----------------------------------------------
;SPFONT
;
; SPFONT is storage of characters at $700
; A character is stored in landscape
; Every character is stored in 5 bytes (8 x 5 pixels)
; A character is displayed on screen as 5 x 8 pixels (portrait)
; Every screenpixel is a coloured pixel (2 bits)
; Every character is 1.5 bytes width (width 1 pixel space)
;
; eg:    Storage    Screen
;
;    $78 .1111...   .111.
;    $7F .1111111   .1.1.
;    $09 ....1..1   .1.1.
;    $0F ....1111   11111
;    $78 .1111...   11..1
;                   11..1
;                   11..1
;                   .....
;----------------------------------------------

spfont

; REM 012345
	.db  $7F,$41,$41,$79,$7F
	.db  $00,$78,$7F,$00,$00
	.db  $79,$79,$49,$49,$4F
	.db  $63,$49,$49,$7F,$78
	.db  $1F,$11,$71,$7F,$10
	.db  $6F,$49,$49,$79,$79
; REM 6789
	.db  $7F,$49,$49,$49,$7B
	.db  $01,$01,$01,$79,$7F
	.db  $78,$4F,$49,$7F,$78
	.db  $0F,$09,$09,$79,$7F
; REM ABCDEF
	.db  $78,$7F,$09,$0F,$78
	.db  $7F,$79,$49,$4F,$38
	.db  $7F,$79,$41,$41,$63
	.db  $7F,$79,$41,$41,$7F
	.db  $7F,$79,$49,$49,$49
	.db  $7F,$79,$09,$09,$09
; REM GHIJKL
	.db  $7F,$79,$41,$49,$7B
	.db  $7F,$78,$08,$08,$7F
	.db  $00,$7F,$78,$00,$00
	.db  $70,$40,$40,$7F,$78
	.db  $7F,$78,$08,$0F,$78
	.db  $7F,$78,$40,$40,$40
; REM MNOPQR
	.db  $7F,$01,$7F,$01,$7F
	.db  $7F,$79,$01,$01,$7F
	.db  $7F,$41,$41,$43,$7F
	.db  $7F,$79,$09,$09,$0F
	.db  $7F,$41,$61,$61,$7F
	.db  $7F,$79,$09,$0F,$78
; REM STUVWX
	.db  $4F,$49,$49,$79,$79
	.db  $01,$01,$7F,$79,$01
	.db  $7F,$78,$40,$40,$7F
	.db  $0F,$7F,$40,$70,$0F
	.db  $7F,$40,$7F,$40,$7F
	.db  $77,$78,$08,$08,$77
; REM YZ.- space 
	.db  $0F,$08,$78,$08,$0F
	.db  $7B,$79,$49,$49,$6F
	.db  $00,$00,$60,$00,$00
	.db  $80,$80,$80,$80,$80
	.db  $00,$00,$00,$00,$00

