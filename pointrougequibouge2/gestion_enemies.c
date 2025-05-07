  #include "fichier_h/gestion_enemies.h"

#include "fichier_h/gestion_tourelles.h"
#include "fichier_h/affichages_menus_widgets.h"
#include "fichier_h/generation.h"
#include "fichier_h/constantes_structures.h"



void deplacement_ennemi(SDL_Renderer *renderer, int map[MAP_SIZE][MAP_SIZE], int* vie, enemy* enemies, int num, tower* towers, int num_towers, int* argent) {
    // Vérification si l'ennemi est déjà mort
    if (enemies[num].life <= 0 || enemies[num].x < 0 || enemies[num].x >= MAP_SIZE || enemies[num].y >= MAP_SIZE){

    printf("Ennemi %d inactif ou hors carte. Aucun déplacement.\n", num);
    
    return;
}
    Uint32 now = SDL_GetTicks();
    if (now < enemies[num].spawn_time) {
        // Pas encore le moment de spawn
        return;
    }



    int x = enemies[num].x;
    int y = enemies[num].y;

    // Affichage sprite de l'ennemi
    if (y >= 0 && y < MAP_SIZE)
    affiche_image(renderer, "assets/crabe.png", x * TILE_SIZE, y * TILE_SIZE);
    

    if (now - enemies[num].last_move_time > 200) {
        printf("Déplacement de l'ennemi %d - Position actuelle : (%d, %d)\n", num, x, y);

        int moved = 0;

        // Priorité au bas
        if (y < MAP_SIZE - 1 && map[x][y + 1] == 1) {
            enemies[num].y++;
            moved = 1;
            printf("Ennemi %d descend en (%d, %d)\n", num, x, y + 1);
        }
        // Diagonale droite-bas
        else if (y < MAP_SIZE - 1 && x < MAP_SIZE - 1 && map[x + 1][y + 1] == 1) {
            enemies[num].x++; enemies[num].y++;
            moved = 1;
            printf("Ennemi %d descend à droite en (%d, %d)\n", num, x + 1, y + 1);
        }
        // Diagonale gauche-bas
        else if (y < MAP_SIZE - 1 && x > 0 && map[x - 1][y + 1] == 1) {
            enemies[num].x--; enemies[num].y++;
            moved = 1;
            printf("Ennemi %d descend à gauche en (%d, %d)\n", num, x - 1, y + 1);
        }
        // Gauche
        else if (x > 0 && map[x - 1][y] == 1) {
            enemies[num].x--;
            moved = 1;
            printf("Ennemi %d va à gauche en (%d, %d)\n", num, x - 1, y);
        }
        // Droite
        else if (x < MAP_SIZE - 1 && map[x + 1][y] == 1) {
            enemies[num].x++;
            moved = 1;
            printf("Ennemi %d va à droite en (%d, %d)\n", num, x + 1, y);
        }

        if (!moved) {
            printf("Ennemi %d bloqué en (%d, %d). Aucun mouvement possible.\n", num, x, y);
        }

        // Si l'ennemi atteint la fin
        if (enemies[num].y >= MAP_SIZE - 1) {
            (*vie)--;
            enemies[num].life = 0; // On le retire du jeu
            printf(">>> Ennemi %d a atteint la fin ! Vie restante du joueur : %d\n", num, *vie);
        }

        enemies[num].last_move_time = now;
    }
}

void initialiser_vague(enemy *enemies, int count, int wave, int start_x, Uint32 current_time) {
    for (int i = 0; i < count; i++) {
        enemies[i] = (enemy){
            .x = start_x, .y = 0,
            .life = 1 + wave / 2 ,
            .speed = 1,
            .last_move_time = 0,
            .spawn_time = current_time + i * 1000
        };
    }
}

int tous_les_ennemis_morts(enemy *enemies, int count) {
    for (int i = 0; i < count; i++)
        if (enemies[i].life > 0) return 0;
    return 1;
}
