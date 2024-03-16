CC=gcc
CFLAGS=-Wall -Wextra -g
TARGET=life
OBJS=life.o utils.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)
	rm -f $(OBJS)

life.o: life.c life.h
	$(CC) $(CFLAGS) -c life.c

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c

clean:
	rm -f $(TARGET)

.PHONY: all clean