//
// Created by TogExe on 05/04/2025.
//
#ifndef GENERATOR_H
#define GENERATOR_H

typedef struct {
    int opt_count;
    char *** opt;
    char ** choice;
} tile;

#define TILE_SIZE 3
#define MAP_SIZE 10

void initialize_map(tile map[MAP_SIZE][MAP_SIZE], int size);
void free_map(tile map[MAP_SIZE][MAP_SIZE], int size);
void generation(tile map[MAP_SIZE][MAP_SIZE], int size);
#endif // GENERATOR_H
