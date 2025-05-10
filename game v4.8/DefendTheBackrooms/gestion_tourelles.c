

#include "fichier_h/gestion_tourelles.h"

#include "fichier_h/gestion_enemies.h"
#include "fichier_h/affichages_menus_widgets.h"
#include "fichier_h/generation.h"
#include "fichier_h/constantes_structures.h"


// Renvoie 1 si une position a été trouvée et placée dans out_x/out_y
int draw_click_zone(SDL_Renderer *renderer, int mouse_x, int mouse_y,int side, char ** map, int tile_size,
                    int *out_x, int *out_y, int click_positions[MAX_CLICKS][2], int click_count, int argent){
    (void)renderer;
    
    int tile_x = mouse_x / tile_size;
    int tile_y = mouse_y / tile_size;
    
    
    
    if (argent < 10){ //plus tard : towern.cost typed tower d'indice n pour savoir son coût)
      return 0;
    }
  	if (tile_x < 0 || tile_x >= side || tile_y < 0 || tile_y >= side) {
	
       	return 0;
   
    }
    for(int i=0;i<side;i++){
    	printf("\n");
    	for(int j=0;j<side;j++){
    		printf("%c",map[i][j]);
		}
    }
    
    printf("\n === [%c] === \n",map[tile_x][tile_y]);
    
    //if ((map[tile_x][tile_y] >= 'a' || map[tile_x][tile_y] <= 'z') && map[tile_x][tile_y] != ' ') {
    if (map[tile_x][tile_y]!='y') {
    	printf("invalid\n");
        return 0;
    }
    

    int best_x = -1, best_y = -1;
    double min_dist = 1e9;

    for (int radius = 1; radius <= 3; radius++) {
        for (int dx = -radius; dx <= radius; dx++) {
            for (int dy = -radius; dy <= radius; dy++) {
                int nx = tile_x + dx;
                int ny = tile_y + dy;

                if (nx >= 0 && nx < side && ny >= 0 && ny < side && map[nx][ny] != ' ') {
                    // Vérifier qu'aucune tourelle n'est déjà placée ici
                    int pixel_x = nx * tile_size;
                    int pixel_y = ny * tile_size;
                    int already_placed = 0;
                    for (int i = 0; i < click_count; i++) {
                        int cx = click_positions[i][0] / tile_size * tile_size;
                        int cy = click_positions[i][1] / tile_size * tile_size;
                        if (cx == pixel_x && cy == pixel_y) {
                            already_placed = 1;
                            break;
                        }
                    }

                    if (!already_placed) {
                        int px = nx * tile_size + tile_size / 2;
                        int py = ny * tile_size + tile_size / 2;
                        double dist = sqrt((mouse_x - px) * (mouse_x - px) + (mouse_y - py) * (mouse_y - py));
                        if (dist < min_dist) {
                            min_dist = dist;
                            best_x = nx;
                            best_y = ny;
                        }
                    }
                }
            }
        }
    }

    if (best_x != -1 && best_y != -1) {
        *out_x = best_x * tile_size;
        *out_y = best_y * tile_size;
        return 1;
    }

    return 0;
}

void turret(Tower* towers, int tower_count, Enemy* enemies, int enemy_count, Mix_Chunk *tir_son, int tile_size) {
    for (int i = 0; i < tower_count; i++) {
        if (towers[i].active_shoot > 0) {
            towers[i].active_shoot--;
        }

        if (towers[i].cooldown > 0) {
            towers[i].cooldown--;
            continue;
        }

        int tower_cx = towers[i].x + tile_size / 2;
        int tower_cy = towers[i].y + tile_size / 2;

        for (int j = 0; j < enemy_count; j++) {
            if (enemies[j].life <= 0)
                continue;
			
            int enemy_cx = enemies[j].x * tile_size + tile_size / 2;
            int enemy_cy = enemies[j].y * tile_size + tile_size / 2;

            int dx = enemy_cx - tower_cx;
            int dy = enemy_cy - tower_cy;
            int distance_squared = dx * dx + dy * dy;

            if (distance_squared <= towers[i].range * towers[i].range) {
                enemies[j].life -= towers[i].damage;
                towers[i].cooldown = towers[i].speed_damage;
                towers[i].active_shoot = 10;
                printf("Tour %d (Niveau %d) a tiré sur ennemi %d ! Vie ennemie = %d\n", i, towers[i].level , j, enemies[j].life);
                Mix_PlayChannel(-1, tir_son, 0);
                break;  // Une tour tire sur un seul ennemi à la fois
            }
        }
    }
}

void upgrade(Tower* towers, int tower_count, int mouse_x, int mouse_y, int tile_size, int*money) {
    for (int i = 0; i < tower_count; i++) {
        int tx = towers[i].x;
        int ty = towers[i].y;

        SDL_Rect rect = { tx, ty, tile_size, tile_size };
        if (mouse_x >= rect.x && mouse_x <= rect.x + tile_size &&
            mouse_y >= rect.y && mouse_y <= rect.y + tile_size) {

            towers[i].damage += 1;
            towers[i].level += 1;
            printf("Tour %d niveau %d\n", i, towers[i].level);
            *money-=8;
            return;
        }
    }
}
