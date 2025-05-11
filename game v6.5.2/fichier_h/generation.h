#ifndef GENERATION_H
#define GENERATION_H

#include "constantes_structures.h"

SDL_Color get_sdl_color_for_tile(char tile);
void generation_map(int side, char** map);
void draw_map(SDL_Renderer *renderer, int side,int tile_size, char **map);
int find_start(int side, char ** map);

#endif
 	
