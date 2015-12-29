;----------------------------------------------
;WAVE
;----------------------------------------------
corr	equ 3

wave0				; ok
	.db 10-corr		; Speed
	.db 0,2,43,1,11,10	; Wave patterns
	.db $FF			; end of wave marker
wave1				; ok
	.db 8-corr
	.db 6,7,6,7,4
	.db $FF
wave2				; ok
	.db 10-corr
	.db 5,8,9,13,14
	.db $FF
wave3				; ok
	.db 8-corr
	.db 10,11,23,24,2,3
	.db 45
	.db $FF
wave4				; ok
	.db 10-corr
	.db 16,17,18,15,5,27
	.db $FF
wave5				; ok
	.db 10-corr
	.db 6,7,6,7,12,19,20
	.db $FF
wave6				; ok
	.db 8-corr
	.db 10,11,10,11,2,3
	.db $FF
wave7				; ok
	.db 8-corr
	.db 13,14,21,22,23,24
	.db 45
	.db $FF
wave8				; ok
 	.db 10-corr
	.db 4,4,0,1,5,17,18
	.db $FF
wave9				; ok
	.db 7-corr
	.db 8,9,12,44,2
	.db $FF
wave10				; ok
	.db 8-corr
	.db 25,26,15,16,29
	.db $FF
wave11				; ok
	.db 10-corr
 	.db 27,28,12,37,38,28
	.db 45
	.db $FF
wave12				; ok
	.db 10-corr
	.db 29,30,8,9,31,32
	.db $FF
wave13				; ok
	.db 8-corr
	.db 33,34,15,16,25,26
	.db $FF
wave14				; ok
	.db 8-corr
	.db 23,24,35,36,21,22
	.db $FF
wave15				; ok
	.db 8-corr
	.db 39,40,3,10,11,41,42
	.db 45
	.db $FF
