all: life

life: life.c
	gcc -o life life.c

clean:
	rm -f life
