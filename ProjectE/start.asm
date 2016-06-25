;start.asm
;Ivan Deras, 2015

.extern _main

entry start
start:
	call _main
loop1: jmp loop1

