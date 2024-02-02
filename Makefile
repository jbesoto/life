all: life

life: life.c
	gcc -Wall -Wextra life.c -o life

clean:
	rm -f life