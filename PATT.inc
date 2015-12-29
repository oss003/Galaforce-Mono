;----------------------------------------------
;PATT
;
; Initial pattern data
;
; byte 0, Nr of simultanious patterns
; byte 1, Initial X-pos
; byte 2, Initial Y-pos
; byte 3, Delay before next alien appears
; byte 4, Nr of aliens to show
; byte 5, Initial reletive X-move
; byte 6, Initial relative Y-move
; byte 7, Sprite nr:
;           sprite nr is alien sprite nr x2 so:
;           sprite nr 12 -> Alien 6
;                     14 ->       7
;                     ..         ..
;                     34 ->      17
; byte 8, Pattern data:
;           bit 7 = 0 -> normal pattern
;                   1 -> mirror pattern
 
;
; pattern;----------------------------------------------

patt0				; ok
	.db 1
	.db 0,24
	.db 3
	.db 20
	.db 0,4
	.db 12
	.db 0

patt1				; ok
	.db 1
	.db 28,24
	.db 3
	.db 20
	.db 0,4
	.db 12
	.db 0+$80

patt2				; ok
	.db 1
	.db 0,8
	.db 2
	.db 30
	.db 0,0
	.db 18
	.db 1

patt3				; ok
	.db 1
	.db 28,8
	.db 2
	.db 30
	.db 0,0
	.db 18
	.db 1+$80

patt4
	.db 2			; ok
	.db 0,160
	.db 20
	.db 2
	.db 8,-24
	.db 20
	.db 2
; 2nd half
	.db 28,160
	.db 20
	.db 2
	.db -8,-24
	.db 20
	.db 2+$80

patt5				; ok
	.db 1
	.db 0,8 
	.db 50
	.db 4
	.db 0,16
	.db 24
	.db 4

patt6				; ok
	.db 2
	.db 0,150
	.db 8
	.db 10
	.db 0,0
	.db 24
	.db 5
; 2nd part
	.db 4,150
	.db 8
	.db 10
	.db 0,0
	.db 20
	.db 5

patt7				; ok
	.db 2
	.db 28,150
	.db 8
	.db 10
	.db 0,0
	.db 24
	.db 5+$80
; 2nd part
	.db 24,150
 	.db 8
	.db 10
	.db 0,0
	.db 20
	.db 5+$80

patt8				; ok
	.db 1
	.db 4,8
	.db 6
	.db 15
	.db 0,0
 	.db 20
 	.db 6
 	
patt9				; ok
 	.db 1
 	.db 24,8
 	.db 6
 	.db 15
 	.db 0,0
 	.db 20
 	.db 6+$80
 	
patt10				; ok
 	.db 1
 	.db 0,32
 	.db 5
 	.db 8
 	.db 0,0
 	.db 16
 	.db 7
 	
patt11				; ok
 	.db 1
 	.db 28,48
 	.db 5
 	.db 8
 	.db 0,0
 	.db 16
 	.db 7+$80
 	
patt12				; ok
 	.db 2
 	.db 15,8
 	.db 6
 	.db 12
 	.db 0,0
 	.db 30
 	.db 8
; 2nd part
 	.db 11,8
 	.db 6
 	.db 12
 	.db 0,0
 	.db 30
 	.db 8+$80
 	
patt13				; ok
 	.db 1
 	.db 0,114
 	.db 6
 	.db 16
 	.db 0,0
 	.db 12
 	.db 9
 	
patt14				; ok
 	.db 1
 	.db 28,114
 	.db 6
 	.db 16
 	.db 0,0
 	.db 12
 	.db 9+$80
 	
patt15				; ok
 	.db 1
 	.db 0,160
 	.db 5
 	.db 12
 	.db 0,0
 	.db 18
 	.db 10
 	
patt16				; ok
 	.db 1
 	.db 28,160
 	.db 5
 	.db 12
 	.db 0,0
 	.db 18
 	.db 10+$80
 	
patt17				; ok
 	.db 1
 	.db 0,32
 	.db 7
 	.db 12
 	.db 0,0
 	.db 14
 	.db 11
 	
patt18				; ok
 	.db 1
 	.db 28,32
 	.db 7
 	.db 12
 	.db 0,0
 	.db 14
 	.db 11+$80
 	
patt19				; ok
 	.db 1
 	.db 20,8
 	.db 5
 	.db 10
 	.db 0,0
 	.db 16
 	.db 13
 	
patt20				; ok
 	.db 1
 	.db 8,8
 	.db 5
 	.db 10
 	.db 0,0
 	.db 16
 	.db 13+$80
 	
patt21				; ok
 	.db 1
 	.db 0,32
 	.db 5
 	.db 8
 	.db 0,0
 	.db 20
 	.db 14
 	
patt22				; ok
 	.db 1
 	.db 28,32
 	.db 5
 	.db 8
 	.db 0,0
 	.db 20
 	.db 14+$80
 	
patt23				; ok
 	.db 1
 	.db 2,160
 	.db 3
 	.db 20
 	.db 0,0
 	.db 20
 	.db 15
 	
patt24				; ok
 	.db 1
 	.db 26,160
 	.db 3
 	.db 20
 	.db 0,0
 	.db 20
 	.db 15+$80
 	
patt25				; ok
 	.db 1
 	.db 0,80
 	.db 3
 	.db 16
 	.db 0,0
 	.db 16
 	.db 16
 	
patt26				; ok
 	.db 1
 	.db 28,80
 	.db 3
 	.db 16
 	.db 0,0
 	.db 16
 	.db 16+$80
 	
patt27				; ok
 	.db 1
 	.db 0,72
 	.db 4
 	.db 14
 	.db 0,0
 	.db 18
 	.db 17
 	
patt28				; ok
 	.db 1
 	.db 28,72
 	.db 4
 	.db 14
 	.db 0,0
 	.db 18
 	.db 17+$80
 	
patt29				; ok
 	.db 1
 	.db 0,8
 	.db 2
 	.db 21
 	.db 3,0
 	.db 20
 	.db 18
 	
patt30				; ok
 	.db 1
 	.db 28,8
 	.db 2
 	.db 21
 	.db -3,0
 	.db 20
 	.db 18+$80
 	
patt31				; ok
 	.db 1
 	.db 28,8
 	.db 4
 	.db 12
 	.db 0,0
 	.db 32
 	.db 19
 	
patt32				; ok
 	.db 1
 	.db 0,8
 	.db 4
 	.db 12
 	.db 0,0
 	.db 32
 	.db 19+$80
 	
patt33				; ok
 	.db 1
 	.db 30,8
 	.db 7
 	.db 20
 	.db 0,0
 	.db 18
 	.db 20
 	
patt34				; ok
 	.db 1
 	.db 31,8
 	.db 7
 	.db 20
 	.db 0,0
 	.db 18
 	.db 20+$80
 	
patt35				; ok
 	.db 1
 	.db 28,8
 	.db 4
 	.db 14
 	.db 0,0
 	.db 34
 	.db 21
 	
patt36				; ok
 	.db 1
 	.db 0,8
 	.db 4
 	.db 14
 	.db 0,0
 	.db 34
 	.db 21+$80
 	
patt37				; ok
 	.db 1
 	.db 0,8
 	.db 4
 	.db 18
 	.db 0,0
 	.db 16
 	.db 22
 	
patt38				; ok
 	.db 1
 	.db 27,8
 	.db 4
 	.db 18
 	.db 0,0
 	.db 16
 	.db 22+$80
 	
patt39				; ok
 	.db 1
 	.db 30,8
 	.db 5
 	.db 12
 	.db 0,0
 	.db 20
 	.db 23
 	
patt40				; ok
 	.db 1
 	.db 30,8
 	.db 5
 	.db 12
 	.db 0,0
 	.db 20
 	.db 23+$80
 	
patt41				; ok
 	.db 1
 	.db 0,8
 	.db 4
 	.db 19
 	.db 2,0
 	.db 12
 	.db 24
 	
patt42				; ok
 	.db 1
 	.db 28,8
 	.db 4
 	.db 19
 	.db -2,0
 	.db 12
 	.db 24+$80
 	
patt43				; ok
 	.db 2
 	.db 0,8		;0,8
 	.db 16
 	.db 20
 	.db 0,0
 	.db 16
 	.db 25
; 2nd part
 	.db 28,8	;28,8
 	.db 14
 	.db 20
 	.db 0,0
 	.db 16
 	.db 25+$80
 	
patt44				; ok
 	.db 2
 	.db 0,16
 	.db 8
 	.db 20
 	.db 0,0
 	.db 18
 	.db 26
; part2
 	.db 28,16
 	.db 8
 	.db 20
 	.db 0,0
 	.db 18
 	.db 26+$80
 	
patt45				; ok
 	.db 3
 	.db 0,8
 	.db 1
 	.db 1
 	.db 0,0
 	.db 22
 	.db 29
; part 2
 	.db 2,40
 	.db 2
 	.db 11
 	.db 0,0
 	.db 26
 	.db 27
; part 3
 	.db 2,64
 	.db 2
 	.db 10
 	.db 0,0
 	.db 28
 	.db 28
