
#include "fichier_h/gestion_tourelles.h"

#include "fichier_h/gestion_enemies.h"
#include "fichier_h/affichages_menus_widgets.h"
#include "fichier_h/generation.h"
#include "fichier_h/constantes_structures.h"


// Renvoie 1 si une position a été trouvée et placée dans out_x/out_y
int draw_click_zone(SDL_Renderer *renderer, int mouse_x, int mouse_y, int map[MAP_SIZE][MAP_SIZE],
                    int *out_x, int *out_y, int click_positions[MAX_CLICKS][2], int click_count, int argent){
    (void)renderer;
    int tile_x = mouse_x / TILE_SIZE;
    int tile_y = mouse_y / TILE_SIZE;
    
    
    
    if (argent < 10){ //plus tard : towern.cost typed tower d'indice n pour savoir son coût)
      return 0;
    }
    
    if (tile_x < 0 || tile_x >= MAP_SIZE || tile_y < 0 || tile_y >= MAP_SIZE) {
        return 0;
    }

    if (map[tile_x][tile_y] != 0 && map[tile_x][tile_y] != 1) {
    return 0;

    }

    int best_x = -1, best_y = -1;
    double min_dist = 1e9;

    for (int radius = 1; radius <= 3; radius++) {
        for (int dx = -radius; dx <= radius; dx++) {
            for (int dy = -radius; dy <= radius; dy++) {
                int nx = tile_x + dx;
                int ny = tile_y + dy;

                if (nx >= 0 && nx < MAP_SIZE && ny >= 0 && ny < MAP_SIZE && map[nx][ny] == 0) {
                    // Vérifier qu'aucune tourelle n'est déjà placée ici
                    int pixel_x = nx * TILE_SIZE;
                    int pixel_y = ny * TILE_SIZE;
                    int already_placed = 0;
                    for (int i = 0; i < click_count; i++) {
                        int cx = click_positions[i][0] / TILE_SIZE * TILE_SIZE;
                        int cy = click_positions[i][1] / TILE_SIZE * TILE_SIZE;
                        if (cx == pixel_x && cy == pixel_y) {
                            already_placed = 1;
                            break;
                        }
                    }

                    if (!already_placed) {
                        int px = nx * TILE_SIZE + TILE_SIZE / 2;
                        int py = ny * TILE_SIZE + TILE_SIZE / 2;
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
        *out_x = best_x * TILE_SIZE;
        *out_y = best_y * TILE_SIZE;
        return 1;
    }

    return 0;
}

void tourelle(tower* towers, int tower_count, enemy* enemies, int enemy_count, Mix_Chunk *tir_son) {
    for (int i = 0; i < tower_count; i++) {
        if (towers[i].tir_actif > 0) {
            towers[i].tir_actif--;
        }

        if (towers[i].cooldown > 0) {
            towers[i].cooldown--;
            continue;
        }

        int tower_cx = towers[i].x + TILE_SIZE / 2;
        int tower_cy = towers[i].y + TILE_SIZE / 2;

        for (int j = 0; j < enemy_count; j++) {
            if (enemies[j].life <= 0)
                continue;

            int enemy_cx = enemies[j].x * TILE_SIZE + TILE_SIZE / 2;
            int enemy_cy = enemies[j].y * TILE_SIZE + TILE_SIZE / 2;

            int dx = enemy_cx - tower_cx;
            int dy = enemy_cy - tower_cy;
            int distance_squared = dx * dx + dy * dy;

            if (distance_squared <= towers[i].range * towers[i].range) {
                enemies[j].life -= towers[i].damage;
                towers[i].cooldown = towers[i].speed_damage;
                towers[i].tir_actif = 10;
                printf("Tour %d a tiré sur ennemi %d ! Vie ennemie = %d\n", i, j, enemies[j].life);
                Mix_PlayChannel(-1, tir_son, 0);
                break;  // Une tour tire sur un seul ennemi à la fois
            }
        }
    }
}
