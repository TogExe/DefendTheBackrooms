#include "headers/generator.h"
#include "headers/pathfinder.h"
#include "headers/constraintBuilder.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>

// Function to create and display a colorized grid buffer in the terminal
void create_terminal_grid_buffer(char grid[MAX_SIDE][MAX_SIDE], int width, int height) {
	// MOST USELESS PROCEDURE PRETTY MUCH LIKE MINECRAFT 1.17 UPDATE
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            char tile = grid[y][x];
            //const char* color = get_color_for_tile(tile);

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

//to move it to the new program

int main(int argc, char* argv[]) {

   	
	int seed =time(NULL);// 1746632788;//time(NULL);
	printf("%d \n",seed);
	seed = 1746634253;
	srand(seed);
    int side = rand()% MAX_SIDE-1;
    char grid[MAX_SIDE][MAX_SIDE] = {{0}};
	
	smpl_gen(seed,side,grid);
	
	printf("generation has officially been completed\n");
	SDL_Init(SDL_INIT_VIDEO);
	printf("video pass\n");
    // Create a window
    printf(".");
    SDL_Window* window = SDL_CreateWindow("Pixel Grid", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1000, 1000, SDL_WINDOW_SHOWN);
	printf(".");
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	printf(".");
    // Example grid of tiles
	
    // Terminal grid display
    //create_terminal_grid_buffer(grid, 5, 5);

    // Example grid size for SDL rendering
    int gridSize = 1000/side; // Each pixel is 40x40 in size

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
        draw_pixel_grid(renderer, side,side, gridSize, grid);

        // Present the renderer
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
