all: hole

hole: main.c
	gcc -Wpedantic -std=gnu99 main.c -g -o hole

clean:
	rm hole