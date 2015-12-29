;----------------------------------------------
;ABSWORK $400
;----------------------------------------------

alst		equ $400
algra		equ alst+maxaliens
alpatlow	equ algra+maxaliens
alpathigh	equ alpatlow+maxaliens
alpatoff	equ alpathigh+maxaliens
alcount 	equ alpatoff+maxaliens
almult		equ alcount+maxaliens
aldirect	equ almult+maxaliens
alx		equ aldirect+maxaliens
aly		equ alx+maxaliens
al_loop_count	equ aly+maxaliens
al_loop_start	equ al_loop_count+maxaliens
alpatreflect	equ al_loop_start+maxaliens

mybullx		equ alpatreflect+maxaliens
mybully		equ mybullx+mymaxbull
mybullst	equ mybully+mymaxbull

albullx		equ mybullst+mymaxbull
albully		equ albullx+almaxbull
albullst	equ albully+almaxbull
albullspeed	equ albullst+almaxbull

lives		equ albullspeed+almaxbull
myx		equ lives+1
myy		equ myx+1
myst 		equ myy+1
score_base      equ myst+1
myscore		equ score_base
hiscore		equ myscore+7

wavoff 		equ hiscore+7
almove 		equ wavoff+1
curwave 	equ almove+1
procst 		equ curwave+1
mybullact 	equ procst+1
albullact 	equ mybullact+1
initact 	equ albullact+1
bombdel 	equ initact+1
dem_section 	equ bombdel+1

sound_flag 	equ dem_section+1
key_joy_flag 	equ sound_flag+1
demo_flag 	equ key_joy_flag+1
demo_count 	equ demo_flag+1
demo_direction 	equ demo_count+1
counter_sound 	equ demo_direction+1
extra_life_flag equ counter_sound+1
sixteen_flag 	equ extra_life_flag+1
pause_flag 	equ sixteen_flag+1

hstxt		equ pause_flag+1
hsnum		equ hstxt+8*10
e_n_d		equ hsnum+8*7
