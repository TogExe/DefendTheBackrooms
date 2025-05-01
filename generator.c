#include "generator.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define MAX_TILE_CHARS 52  // 26 lowercase + 26 uppercase

// Convert tile ID (0 to 51) to character
char tile_id_to_char(int id) {
    if (id < 26) return 'a' + id;
    else if (id < 52) return 'A' + (id - 26);
    else return '?';  // Invalid
}

// Convert character to tile ID
int char_to_tile_id(char c) {
    if (c >= 'a' && c <= 'z') return c - 'a';
    else if (c >= 'A' && c <= 'Z') return 26 + (c - 'A');
    else return -1;
}

void generate_grid_with_constraints(int side, char grid[MAX_SIDE][MAX_SIDE], Constraint constraints[], int constraint_count) {
    srand(time(NULL));

    // Safety: Cap constraint_count to 52
    if (constraint_count > MAX_TILE_CHARS) {
        printf("Too many tile types (max 52 allowed).\n");
        return;
    }

    // Initialize grid to placeholder
    for (int i = 0; i < side; i++) {
        for (int j = 0; j < side; j++) {
            grid[i][j] = '.';  // Indicates uninitialized cell
        }
    }

    // Start with a random tile at (0, 0)
    int start_tile = rand() % constraint_count;
    grid[0][0] = tile_id_to_char(start_tile);

    // Fill grid
    for (int y = 0; y < side; y++) {
        for (int x = 0; x < side; x++) {
            if (grid[x][y] == '.') {
                char options[MAX_TILE_TYPES];
                int options_count = 0;

                for (int i = 0; i < constraint_count; i++) {
                    int valid = 1;

                    // Check left neighbor
                    if (x > 0 && grid[x][y-1] != '.') {
                        int left = char_to_tile_id(grid[x][y-1]);
                        if (left < 0 || left >= constraint_count) {
                            valid = 0;
                        } else {
                            int found = 0;
                            for (int j = 0; j < constraints[left].count; j++) {
                                if (constraints[left].values[j] == i) {
                                    found = 1;
                                    break;
                                }
                            }
                            if (!found) valid = 0;
                        }
                    }

                    // Check top neighbor
                    if (y > 0 && grid[x-1][y] != '.') {
                        int top = char_to_tile_id(grid[x-1][y]);
                        if (top < 0 || top >= constraint_count) {
                            valid = 0;
                        } else {
                            int found = 0;
                            for (int j = 0; j < constraints[top].count; j++) {
                                if (constraints[top].values[j] == i) {
                                    found = 1;
                                    break;
                                }
                            }
                            if (!found) valid = 0;
                        }
                    }

                    if (valid) {
                        options[options_count++] = tile_id_to_char(i);
                    }
                }

                if (options_count > 0) {
                    grid[x][y] = options[rand() % options_count];
                } else {
                    // Fallback: random tile
                    grid[x][y] = tile_id_to_char(rand() % constraint_count);
                }
            }
        }
    }
}

/*
void generate_grid_with_constraints(int side, char grid[MAX_SIDE][MAX_SIDE], Constraint constraints[], int constraint_count) {
    srand(time(NULL));
    // Initialize grid with empty cells
    for (int i = 0; i < side; i++) {
        for (int j = 0; j < side; j++) {
            grid[i][j] = 0;
        }
    }

    // Pick a random tile to start
    int start_tile = rand() % constraint_count;
    grid[0][0] = 'a' + start_tile; // simple first guess

    // Fill the grid
    for (int y = 0; y < side; y++) {
        for (int x = 0; x < side; x++) {
            if (grid[y][x] == 0) { // Only fill empty cells
                char options[MAX_TILE_TYPES];
                int options_count = 0;

                // Collect valid tiles based on neighbors
                for (int i = 0; i < constraint_count; i++) {
                    int valid = 1;

                    // Check tile to the left
                    if (x > 0 && grid[y][x-1] != 0) {
                        int left = grid[y][x-1] - 'a';
                        int found = 0;
                        for (int j = 0; j < constraints[left].count; j++) {
                            if (constraints[left].values[j] == i) {
                                found = 1;
                                break;
                            }
                        }
                        if (!found) {
                            valid = 0;
                        }
                    }

                    // Check tile above
                    if (y > 0 && grid[y-1][x] != 0) {
                        int top = grid[y-1][x] - 'a';
                        int found = 0;
                        for (int j = 0; j < constraints[top].count; j++) {
                            if (constraints[top].values[j] == i) {
                                found = 1;
                                break;
                            }
                        }
                        if (!found) {
                            valid = 0;
                        }
                    }

                    if (valid) {
                        options[options_count++] = 'a' + i;
                    }
                }

                if (options_count > 0) {
                    // Randomly select one of the valid options
                    grid[y][x] = options[rand() % options_count];
                } else {
                    // Fallback: Random tile if no options found
                    grid[y][x] = 'a' + (rand() % constraint_count);
                }
            }
        }
    }
}*/
