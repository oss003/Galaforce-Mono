

;----------------------------------------------
;GALAFORCE
; Original version for the BBC by Kevin Edwards
; Atom conversion by Kees van Oss
; 2011
;----------------------------------------------
asm_code         equ $3000

;********************************************************************
; Header for Atom emulator Wouter Ras

             org asm_code-22
name_start
		 .db "GALAFORCE"                  ; 16 bytes filename
name_end
             .blkb (16-name_end+name_start),0
		 .dw asm_code                     ; 2 bytes startaddress
		 .dw exec                         ; 2 bytes linkaddress
		 .dw eind_asm-start_asm           ; 2 bytes filelength

;********************************************************************

start_asm          
	include CONST.inc
	include ZPWORK.inc
	include ABSWORK.inc
	include SPRITES.inc
	include INIT.inc
	include ALIENS1.inc
	include ALIENS2.inc
	include ALIENS3.inc
	include ALIENS4.inc
	include ROUT1.inc
	include ROUT2.inc
	include ROUT3.inc
	include ROUT4.inc
	include STARS.inc
	include BOMBS1.inc
	include BOMBS2.inc
	include CHARP.inc
	include FLAGS.inc
	include MUSIC1.inc
	include MUSIC2.inc
;	include MUSIC3.inc
	include TITLE.inc
	include HIGH.inc
	include WAVE.inc
	include PATT.inc
	include PATDAT.inc
	include VECTORS.inc
	
	include sound.inc
	include joystick.inc
	include ATOMOS.inc
	include graph.inc

	org ((*/256)+1)*256
	include digits.inc
	include spfont.inc

eind_asm
