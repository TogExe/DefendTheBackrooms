#include "generator.h"
#include "pathfinder.h"
#include "constraintBuilder.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>

void convert_constraints(ConstraintSet *set, Constraint constraints[], int *constraint_count) {
    *constraint_count = 0;

    for (int i = 0; i < set->count; ++i) {
        ConstraintRule *rule = &set->rules[i];
        int from_id = get_tile_id(rule->from);
        if (from_id == -1) continue; // Skip if the tile is not found

        Constraint *constraint = &constraints[*constraint_count];
        constraint->count = rule->to_count;

        for (int j = 0; j < rule->to_count; ++j) {
            int to_id = get_tile_id(rule->to[j]);
            if (to_id == -1) continue; // Skip if the tile is not found
            constraint->values[j] = to_id;
        }

        (*constraint_count)++;
    }

    // Debugging: Print the converted constraints
    printf("Converted Constraints:\n");
    for (int i = 0; i < *constraint_count; i++) {
        printf("Constraint %d: ", i);
        for (int j = 0; j < constraints[i].count; j++) {
            printf("%c ", constraints[i].values[j]);
        }
        printf("\n");
    }
}

// Convert tile to corresponding color code for terminal (as in your original function)
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

// Convert tile to corresponding SDL RGBA color
/*SDL_Color get_sdl_color_for_tile(char tile) {
    switch (tile) {
        // Forest and Grassland biomes (greens)
        case 'g': return (SDL_Color){80, 160, 80, 255};     // Grass
        case 'f': return (SDL_Color){110, 190, 110, 255};   // Light Grass
        case 'a': return (SDL_Color){140, 220, 140, 255};   // Forest or grasslands
        case 'b': return (SDL_Color){150, 200, 150, 255};   // Lush grass, grassland mix
        case 'c': return (SDL_Color){180, 240, 180, 255};   // Fertile land and grass
        case 'd': return (SDL_Color){180, 240, 180, 255};   // Lush or rich forest
        case 'e': return (SDL_Color){150, 200, 150, 255};   // Dry forest or field
        case 'h': return (SDL_Color){70, 50, 30, 255};      // Wetlands/mud
        case 'i': return (SDL_Color){60, 40, 20, 255};      // Dark soil (swamp)
        case 'j': return (SDL_Color){80, 120, 60, 255};     // Wet grassland or transition
        case 'k': return (SDL_Color){100, 140, 80, 255};    // Bog or swamp
        case 'l': return (SDL_Color){90, 130, 90, 255};     // Swamp transition

        // Desert and Rocky biomes (yellows, oranges, browns)
        case 'm': return (SDL_Color){255, 224, 100, 255};   // Desert scrub or arid land
        case 'n': return (SDL_Color){180, 160, 110, 255};   // Rocky outcrops
        case 'o': return (SDL_Color){210, 180, 120, 255};   // Sand dunes and rocky desert
        case 'p': return (SDL_Color){230, 200, 130, 255};   // Dry desert, scrub, or dunes
        case 'q': return (SDL_Color){180, 170, 140, 255};   // Rocky desert terrain
        case 'r': return (SDL_Color){160, 140, 120, 255};   // Rocky desert with sparse vegetation
        case 's': return (SDL_Color){190, 160, 130, 255};   // Desert cliffs or rocky transition

        // Frozen biomes (snow, ice)
        case 't': return (SDL_Color){200, 220, 255, 255};   // Snow and ice terrain
        case 'u': return (SDL_Color){190, 210, 255, 255};   // Ice plains or frozen lakes
        case 'v': return (SDL_Color){180, 200, 255, 255};   // Glacial terrain
        case 'w': return (SDL_Color){160, 180, 255, 255};   // Snow-covered mountains

        // Special transitional or mixed biomes
        case 'x': return (SDL_Color){170, 150, 255, 255};   // Mixed biome, desert transitioning to frozen
        case 'y': return (SDL_Color){160, 140, 255, 255};   // Mountainous regions with forests and snow
        case 'z': return (SDL_Color){180, 160, 255, 255};   // Desert transition to mountains

        // Empty space (black background)
        case ' ': return (SDL_Color){0, 0, 0, 255};         // Empty space (black background)

        default: return (SDL_Color){255, 255, 255, 255};    // Fallback (white)
    }
}
*/
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




// Function to create and display a colorized grid buffer in the terminal
void create_terminal_grid_buffer(char grid[MAX_SIDE][MAX_SIDE], int width, int height) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            char tile = grid[y][x];
            const char* color = get_color_for_tile(tile);

            // Print color for each tile followed by the tile
            printf("%c", tile);
        }
        // Reset color at the end of each row
        printf("\033[0m\n");
    }
}

// Function to draw a pixel grid using SDL
void draw_pixel_grid(SDL_Renderer* renderer, int width, int height, int gridSize, char grid[MAX_SIDE][MAX_SIDE]) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Get the SDL color for the tile
            SDL_Color color = get_sdl_color_for_tile(grid[x][y]);

            // Set the drawing color for SDL
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

            // Draw the pixel (as a filled rectangle)
            SDL_Rect rect = {x * gridSize, y * gridSize, gridSize, gridSize};
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

int main(int argc, char* argv[]) {
    int side = MAX_SIDE-1;
    char grid[MAX_SIDE][MAX_SIDE] = {{0}};
    Constraint constraints[MAX_TILE_TYPES];
    int tile_count = 0;

    ConstraintSet set;
    init_constraints(&set);
    convert_constraints(&set, constraints, &tile_count);

    char avoid[5] = {'w','x','e','K','J'};
    char prefer[7] = {'a','b','c','d','e','f','g'};
    int ptgs = -1;
    while (ptgs ==-1)
    {
        generate_grid_with_constraints(side, grid, constraints, tile_count);
        create_terminal_grid_buffer(grid,50,50);
        ptgs = generate_organic_path(grid, side, avoid, 5, prefer, 7);
    }

    SDL_Init(SDL_INIT_VIDEO);

    // Create a window
    SDL_Window* window = SDL_CreateWindow("Pixel Grid", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Example grid of tiles

    // Terminal grid display
    create_terminal_grid_buffer(grid, 5, 5);

    // Example grid size for SDL rendering
    int gridSize = 5; // Each pixel is 40x40 in size

    // Main loop for SDL rendering
    int running = 1;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw the pixel grid
        draw_pixel_grid(renderer, MAX_SIDE,MAX_SIDE, gridSize, grid);

        // Present the renderer
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
