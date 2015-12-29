;----------------------------------------------
;ZPWORK $0
;----------------------------------------------

screen		equ $82    ; 2
data		equ $84    ; 2
screen2		equ $86    ; 2
temp		equ $88    ; 2
temp1		equ $8a    ; 2
width		equ $8c    ; 1
temp2		equ $90    ; 2
addres		equ $92    ; 2
temp3		equ $94    ; 2
addres1		equ $96    ; 2
temp4		equ $98    ; 2
wavbase		equ $9a		; 2,    Address pointing to wavebase data
stardat		equ $a0		; 3*23, $A0-$E5

colour		equ $260   ; 1
savey		equ $261   ; 1

length		equ $265   ; 1

rand1		equ $26b	; Random 3 byte nr

whichstar	equ $26e   ; 1
counter		equ $26f   ; 1
expldelay	equ $270   ; 1

aliens		equ $299	; Nr of aliens per wave
aliensm1	equ $29a	; Nr of aliens per wave-1

;----------------------------------------------
; Actual pattern storage for initial values
;----------------------------------------------

initst		equ $271	; Status
initx		equ $275	; Initial alien X-pos
inity		equ $279	; Initial alien Y-pos
initdel		equ $27d	; Initial pattern delay value
initcount	equ $281	; Pattern delay counter before pattern starts
initnum		equ $285	; Numbers of aliens to show
initrelx	equ $289	; Initial relative X-movement
initrely	equ $28d	; Initial relative Y-movement
initgra		equ $291	; Initial alien graphic sprite 
initpnum	equ $295	; Initial pattern nr

