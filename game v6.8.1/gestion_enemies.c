#include "fichier_h/gestion_enemies.h"

#include "fichier_h/gestion_tourelles.h"
#include "fichier_h/affichages_menus_widgets.h"
#include "fichier_h/generation.h"
#include "fichier_h/constantes_structures.h"




void move(SDL_Renderer *renderer, int side, int tile_size, char ** map,
          int* vie, Enemy* enemies, int num, int* argent) {
    if (!enemies[num].isalive) return;

    if (enemies[num].life <= 0) {
        enemies[num].isalive = false;
        *argent += 1;
        return;
    }

    if (enemies[num].x < 0 || enemies[num].x >= side || enemies[num].y >= side) return;

    Uint32 now = SDL_GetTicks();
    if (now < enemies[num].spawn_time) return;

    int x = enemies[num].x;
    int y = enemies[num].y;

    paste_image(renderer, "assets/crabe.png", x * tile_size, y * tile_size, tile_size);

    if (now - enemies[num].last_move_time > 280&&rand()%7!=0) {
        int moved = 0;

        int dx[] = {  0, -1,  1, -1,  1,  0, -1,  1 };
        int dy[] = {  1,  1,  1,  0,  0, -1, -1, -1 };

        for (int dir = 0; dir < 8; dir++) {
            int nx = x + dx[dir];
            int ny = y + dy[dir];

            if (
                nx >= 0 && nx < side &&
                ny >= 0 && ny < side &&
                map[nx][ny] == ' ' &&
                !(nx == enemies[num].l_p[0][0] && ny == enemies[num].l_p[0][1]) &&
                !(nx == enemies[num].l_p[1][0] && ny == enemies[num].l_p[1][1])
            ) {
                // Effectuer le mouvement
                enemies[num].x = nx;
                enemies[num].y = ny;

                // Mettre à jour l'historique (après déplacement)
                enemies[num].l_p[1][0] = enemies[num].l_p[0][0];
                enemies[num].l_p[1][1] = enemies[num].l_p[0][1];

                enemies[num].l_p[0][0] = x; // ancienne position
                enemies[num].l_p[0][1] = y;

                moved = 1;
                break;
            }
        }

        if (!moved) {
            printf("Enemy %d blocked at (%d, %d)\n", num, x, y);
        }

        if (enemies[num].y == side - 1) {
            (*vie)--;
            enemies[num].life = 0;
            printf(">>> Enemy %d reached the end! Player life: %d\n", num, *vie);
        }

        enemies[num].last_move_time = now;
        printf("%d",now);
    }
}



void init_wave(Enemy *enemies, int count, int wave, int start_x, Uint32 current_time) {
    for (int i = 0; i < count; i++) {
        enemies[i] = (Enemy){
            .x = start_x, .y = 0,
            .life = 1 + wave / 2 ,
            .speed = 1,
            .last_move_time = 0,
            .spawn_time = current_time + i * 1000+rand()%1000,
            .isalive=true,
            .l_p = {{0,0}}
        };
    }
}

int all_enemies_dead(Enemy *enemies, int count) {
    for (int i = 0; i < count; i++)
        if (enemies[i].life > 0) return 0;
    return 1;
}

void move_all_enemies(SDL_Renderer *renderer,int side,int tile_size,  char ** map, int* vie, Enemy* enemies, int count, int* argent) {
	for (int i = 0; i < count; i++) {
		move(renderer,side, tile_size, map, vie, enemies, i, argent);
	}
}

