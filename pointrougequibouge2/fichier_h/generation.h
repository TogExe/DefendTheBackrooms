#ifndef GENERATION_H
#define GENERATION_H

#include "constantes_structures.h"

SDL_Color get_sdl_color_for_tile(char tile);
void generation_map(int map[MAP_SIZE][MAP_SIZE]);
void draw_map(SDL_Renderer *renderer, int map[MAP_SIZE][MAP_SIZE]);
int trouver_depart(int map[MAP_SIZE][MAP_SIZE]);

#endif
