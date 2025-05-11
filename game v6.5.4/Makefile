# Nom de l'exécutable
TARGET = COCONUT_PARTY

# Fichiers sources
SRCS = main.c generation.c gestion_enemies.c gestion_tourelles.c affichages_menus_widgets.c generator.c constraintBuilder.c pathfinder.c gui.c save.c

# Drapeaux de compilation
CFLAGS = -Wall -g

# Bibliothèques à lier
LIBS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm


# Compilation de l'exécutable
$(TARGET): $(SRCS)
	@gcc $(CFLAGS) $(SRCS) -o $(TARGET) $(LIBS)

# Nettoyage des fichiers objets et de l'exécutable
clean:
	@rm -f $(TARGET)
	
# Fonction pour lancer le programme après la compilation
run: $(TARGET)
	./$(TARGET)


# Règle par défaut
all: $(TARGET)

