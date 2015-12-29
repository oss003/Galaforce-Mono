;----------------------------------------------
;ALIENS2
;----------------------------------------------

;----------------------------------------------
;proces_aliens:
;
; Check 4 patterns per wave
; Check for every pattern if alien has to be initialized
; If so, plot alien
;
; Input : -
; Output: -
;----------------------------------------------

process_aliens
	LDA myst			; Check if myship is vivible
	BPL process_ok			; Yes, jump
	LDX #2				; No, delay
	JMP delay2			; Exit

;--- Loop patterns ---

process_ok
	LDX #maxpatt-1			; Set max patterns
proc1
	LDA initst,X			; Check if pattern active
	BPL proc2			; Not active, skip

	LDA initcount,X			; Check delay before start pattern
	BEQ proc6			; Delay ended, start pattern
	DEC initcount,X			; Dec delay counter
proc2
	DEX				; Next pattern
	BPL proc1
	RTS

;--- Loop aliens to find empty space ---
;--- X = pattern pointer ---

proc6
	LDA initdel,X			; Reset delay counter
	STA initcount,X

	LDY aliensm1
proc3
	LDA alst,Y			; Check alien status
	BPL proc4			; If not visible, use this
	DEY
	BPL proc3			; Next alien		
	BMI proc2			; Forced jump

;--- Empty space found, add alien ---
;--- X = pattern pointer ---
;--- Y = alien pointer ---

proc4
	LDA #0
	STA alpatoff,Y			; Reset pattern offset pointer
 	STA almult,Y			; Reset mulplier

	LDA #$80			; Mark alien as visible
	STA alst,Y 

	LDA initx,X			; Set X-pos alien
	STA alx,Y
 	PHA				; Save alien X-pos

	LDA inity,X			; Set Y-pos alien
	STA aly,Y

	LDA initgra,X			; Set alien graphic sprite
	STA algra,Y
	STA temp3			; Save graphic sprite nr

	SEC				; Get alien sprite nr = (graphic-12)/2
	SBC #12
	LSR A

	STX temp2+1			; Save pattern pointer

	TAX				; Set alien pointer
	LDA alien_hits,X		; Get hits needed to kill alien from database
	STA alcount,Y			; Set alien hitcounter

	LDX temp2+1			; Get pattern pointer
	LDA initpnum,X			; Get initial pattern nr
	STA alpatreflect,Y		; Set pattern for alien

	AND #$7F			
	TAX				; Set patterdata pointer
	LDA vecpatdl,X
	STA alpatlow,Y			; Set alien pattern data address
	LDA vecpatdh,X
	STA alpathigh,Y

	PLA				; Get alien X-pos
	TAX				; Save X-pos

	JSR rand			; Get random nr
	AND #15				; Filter between 0-15

	CPX #30				; Check limit right
	BEQ alter_start_x

	CPX #31				; If over right border, don't change X-pos
	BNE use_original_x

	ADC #15				; Set random position right half
alter_start_x
	STA alx,Y			; Set X-pos alien

use_original_x
	LDX temp2+1			; Get pattern pointer
	INC almove			; Set alien to be moved
	DEC initnum,X			; Dec alien to show counter
	BNE proc5			; If not all aliens on screen, jump

	LDA #0				; All aliens on screen, reset status
	STA initst,X
	DEC initact			; Dec actual aliens on screen pointer
proc5
	CLC
	LDA initx,X			; Adjust init X-pos for next alien
	ADC initrelx,X
chk_xinit_wrap
	CMP #32				; Check limit right
	BCC no_initx_wrap
	SBC #32				; If over right limit, wrap left
	JMP chk_xinit_wrap		; Check again

no_initx_wrap
	STA initx,X			; Set new init X-pos alien

	CLC
	LDA inity,X			; Adjust init Y-pos for next alien
	ADC initrely,X
	STA inity,X			; Set new init Y-pos alien

	LDX alx,Y			; Get alien X-pos
	LDA aly,Y			; Get alien Y-pos
	TAY
	JSR alien_on_off		; Plot alien

	LDX temp2+1			; Get pattern pointer
	JMP proc2			; Next pattern

;----------------------------------------------
;alien_on_off:
;
; Plot/erase alien
;
; Input : X=X-pos alien
;         Y=Y-pos alien
;         temp3=spritenr
; Output: -
;----------------------------------------------

alien_on_off
	JSR xycalc			; Calculate screenaddress
	STA screen+1
	STX screen
	LDA #alheight
	STA temp1+1			; Set spriteheight

	LDA #$C0			; Make sure sprite is plot
	STA screen2+1

	LDX temp3			; Get spritepointer
	LDA graph,X			; Get lb graphics spritedata
	LDY graph+1,X			; Get hb graphics spritedata
	STY temp1
	JMP sprite			; Plot sprite
