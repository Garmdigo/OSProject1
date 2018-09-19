#cop 4610 project1 makefile




CC=gcc

shellmain: shell_main.c
	gcc -std=c99 shell_main.c parser_main.c shell_functions.c
	./a.out

clean:
	rm a.out

