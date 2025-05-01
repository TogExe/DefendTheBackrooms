# Nom de l'exécutable
EXEC = testsdl

# Fichiers source
SRC = testsdl.c

# Compilation
CC = gcc

# Répertoires d'inclusion possibles pour les headers SDL2
INCLUDE_DIRS = -I/usr/include/SDL2 -I/usr/local/include/SDL2

# Répertoires de bibliothèques possibles
LIB_DIRS = -L/usr/lib -L/usr/local/lib

# Flags de compilation
CFLAGS = -Wall -Wextra -O2 $(INCLUDE_DIRS) $(shell sdl2-config --cflags)

# Flags d'édition de liens
LDFLAGS = $(LIB_DIRS) $(shell sdl2-config --libs) -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm

# Règle de compilation principale
all: $(EXEC)

$(EXEC): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Nettoyage
clean:
	rm -f $(EXEC)

# Nettoyage complet
mrproper: clean

