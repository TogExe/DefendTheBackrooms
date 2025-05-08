#include "fichier_h/gestion_enemies.h"

#include "fichier_h/gestion_tourelles.h"
#include "fichier_h/affichages_menus_widgets.h"
#include "fichier_h/generation.h"
#include "fichier_h/constantes_structures.h"



void move(SDL_Renderer *renderer, int side, int tile_size, char map[side][side], int* vie, Enemy* enemies, int num, int* argent) {
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

    // Draw enemy
    paste_image(renderer, "assets/crabe.png", x * tile_size, y * tile_size, tile_size);

    if (now - enemies[num].last_move_time > 200) {
        int moved = 0;
		//""
        // Direction vectors for 8 directions
        //int dx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
        //int dy[] = {0, 1, 1, 1, 0, -1, -1, -1};
		int dx[] = { 1, 0, -1,  1,-1, 1, 0, -1};
        int dy[] = {  1, 1,  1, 0, 0,-1,-1, -1};
		
        for (int dir = 0; dir < 8; dir++) {
			
            int nx = x + dx[dir];
            int ny = y + dy[dir];
            
            if (nx >= 0 && nx < side && ny >= 0 && ny < side &&
                map[nx][ny] == ' ' &&
                !(nx == enemies[num].l_p[0] && ny == enemies[num].l_p[1])) {
                enemies[num].l_p[0] = x;
                enemies[num].l_p[1] = y;
                enemies[num].x = nx;
                enemies[num].y = ny;
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

    }
}

void init_wave(Enemy *enemies, int count, int wave, int start_x, Uint32 current_time) {
    for (int i = 0; i < count; i++) {
        enemies[i] = (Enemy){
            .x = start_x, .y = 0,
            .life = 1 + wave / 2 ,
            .speed = 1,
            .last_move_time = 0,
            .spawn_time = current_time + i * 1500,
            .isalive=true,
            .l_p = {0,0}
        };
    }
}

int all_enemies_dead(Enemy *enemies, int count) {
    for (int i = 0; i < count; i++)
        if (enemies[i].life > 0) return 0;
    return 1;
}

void move_all_enemies(SDL_Renderer *renderer,int side,int tile_size,  char map[side][side], int* vie, Enemy* enemies, int count, int* argent) {
	for (int i = 0; i < count; i++) {
		move(renderer,side, tile_size, map, vie, enemies, i, argent);
	}
}

/*#include "fichier_h/gestion_enemies.h"

#include "fichier_h/gestion_tourelles.h"
#include "fichier_h/affichages_menus_widgets.h"
#include "fichier_h/generation.h"
#include "fichier_h/constantes_structures.h"



void move(SDL_Renderer *renderer,int side,int tile_size,  char map[side][side], int* vie, Enemy* enemies, int num, int* argent) {
    // Vérification si l'ennemi est déjà mort
	
	
    if (!enemies[num].isalive){
    	return;
    }
	if (enemies[num].isalive && enemies[num].life <=0){
		printf("aaaaaaaaaaaaaaaaaaaaaa");
		enemies[num].isalive=false; 
		printf("%d",*argent);
		*argent = *argent +1;
		return;
	}
    
    if (enemies[num].life <= 0 || enemies[num].x < 0 || enemies[num].x >= side || enemies[num].y >= side){

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
    if (y >= 0 && y < side)
    paste_image(renderer, "assets/crabe.png", x * tile_size, y * tile_size,  tile_size);
    

    if (now - enemies[num].last_move_time > 200) {
        printf("Déplacement de l'ennemi %d - Position actuelle : (%d, %d)\n", num, x, y);

        int moved = 0;

        // Priorité au bas
        if (y < side - 1 && map[x][y + 1] == ' ' && (enemies[num].l_p[0]!=x&&enemies[num].l_p[1]!=y+1)){
            enemies[num].l_p[0]=enemies[num].x;
            enemies[num].l_p[1]=enemies[num].y;
			enemies[num].y++;
            moved = 1;
            printf("Ennemi %d descend en (%d, %d)\n", num, x, y + 1);
        }
        // Gauche
        else if (x > 0 && map[x - 1][y] == ' ' && (enemies[num].l_p[0]!=x-1&&enemies[num].l_p[1]!=y)) {
			enemies[num].l_p[0]=enemies[num].x;
			enemies[num].l_p[1]=enemies[num].y;
            enemies[num].x--;
            moved = 1;
            printf("Ennemi %d va à gauche en (%d, %d)\n", num, x - 1, y);
        }
        // Droite
        else if (x < side - 1 && map[x + 1][y] == ' ' && (enemies[num].l_p[0]!=x+1&&enemies[num].l_p[1]!=y)) {
			enemies[num].l_p[0]=enemies[num].x;
			enemies[num].l_p[1]=enemies[num].y;
            enemies[num].x++;
            moved = 1;
            printf("Ennemi %d va à droite en (%d, %d)\n", num, x + 1, y);
        }
        // Diagonale droite-bas
        else if (y < side - 1 && x < side - 1 && map[x + 1][y + 1] == ' ' && (enemies[num].l_p[0]!=x+1&&enemies[num].l_p[1]!=y+1)) {
			enemies[num].l_p[0]=enemies[num].x;
			enemies[num].l_p[1]=enemies[num].y;
            enemies[num].x++; enemies[num].y++;
            moved = 1;
            printf("Ennemi %d descend à droite en (%d, %d)\n", num, x + 1, y + 1);
        }
        // Diagonale gauche-bas
        else if (y < side - 1 && x > 0 && map[x - 1][y + 1] == ' ' && (enemies[num].l_p[0]!=x-1&&enemies[num].l_p[1]!=y+1)) {
			enemies[num].l_p[0]=enemies[num].x;
			enemies[num].l_p[1]=enemies[num].y;
            enemies[num].x--; enemies[num].y++;
            moved = 1;
            printf("Ennemi %d descend à gauche en (%d, %d)\n", num, x - 1, y + 1);
        }

        if (!moved) {
            printf("Ennemi %d bloqué en (%d, %d). Aucun mouvement possible.\n", num, x, y);
        }

        // Si l'ennemi atteint la fin
        if (enemies[num].y >= side - 1) {
            (*vie)--;
            enemies[num].life = 0; // On le retire du jeu
            printf(">>> Ennemi %d a atteint la fin ! Vie restante du joueur : %d\n", num, *vie);
        }

        enemies[num].last_move_time = now;
    }
    
}

void init_wave(Enemy *enemies, int count, int wave, int start_x, Uint32 current_time) {
    for (int i = 0; i < count; i++) {
        enemies[i] = (Enemy){
            .x = start_x, .y = 0,
            .life = 1 + wave / 2 ,
            .speed = 1,
            .last_move_time = 0,
            .spawn_time = current_time + i * 1000,
            .isalive=true,
            .l_p = {0,0}
        };
    }
}

int all_enemies_dead(Enemy *enemies, int count) {
    for (int i = 0; i < count; i++)
        if (enemies[i].life > 0) return 0;
    return 1;
}

void move_all_enemies(SDL_Renderer *renderer,int side,int tile_size,  char map[side][side], int* vie, Enemy* enemies, int count, int* argent) {
	for (int i = 0; i < count; i++) {
		move(renderer,side, tile_size, map, vie, enemies, i, argent);
	}
}*/
