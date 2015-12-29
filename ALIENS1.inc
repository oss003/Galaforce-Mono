;----------------------------------------------
;ALIENS1
;----------------------------------------------

;----------------------------------------------
;init_new_aliens:
;
; If first entry:
; - initialize wave vars
; - Plot flags with wavenr
; - Play tune 
;
; Input : -
; Output: -
;----------------------------------------------

init_new_aliens
	LDA myst		; Check if myship visable
	BPL init_new_al2	; If yes, init aliens
	RTS

init_new_al2
	LDA initact		; Check if first entry (all 0)
	ORA almove
	ORA albullact
	BNE alien1		; Not first entry, exit

	LDY wavoff		; Set Wave offset pointer
	BNE inital1		; If not first wave, jump

	LDA (wavbase),Y		; Read nr of aliens per wave
	STA aliens		; Set nr of aliens

 	STY procst		; Start alien=0 !!!

	SEC
	SBC #1
	STA aliensm1		; Set nr of aliens-1

	INC wavoff		; Inc wave offset pointer

	INY			; Inc wave offset pointer
inital1
	INC wavoff		; Inc wave offset pointer
	LDA (wavbase),Y		; Get pattern nr
	BPL alien2		; If not end of patterns, jump 

	JSR flagson		; Erase flags

	INC curwave		; Set current wave

	LDA #0			; Reset above level 16 flag
	STA sixteen_flag

	JSR flagson		; Plot wavenr with flags

	BIT demo_flag		; Check if demomode
	BMI init_snd		; If demomode, skip starttune
	
	LDY #6			; Set music pointer

	LDA curwave		; Check wavenr
	BEQ nt_zone16		; Jump if wavenr=0
	AND #$F
	BNE nt_zone16		; Jump if 1<=wavenr<=15

	LDY #8			; Set music pointer
nt_zone16
	JSR StartTune		; Play tune
init_snd
	JSR message_loop	; Print 'ENTERING WAVE..'

	LDA curwave		; Get current wavenr
	AND #15			; Filter 0-15
	TAY			; Max 16 waves so wrap around !!! 

	LDX #0			; Reset waveoffset
	STX wavoff

	LDA vecwavl,Y
	STA wavbase		; Set lb wavedata address
	LDA vecwavh,Y
	STA wavbase+1		; Get hb wavedata address
alien1
	RTS
;----------------------------------------------
;alien2:
;
; Initialize aliens
;
; Input : A=pattern nr (45 is special)
; Output: -
;----------------------------------------------

alien2
	CMP #45			; Check special level
	BNE special

	LDX #33			; If special level, set 33 aliens
	STX aliens
 	DEX
	STX aliensm1
special
	TAY
	LDA vecpatl,Y
	STA temp1		; Set lb pattern
	LDA vecpath,Y
	STA temp1+1		; Get hb pattern

	LDY #0
	LDA (temp1),Y		; Get nr of patterns
	STA temp2		; Store nr of pattern

	INY			; Point to next 
alien4
	LDX #0			; Reset pointer
alien5
	LDA initst,X		; Loop patterns until bit7 clear
	BPL alien6		; Found, jump
	INX			; Next pattern
	CPX #maxpatt
	BNE alien5

	BEQ process_aliens	; Not found, process aliens
alien6
	LDA (temp1),Y		; Get initial X-pos alien
	BPL normal_process	; If bit7=0, jump

	AND #$7F		; Filter 0-127
	BPL alien2		; Forced jump

normal_process
	INY
	STA initx,X		; Set X-pos alien

	LDA (temp1),Y
	INY
	STA inity,X		; Set Y-pos alien

	LDA (temp1),Y
	INY
	STA initdel,X		; Set pattern delay
	STA initcount,X		; Set pattern delay counter

	LDA (temp1),Y
	INY
	STA initnum,X		; Set nr of aliens to show

	LDA (temp1),Y
	INY
	STA initrelx,X		; Set relative X-offset next alien

	LDA (temp1),Y
	INY
	STA initrely,X		; Set relative Y-offset next alien

	LDA (temp1),Y
	INY
	STA initgra,X		; Set initial alien sprite nr

 	LDA (temp1),Y
	INY
	STA initpnum,X		; Set initial pattern nr

	LDA #$80
	STA initst,X		; Mark alien as visible

	INC initact		; Inc aliens on screen counter
	DEC temp2		; Check if more then 1 pattern
	BNE alien4		; If more, init more aliens

	RTS
