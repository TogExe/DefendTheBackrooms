#ifndef GENERATOR_H
#define GENERATOR_H

#define MAX_SIDE  400
#define MAX_TILE_TYPES 52
#include <SDL2/SDL.h>

typedef struct {
    int count;
    int values[MAX_TILE_TYPES];
} Constraint;

void generate_grid_with_constraints(int seed ,int side, char grid[MAX_SIDE][MAX_SIDE]);
const char* get_color_for_tile(char tile);
SDL_Color get_sdl_color_for_tile(char tile);
void smpl_gen(int seed,int side,char ** grid, char ** blocker);

#endif
