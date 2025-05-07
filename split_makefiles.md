# gui
CC = gcc
CFLAGS = -Wall -g `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs` -lSDL2_ttf -lSDL2_mixer
OBJS = main.o gui.o

all: gui_demo

gui_demo: $(OBJS)
	$(CC) -o gui_demo $(OBJS) $(LDFLAGS)

main.o: main.c gui.h
	$(CC) $(CFLAGS) -c main.c

gui.o: gui.c gui.h
	$(CC) $(CFLAGS) -c gui.c

clean:
	rm -f *.o gui_demo

# generation
 # Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -std=c99 `sdl2-config --cflags`

# Linker flags for SDL2
LDFLAGS = `sdl2-config --libs`

# Source files
SRCS = constraintBuilder.c generator.c main.c pathfinder.c heightmap.c

# Object files
OBJS = $(SRCS:.c=.o)

# Header files
HDRS = constraintBuilder.h generator.h pathfinder.h heightmap.h

# Executable name
TARGET = my_program

# Default target
all: $(TARGET)

# Linking the object files to create the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

# Rule to create object files from source files
%.o: %.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean
