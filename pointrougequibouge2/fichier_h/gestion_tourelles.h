#ifndef GESTION_TOURELLES_H
#define GESTION_TOURELLES_H

#include "constantes_structures.h"

int draw_click_zone(SDL_Renderer *renderer, int mouse_x, int mouse_y, int map[MAP_SIZE][MAP_SIZE],
                    int *out_x, int *out_y, int click_positions[MAX_CLICKS][2], int click_count, int argent);
void tourelle(tower* towers, int tower_count, enemy* enemies, int enemy_count, Mix_Chunk *tir_son);

#endif
