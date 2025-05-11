#include <stdio.h>
#include "headers/generator.h"
#include "headers/constraintBuilder.h"



typedef struct {
    char name;
    int id;
} TileMapping;




int get_tile_id(char name) {
     TileMapping tile_mappings[MAX_TILE_TYPES] = {
	    {'a', 0},  {'b', 1},  {'c', 2},  {'d', 3},  {'e', 4},
	    {'f', 5},  {'g', 6},  {'h', 7},  {'i', 8},  {'j', 9},
	    {'k', 10}, {'l', 11}, {'m', 12}, {'n', 13}, {'o', 14},
	    {'p', 15}, {'q', 16}, {'r', 17}, {'s', 18}, {'t', 19},
	    {'u', 20}, {'v', 21}, {'w', 22}, {'x', 23}, {'y', 24},
	    {'z', 25}, {'A', 26}, {'B', 27}, {'C', 28}, {'D', 29},
	    {'E', 30}, {'F', 31}, {'G', 32}, {'H', 33}, {'I', 34},
	    {'J', 35}, {'K', 36}, {'L', 37}, {'M', 38}, {'N', 39},
	    {'O', 40}, {'P', 41}, {'Q', 42}, {'R', 43}, {'S', 44},
	    {'T', 45}, {'U', 46}, {'V', 47}, {'W', 48}, {'X', 49},
	    {'Y', 50}
    };
    for (int i = 0; i < MAX_TILE_TYPES; i++) {
        if (tile_mappings[i].name == name) {
            return tile_mappings[i].id;
        }
    }
    return -1; // Return -1 if the name is not found
}

void add_rule(ConstraintSet *set, char from, char to_options[], int to_count) {
    if (set->count >= MAX_RULES) return;

    ConstraintRule *rule = &set->rules[set->count++];
    rule->from = from;
    rule->to_count = to_count;

    for (int i = 0; i < to_count; ++i) {
        rule->to[i] = to_options[i];
    }
}

void init_constraints(ConstraintSet *set) {
    set->count = 0;

    // Grassland and Forest Biomes
    add_rule(set, 'g', (char[]){'g', 'f', 'a'}, 3); // Grass
    add_rule(set, 'f', (char[]){'f', 'g', 'e'}, 3); // Light grass or shrubland
    add_rule(set, 'a', (char[]){'a', 'f', 'g'}, 3); // Forest or grasslands
    add_rule(set, 'b', (char[]){'b', 'a', 'g'}, 3); // Lush grass, grassland mix
    add_rule(set, 'c', (char[]){'c', 'b', 'a'}, 3); // Fertile land and grass
    add_rule(set, 'd', (char[]){'d', 'c', 'b'}, 3); // Lush or rich forest
    add_rule(set, 'e', (char[]){'e', 'd', 'c'}, 3); // Dry forest or field
    add_rule(set, 'h', (char[]){'h', 'g', 'f'}, 3); // Wetlands/mud
    add_rule(set, 'i', (char[]){'i', 'h', 'g'}, 3); // Swampy or marshland
    add_rule(set, 'j', (char[]){'j', 'h', 'f'}, 3); // Wet grassland or transition
    add_rule(set, 'k', (char[]){'k', 'i', 'j'}, 3); // Bog or swamp
    add_rule(set, 'l', (char[]){'l', 'j', 'i'}, 3); // Swamp transition

    // Desert Biomes
    add_rule(set, 'm', (char[]){'m', 'l', 'k'}, 3); // Desert scrub or arid land
    add_rule(set, 'n', (char[]){'n', 'm', 'k'}, 3); // Rocky outcrops
    add_rule(set, 'o', (char[]){'o', 'm', 'n'}, 3); // Sand dunes and rocky desert
    add_rule(set, 'p', (char[]){'p', 'o', 'm'}, 3); // Dry desert, scrub, or dunes
    add_rule(set, 'q', (char[]){'q', 'p', 'n'}, 3); // Rocky desert terrain
    add_rule(set, 'r', (char[]){'r', 'q', 'p'}, 3); // Rocky desert with sparse vegetation
    add_rule(set, 's', (char[]){'s', 'r', 'o'}, 3); // Desert cliffs or rocky transition

    // Frozen Biomes (snow, ice)
    add_rule(set, 't', (char[]){'t', 's', 'r'}, 3); // Snow and ice terrain
    add_rule(set, 'u', (char[]){'u', 't', 's'}, 3); // Ice plains or frozen lakes
    add_rule(set, 'v', (char[]){'v', 'u', 't'}, 3); // Glacial terrain
    add_rule(set, 'w', (char[]){'w', 'v', 'u'}, 3); // Snow-covered mountains

    // Special Transitional or Mixed Biomes
    add_rule(set, 'x', (char[]){'x', 'r', 'v'}, 3); // Mixed biome, desert transitioning to frozen
    add_rule(set, 'y', (char[]){'y', 't', 'e'}, 3); // Mountainous regions with forests and snow
    add_rule(set, 'z', (char[]){'z', 'm', 'y'}, 3); // Desert transition to mountains

   /* // Debugging output
    printf("Initialized Constraints for Natural Biomes:\n");
    for (int i = 0; i < set->count; i++) {
        printf("Rule %d: from %c to ", i, set->rules[i].from);
        for (int j = 0; j < set->rules[i].to_count; j++) {
            printf("%c ", set->rules[i].to[j]);
        }
        printf("\n");
    }*/
}
void convert_constraints(Constraint constraints[], int *constraint_count) {
	ConstraintSet set;
	init_constraints(&set);
	
    *constraint_count = 0;

    for (int i = 0; i < set.count; ++i) {
        ConstraintRule *rule = &set.rules[i];
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
    /* printf("Converted Constraints:\n");
    for (int i = 0; i < *constraint_count; i++) {
        printf("Constraint %d: ", i);
        for (int j = 0; j < constraints[i].count; j++) {
            printf("%c ", constraints[i].values[j]);
        }
        printf("\n");
    }*/
}
