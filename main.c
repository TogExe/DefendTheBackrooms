#include "generator.h"
#include "pathfinder.h"
#include "constraintBuilder.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_TILE_TYPES 26


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
// Function to get the ANSI escape code based on the tile type
const char* get_color_for_tile(char tile) {
    switch (tile) {
        case 'a':
            return "\033[0;32m";  // Green for 'a'
        case 'b':
            return "\033[0;33m";  // Yellow for 'b'
        case 'c':
            return "\033[0;34m";  // Blue for 'c'
        case 'd':
            return "\033[0;35m";  // Magenta for 'd'
        case 'e':
            return "\033[0;36m";  // Cyan for 'e'
        case 'f':
            return "\033[0;37m";  // White for 'f'
        case 'x':
            return "\033[1;31m";  // Red for 'x'
        case 'w':
            return "\033[0;31m";  // Dark Red for 'w'
        case 'y':
            return "\033[1;33m";  // Light Yellow for 'y'
        case 'z':
            return "\033[1;32m";  // Light Green for 'z'
        case ' ':
          	return "\033[1;23m";
        default:
            return "\033[0m";     // Default color (reset)
    }
}
const char* reset_color() {
    return "\033[0m"; // Reset to default terminal color
}

int main() {
    srand(time(NULL));

    int side = MAX_SIDE-1;
    char grid[MAX_SIDE][MAX_SIDE] = {{0}};
    Constraint constraints[MAX_TILE_TYPES];
    int tile_count = 0;

    ConstraintSet set;
    init_constraints(&set);
    convert_constraints(&set, constraints, &tile_count);

    char avoid[3] = {'w','x','e'};
    char prefer[7] = {'a','b','d'};
    int ptgs = -1;
    while (ptgs ==-1)
    {
        generate_grid_with_constraints(side, grid, constraints, tile_count);
        ptgs = generate_organic_path(grid, side, avoid, 3, prefer, 4);
    }
    // Debugging: Print the grid after path generation
    printf("Grid after path generation:\n");
    for (int i = 0; i < side; i++) {
        for (int j = 0; j < side; j++) {
            printf("  %c", grid[i][j]);
        }
        printf("\n\n");

    }
    // In your main function or where you're printing the grid:
	printf("Grid after path generation:\n");
	for (int i = 0; i < side; i++) {
    	for (int j = 0; j < side; j++) {
        	// Get the color for the current tile
        	const char* color = get_color_for_tile(grid[i][j]);

        	// Print the tile with the color
        	printf("%s   %c", color,grid[i][j]);
    }
    // Reset color after printing the row
    printf("%s\n\n", reset_color());
}


    return 0;
}
