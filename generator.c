#include "headers/generator.h"
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

void generate_grid_with_constraints(int seed,int side, char grid[MAX_SIDE][MAX_SIDE], Constraint constraints[], int constraint_count) {
	if (grid==NULL){
		printf("ERROR GRID Pointer has been set to null ! ! ! You probably have no more ram :(\n ");
	}
    srand(seed);

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

const char* get_color_for_tile(char tile) {
    switch (tile) {
        case 'a': return "\033[0;32m";  // Green for 'a'
        case 'b': return "\033[0;33m";  // Yellow for 'b'
        case 'c': return "\033[0;34m";  // Blue for 'c'
        case 'd': return "\033[0;35m";  // Magenta for 'd'
        case 'e': return "\033[0;36m";  // Cyan for 'e'
        case 'f': return "\033[0;37m";  // White for 'f'
        case 'x': return "\033[1;31m";  // Red for 'x'
        case 'w': return "\033[0;31m";  // Dark Red for 'w'
        case 'y': return "\033[1;33m";  // Light Yellow for 'y'
        case 'z': return "\033[1;32m";  // Light Green for 'z'
        case ' ': return "\033[1;23m";  // Default background
        default: return "\033[0m";      // Reset color
    }
}

SDL_Color get_sdl_color_for_tile(char tile) {
    switch (tile) {
        // Forest and Grassland biomes (greens)
        case 'g': return (SDL_Color){100, 180, 100, 255};    // Grass
        case 'f': return (SDL_Color){120, 200, 120, 255};    // Light Grass
        case 'a': return (SDL_Color){140, 220, 140, 255};    // Forest or grasslands
        case 'b': return (SDL_Color){130, 200, 130, 255};    // Lush grass, grassland mix
        case 'c': return (SDL_Color){160, 220, 160, 255};    // Fertile land and grass
        case 'd': return (SDL_Color){140, 200, 140, 255};    // Lush or rich forest
        case 'e': return (SDL_Color){130, 180, 130, 255};    // Dry forest or field
        case 'h': return (SDL_Color){80, 60, 40, 255};       // Wetlands/mud
        case 'i': return (SDL_Color){60, 50, 30, 255};       // Dark soil (swamp)
        case 'j': return (SDL_Color){100, 140, 80, 255};     // Wet grassland or transition
        case 'k': return (SDL_Color){120, 160, 100, 255};    // Bog or swamp
        case 'l': return (SDL_Color){110, 150, 90, 255};     // Swamp transition

        // Desert and Rocky biomes (yellows, oranges, browns)
        case 'm': return (SDL_Color){200, 180, 100, 255};    // Desert scrub or arid land
        case 'n': return (SDL_Color){180, 150, 100, 255};    // Rocky outcrops
        case 'o': return (SDL_Color){210, 180, 110, 255};    // Sand dunes and rocky desert
        case 'p': return (SDL_Color){230, 200, 120, 255};    // Dry desert, scrub, or dunes
        case 'q': return (SDL_Color){180, 160, 130, 255};    // Rocky desert terrain
        case 'r': return (SDL_Color){160, 140, 110, 255};    // Rocky desert with sparse vegetation
        case 's': return (SDL_Color){190, 160, 120, 255};    // Desert cliffs or rocky transition

        // Frozen biomes (snow, ice)
        case 't': return (SDL_Color){210, 220, 255, 255};    // Snow and ice terrain
        case 'u': return (SDL_Color){190, 200, 255, 255};    // Ice plains or frozen lakes
        case 'v': return (SDL_Color){180, 190, 255, 255};    // Glacial terrain
        case 'w': return (SDL_Color){160, 170, 255, 255};    // Snow-covered mountains

        // Special transitional or mixed biomes
        case 'x': return (SDL_Color){180, 160, 255, 255};    // Mixed biome, desert transitioning to frozen
        case 'y': return (SDL_Color){170, 150, 255, 255};    // Mountainous regions with forests and snow
        case 'z': return (SDL_Color){190, 170, 255, 255};    // Desert transition to mountains

        // Empty space (black background)
        case ' ': return (SDL_Color){0, 0, 0, 255};          // Empty space (black background)

        default: return (SDL_Color){255, 255, 255, 255};     // Fallback (white)
    }
}
