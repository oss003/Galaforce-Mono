;----------------------------------------------
;ROUT4
;----------------------------------------------

;----------------------------------------------
;message_loop:
;
; Print message 'ENTERING WAVENR xx'
; If wavenr=16,32 or 48 print message
; Wavenr 16 -> 'SATURN'
;        32 -> 'BATTERY'
;        48 -> 'TURBO'
; Loop demo
; 
; Input : -
; Output: Demo game until demoloop counter=255
;         or space pressed
;----------------------------------------------

message_loop
	LDY #$FF		; Reset 10-digit
	LDA #0			; Reset demoloop counter
	STA temp4
	SEC
	LDX curwave		; Get wavenr
	INX
	TXA			; Set wavenr
sub10
	INY			; Determine 10-digit
	CMP #10
	BCC save_wave_text
	SBC #10
	BCS sub10
save_wave_text
	STY wave_text		; Set 10-digit wavenr
	STA wave_text+1		; Set 1-digit wavenr

	LDY #6			; Print 'ENTERING WAVE xx'
	JSR prnstr

	CLC			; First call
	JSR completion_message	; Print message at wavenr 16,32,48
wait_loop
	JSR escape		; If ESC pressed end demo

	LDA myst		; If all lives gone, end demo
	BMI esc_exit

	JSR move_my_base	; Move myship
	JSR movestars		; Move stars
	JSR rand		; Get random nr
	JSR process_my_bombs	; Process bombs myship
	JSR pause		; Check status 'S' and 'P'

	LDX #8			; Delay
	JSR delay2
;	jsr wait_sync

	DEC temp4
	BNE wait_loop		; If demoloop counter<>255 -> loop
esc_exit
	SEC			; Not first run
	JSR completion_message	; Erase message at wavenr 16,32,48
	LDY #6			; Erase 'ENTERING WAVE xx'
	JMP prnstr

;---------------------------------------
;end_message:
;
; Display title and credits text
;
; Input : -
; Output: Print title + credit text
;---------------------------------------

end_message
	JSR title		; Print title

	LDY #14			; Print credits
	JSR prnstr
	LDY #10
	JSR prnstr
	LDY #12
	JSR prnstr
	LDY #30
	JSR prnstr
	LDY #20
;	LDY #38
	JMP prnstr

;---------------------------------------
;escape:
;
; Check if fire (demomode) or ESC (gamemode) pressed
; If pressed -> blow myship + aliens
;
; Input : -
; Output: If fire (demomode) or ESC (gamemode) pressed
;         blow myship + aliens
;---------------------------------------

escape
	LDA demo_flag
	BPL not_demomode	; Check if demomode

	JSR chk_spc_fire	; Check if space or fire pressed
	BEQ test_if_already_dead; If pressed, check if lives left
	RTS
not_demomode
	LDX #$3b		; Test ESC key
	JSR check_key
	BNE no_escape
test_if_already_dead
	LDA myst		; Get my status
	BMI no_escape		; Check if lives left

	JSR liveson		; Erase ships for every live
	LDA #0			; Reset lives
	STA lives
	JMP crash		; Blow aliens
no_escape
	RTS

;---------------------------------------
;game_over_loop:
;
; If not demoloop start tune and
; print 'GAME OVER'
; Loop this until music is ready
;
; Input : -
; Output: - 
;---------------------------------------

game_over_loop
	BIT demo_flag
	BMI no_escape		; Check if demomode

	LDY #8			; Print 'GAME OVER'
	JSR prnstr

	LDY #4			; Play game over tune
	JSR StartTune
	lda #$80		; Temp delay has to be music
	sta temp4

game_over2
	JSR movestars		; Move stars

	JSR pause		; Wait a while
	LDX #25
	JSR delay2

;	JSR MusicTest
	dec temp4
	BNE game_over2		; Loop if music not ready

	LDY #8			; Erase 'GAME OVER'
	JMP prnstr

;---------------------------------------
;chk_spc_fire:
;
; Check if space or fire is pressed
;
; Input : -
; Output: A=0 Space/fire pressed
;---------------------------------------

chk_spc_fire
	LDX #$00	; Check for SPACE
	JSR check_key
	BEQ spc_pressed

; Insert code to test if joystick fire is pressed !!!!!!

spc_pressed
	RTS

;---------------------------------------
;completion_message:
;
; Print message at wavenr 16,32,48
; Wavenr 16 -> 'SATURN'
;        32 -> 'BATTERY'
;        48 -> 'TURBO'
;
; Input : C=0 first call, C=1 not first call
; Output: Print message at wavenr 16,32,48
;---------------------------------------

completion_message
	PHP			; Save processor status

	LDA curwave		; Get wavenr
	TAX			; Save wavenr
	AND #$F
	BNE exit_compl		; Check if multiple of 16

	TXA			; Load wavenr
	LSR A
	LSR A
	LSR A
	BEQ exit_compl		; If wavenr=0, end

	BIT sixteen_flag	; Check if first call
	BMI exit_compl		; If not, end

	CLC			; Wavenr=16 -> Y=22
	ADC #20			; Wavenr=32 -> Y=24
	TAY			; Wavenr=48 -> Y=26
	CPY #28
	BCS exit_compl		; If wavenr>=64 end

	PLP			; Restore processor status
	BCC not_second_call	; If C=0, first call

	LDA #$FF		; Set second call flag
	STA sixteen_flag
not_second_call
	JMP prnstr		; Print message
exit_compl
	PLP			; Restore processor status
	RTS

