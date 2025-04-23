# Makefile
OBJS    = main.o generator.o
SOURCE  = main.c generator.c
HEADER  = generator.h
OUT     = a.out
CC      = gcc

# Compilation and Linker flags
CFLAGS  = -g -Wall `sdl2-config --cflags`
LFLAGS  = `sdl2-config --libs`

all: $(OUT)

$(OUT): $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

generator.o: generator.c
	$(CC) $(CFLAGS) -c generator.c

clean:
	rm -f $(OBJS) $(OUT)

# Test target
test: $(OUT)
	./$(OUT)
