#ifndef GESTION_TOURELLES_H
#define GESTION_TOURELLES_H

#include "constantes_structures.h"

int draw_click_zone(SDL_Renderer *renderer, int mouse_x, int mouse_y,int side,  char map[side][side], int tile_size,
                    int *out_x, int *out_y, int click_positions[MAX_CLICKS][2], int click_count, int argent);
void turret(Tower* towers, int tower_count, Enemy* enemies, int enemy_count, Mix_Chunk *tir_son, int tile_size);
void upgrade(Tower* towers, int tower_count, int mouse_x, int mouse_y, int tile_size) ;

#endif
