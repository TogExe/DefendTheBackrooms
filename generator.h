#ifndef GENERATOR_H
#define GENERATOR_H

#define MAX_SIDE  200
#define MAX_TILE_TYPES 52
#include <SDL2/SDL.h>

typedef struct {
    int count;
    int values[MAX_TILE_TYPES];
} Constraint;

void generate_grid_with_constraints(int side, char grid[MAX_SIDE][MAX_SIDE], Constraint constraints[], int constraint_count);
const char* get_color_for_tile(char tile);
SDL_Color get_sdl_color_for_tile(char tile);


#endif
