#ifndef GESTION_ENEMIES_H
#define GESTION_ENEMIES_H

#include "constantes_structures.h"


void move(SDL_Renderer *renderer,int side,int tile_size,  char map[side][side], int* vie, Enemy* enemies, int num, Tower* towers, int num_towers, int* argent);
void init_wave(Enemy *enemies, int count, int wave, int start_x, Uint32 current_time);
int all_enemies_dead(Enemy *enemies, int count);

#endif
