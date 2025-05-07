# Détail des Fonctions `generation_map` et `draw_map` avec leur Utilisation dans `main`

## Fonction `generation_map`

```c
void generation_map(int map[MAP_SIZE][MAP_SIZE]) {
    int x = rand() % MAP_SIZE; 

    for (int y = 0; y < MAP_SIZE; y++) {
        map[x][y] = 1; // Marquer le chemin

        // Choisir direction aléatoire (gauche, droite ou tout droit)
        int direction = rand() % 3; // 0 = gauche, 1 = tout droit, 2 = droite

        if (direction == 0 && x > 0) {
            x--; // Aller à gauche
        } else if (direction == 2 && x < MAP_SIZE - 1) {
            x++; // Aller à droite
        }
        // Sinon continuer tout droit
    }
}
```

### Rôle

Cette fonction crée un chemin aléatoire de haut en bas sur une grille carrée `map` de taille `MAP_SIZE x MAP_SIZE`. Le chemin est représenté par des `1`, les autres cases sont du sol (`0`). ATTENTION : la taille dépend après de la taille des tuiles (multiplication), exemples : #define MAP_SIZE 20
#define TILE_SIZE 30, co

### Fonctionnement

- La position `x` initiale est choisie aléatoirement sur la première colonne (`y=0`).
- À chaque ligne `y`, la case `(x, y)` est marquée comme chemin (`1`).
- Une direction est choisie aléatoirement :
  - 0 → aller à gauche (si possible),
  - 1 → continuer tout droit,
  - 2 → aller à droite (si possible).

## Fonction `draw_map`

```c
void draw_map(SDL_Renderer *renderer, int map[MAP_SIZE][MAP_SIZE]) {
    for (int x = 0; x < MAP_SIZE; x++) {
        for (int y = 0; y < MAP_SIZE; y++) {
            if (map[x][y] == 0) {
                SDL_SetRenderDrawColor(renderer, 34, 139, 34, 255); // Vert (sol)
            } else {
                SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255); // Marron (chemin)
            }

            // Dernière ligne du chemin : couleur blanche
            if (y == MAP_SIZE - 1 && map[x][y] == 1) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            }

            SDL_Rect rect = {x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}
```

### Rôle

Cette fonction dessine la carte à l’écran selon les valeurs de `map` :
- `0` → sol vert,
- `1` → chemin marron,
- Dernière ligne du chemin (`y == MAP_SIZE - 1`) → blanc pour la destination.

## Utilisation dans `main`

### Initialisation de la carte

```c
for (int x = 0; x < MAP_SIZE; x++) {
    for (int y = 0; y < MAP_SIZE; y++) {
        map[x][y] = 0; // tout est du sol au départ
    }
}
generation_map(map); // crée un chemin dans la carte
```

### Dessin dans la boucle de rendu

```c
draw_map(renderer, map);
```

Cela permet de rendre à l'écran la grille avec le chemin visible.

---

Ces fonctions sont fondamentales pour la génération et l’affichage dynamique du terrain dans un jeu Tower Defense en SDL2.
