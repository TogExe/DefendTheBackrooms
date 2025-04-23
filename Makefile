
OBJS	= main.o generator.o
SOURCE	= main.c generator.c
HEADER	= generator.h
OUT	= a.out
FLAGS	= -g -c -Wall
LFLAGS	= 
CC	= gcc

all:	a.out

a.out: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

main.o: main.c
	$(CC) $(FLAGS) main.c 

generator.o: generator.c
	$(CC) $(FLAGS) generator.c 

clean:
	rm -f $(OBJS) $(OUT)

test: my_program
	./my_program
