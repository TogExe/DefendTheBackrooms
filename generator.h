#ifndef GENERATOR_H
#define GENERATOR_H

#define MAX_SIDE  100
#define MAX_TILE_TYPES 26

typedef struct {
    int count;
    int values[MAX_TILE_TYPES];
} Constraint;

void generate_grid_with_constraints(int side, char grid[MAX_SIDE][MAX_SIDE], Constraint constraints[], int constraint_count);

#endif
