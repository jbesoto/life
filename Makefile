CC=gcc
CFLAGS=-Wall -Wextra -g
TARGET=life
OBJS=life.o utils.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

life.o: src/life.c src/life.h
	$(CC) $(CFLAGS) -c src/life.c

utils.o: src/utils.c src/utils.h
	$(CC) $(CFLAGS) -c src/utils.c

clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all clean