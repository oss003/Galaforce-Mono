;----------------------------------------------
;INIT
;----------------------------------------------

;----------------------------------------------
;exec:
;
; Program entry
;
; Input : -
; Output: -
;----------------------------------------------

exec
	LDY #$E				; Clear myscore + hiscore
	LDA #0
setup_hiscore
	STA myscore-1,Y
	DEY
	BNE setup_hiscore

	LDA #3				; Set HiScore=0030000
	STA hiscore+2

;--- Frame timer ---

	lda #$40               ; Timer 1 continuous, Timer 2 one shot
	sta $b80b

	jsr joyinit			; Initialise joystick

;--------------------

	jsr clear4a			; Clear screen

	JSR seed_rnd			; Init random number

	JSR starinit			; Init stars

	LDY #2				; Print SCR
	JSR prnstr

	LDY #4				; Print HI
	JSR prnstr

	LDA #$80			; Set flags
	STA sound_flag
	STA pause_flag
	STA key_joy_flag

	JSR display_sound_status	; Display S/Q
	JSR display_key_joy_status	; Display J/K
	JSR init_score			; Reset and print scores
	JSR wait_for_space		; Loop screens until key/fire press

	TAY				; Wavebase in A

;----------------------------------------------
;restart:
;
; Infinite 'game' loop entry point
;
; Input : Y = wavenr
; Output: -
;----------------------------------------------

restart
	LDA #3				; Set vars
	STA lives
	STA sixteen_flag
	STA extra_life_flag		; +ve allows it to be printed

	STY curwave			; Store wave nr

	LDA vecwavl,Y			; Set current wave baseaddress
	STA wavbase
	LDA vecwavh,Y
	STA wavbase+1

	JSR flagson			; Draw flags with wavenr

	LDA demo_flag			; Check if in demomode ($80)
	BMI no_st_tune			; No starttune in demomode

	LDY #0				; Reset music pointer
	JSR StartTune			; Play starttune
no_st_tune
	LDA #0				; Reset wave offsetpointer
	STA wavoff

	LDY #process
	STY aliens			; Set initial aliens
	DEY
	STY aliensm1			; Set initial aliens-1

	JSR init_score			; Reset and print scores
	JMP first_life			; Start game

;----------------------------------------------
;next_life:
;
; Entry point if died and lives are not 0
;
; Input : -
; Output: -
;----------------------------------------------

next_life
	JSR liveson			; Remove lives left

	LDY #0				; Set nr of aliens of first wave
	LDA (wavbase),Y
	STA aliens

	STA aliensm1			; Set nr of aliens-1
	dec aliensm1
	DEC wavoff
	DEC lives			; Dec lives counter
	BNE first_life			; If not 0, start game

	LDX #<lstman			; If lives=0, play tune
	LDY #>lstman
	JSR mksound

;----------------------------------------------
;first_life:
;
; Entry point first start or next live
;
; Input : -
; Output: -
;----------------------------------------------

first_life
	JSR liveson			; Draw lives left
	JSR rstall			; Reset gamevars

	JSR pokmypos			; Calculate screenaddress ship

	lda #$c0			; Make shure ship if plot
	sta screen2+1

	LDA graph+36			; Set shipgraphic address
	TAX
	LDY graph+37
	STY temp1			; Save hb shipgraphic address
	JSR sprite

	JSR message_loop		; Plot 'Entering zone nr'

main_loop
	INC counter			; Inc loop counter
	JSR escape			; Check if ESC or fire is pressed
	JSR rand			; Get random nr
	JSR move_my_base		; Check controls to move ship
	JSR movestars			; Move stars
	JSR init_new_aliens		; If possible, init new alien
	JSR move_the_aliens		; Move all aliens
	JSR process_my_bombs		; Move my bombs
	JSR process_aliens_bombs	; Move alien bombs
	JSR collision			; Check if there are any collisions

	LDA myst			; If ship not hit, loop
	BPL main_loop

	LDA almove			; Check alien movement
	BPL main_loop			; If no move, loop

	LDA albullact			; Check if myship hit
	ORA mybullact
	BNE main_loop			; If not, loop

	JSR die_loop			; If myship hit, die

	LDA lives			; Get lives left
	BNE next_life			; If not 0, next life

	JSR check_new_high		; If lives=0, check if highscore

	LDA #>highscore_pos
	STA screen+1
	JSR poke_hi_scr			; 'Print highscore
 
	JSR game_over_loop		; Print 'GAME OVER' and play tune

	JSR ahigh			; If highscore, input name

	JSR wait_for_space		; Loop screens until key is pressed

	PHA				; Save wavenr
	JSR flagson			; Erase flags
	PLA				; Get wavenr

	TAY				; Set Y=wavenr

	JMP restart			; Start new game


;----------------------------------------
;wait_for_space:
;
; Display text and loop screen until key is pressed
;
; Input : -
; Output: A=wavenr, (0=space pressed,random=demomode)
;----------------------------------------

wait_for_space
	JSR end_message			; Display end message text

	LDA #0				; Reset counter
	STA counter
space_loop
	JSR srlp			; Move stars
	INC counter	
	BEQ hsclp			; If nothing pressed, display score
	JSR chk_spc_fire		; Check if space or fire is pressed
	BNE space_loop

	PHP				; If space/fire pressed, save carry
	JSR end_message			; Remove end message text
jkch
	JSR display_key_joy_status	; Remove key/joy status
	PLP				; Restore carry
	LDA #0
	ADC #19
	STA key_joy_letter		; C=0 -> J(19), C=1 -> K(20)
	LDX #$FF
	CMP #20
	BEQ set_kbd_negative
	INX
set_kbd_negative
	TXA
	JSR key_joy2			; A=$FF Key, A=0 Joy 

	LDA #0				; Reset demoflag
	STA demo_flag			; Exit with wavenr=0

	RTS

;----------------------------------------
; Print highscore table and check if space pressed
;----------------------------------------

hsclp
	JSR end_message			; Remove end message text
	JSR pht				; Display Highscore table
hscl2
	JSR srlp			; Move stars
	INC counter
	BEQ into_demo			; If nothing pressed, go to demo
	JSR chk_spc_fire		; Check if space or fire is pressed
	BNE hscl2

	PHP
	JSR pht				; Remove Highscore table
	JMP jkch			; Display key/joy status

into_demo
	JSR pht				; Remove Highscore table

	LDX #$FF			; Reset demo vars
	STX demo_flag
	STX demo_count
	INX
	STX dem_section

	JSR rand
	AND #7				; Random wavenr for demo
	RTS

;----------------------------------------
;Move stars with delay
;----------------------------------------

srlp
	LDX #15				; Wait a while
	JSR delay2

	JSR movestars			; Move stars
	JMP pause			; Check S or P key pressed and exit

;----------------------------------------
;process_demo:
;
; If in demomode, set random demo direction
;
; Input : -
; Output: -
;----------------------------------------

process_demo
	LDA demo_flag			; Check if in demomode
	BPL not_in_demo			; Exit if not

	DEC demo_count			; Dec demo counter
	BPL not_in_demo

	LDA rand1+1			; Set random direction
	STA demo_direction
	AND #15
	ADC #10
	STA demo_count
not_in_demo
	RTS

;----------------------------------------
;dem_sound:
;
; Interrupt sound routine
; Not used in Atom version!!!!!!!!!!!
;----------------------------------------

dem_sound
	CMP #4
	BNE ex_dem2
	BIT pause_flag
	BPL ex_dem2
 	TXA
	PHA
	TYA
	PHA
	JSR Refresh
	JSR MusicTest
	BNE ex_dem
	DEC dem_section
 	BPL not_new_tune
	LDA #7
	STA dem_section
not_new_tune
	LDX dem_section
	LDY dem_table,X
	LDA demo_flag
	BPL ex_dem
	JSR StartTune
	JSR Refresh
ex_dem
	PLA
	TAY
	PLA
	TAX
ex_dem2
	RTS

; Backwards order for demo
; tunes ie 21,21,42,42,..

dem_table
	.db 56,56,49,49
	.db 42,42,21,21

;----------------------------------------------
;die_loop:
;
; If in demomode, exit. 
; If in gamemode, check S and P keys and play tune
;
; Input : -
; Output: -
;----------------------------------------------

die_loop
	BIT demo_flag				; Check if in demomode
	BMI dl1					; If so, exit

	LDY #2					; If died, play tune
	JSR StartTune
dl2
 	JSR movestars				; Move stars
	JSR pause				; Check if S or P key is pressed
 
	LDX #22					; Wait a while
	JSR delay2

	JSR MusicTest				; Check if tune ended
	BNE dl2					; If not, loop
dl1
	RTS					; Exit 

;----------------------------------------------
; Last man tune
;----------------------------------------------

lstman
	.dw $13
	.dw 9
	.dw 160
	.dw 50


