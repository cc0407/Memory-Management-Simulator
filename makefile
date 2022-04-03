all: main

main: main.c
	gcc -Wpedantic -std=gnu99 main.c -g -o hole

clean:
	rm hole