@ECHO OFF
ed %1.asm
x6502 %1.asm %2 >report.txt
type report.txt
pause
link02 -c %1.obj -x
if exist %1.atm erase %1.atm
if exist %1.tsk rename %1.tsk %1.atm
pause
