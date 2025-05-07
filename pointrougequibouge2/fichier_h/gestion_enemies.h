#ifndef GESTION_ENEMIES_H
#define GESTION_ENEMIES_H

#include "constantes_structures.h"


void deplacement_ennemi(SDL_Renderer *renderer, char map[MAP_SIZE][MAP_SIZE], int* vie, enemy* enemies, int num, tower* towers, int num_towers, int* argent);
void initialiser_vague(enemy *enemies, int count, int wave, int start_x, Uint32 current_time);
int tous_les_ennemis_morts(enemy *enemies, int count);

#endif
