#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "constraintBuilder.h"

#define MAX_TILE_TYPES 26

typedef struct {
    char name;
    int id;
} TileMapping;

TileMapping tile_mappings[MAX_TILE_TYPES] = {
    {'a', 0}, {'b', 1}, {'c', 2}, {'d', 3}, {'e', 4},
    {'f', 5}, {'g', 6}, {'h', 7}, {'i', 8}, {'j', 9},
    {'k', 10}, {'l', 11}, {'m', 12}, {'n', 13}, {'o', 14},
    {'p', 15}, {'q', 16}, {'r', 17}, {'s', 18}, {'t', 19},
    {'u', 20}, {'v', 21}, {'w', 22}, {'x', 23}, {'y', 24}, {'z', 25}
};

int get_tile_id(char name) {
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

    // Improved rules for better biome distribution
    char grass_all[] = {'a', 'b', 'c'}; // Grass can transition into grass, forest, or plains
    char forest_all[] = {'a', 'd', 'e'}; // Forest can transition into grass, forest, or river
    char river_all[] = {'b', 'f', 'x'}; // River can transition into grass, sand, or rocks
    char desert_all[] = {'g', 'h', 'f'}; // Desert can transition into sand, rocks, or shrubs
    char mountain_all[] = {'i', 'j', 'k'}; // Mountain can transition into rock, snow
    char swamp_all[] = {'l', 'm', 'n'}; // Swamp can transition into water, forest, grass
    char water_all[] = {'o', 'p', 'q'}; // Water can transition into beach, swamp, grass
    char sand_all[] = {'r', 's', 't'}; // Sand can transition into desert, water, beach
    char beach_all[] = {'u', 'v', 'w'}; // Beach can transition into sand, grass, water

    // Define transition rules based on tile groups
    add_rule(set, 'a', grass_all, 3); // Grass rules
    add_rule(set, 'b', forest_all, 3); // Forest rules
    add_rule(set, 'c', river_all, 3); // River rules
    add_rule(set, 'd', desert_all, 3); // Desert rules
    add_rule(set, 'e', mountain_all, 3); // Mountain rules
    add_rule(set, 'f', swamp_all, 3); // Swamp rules
    add_rule(set, 'g', water_all, 3); // Water rules
    add_rule(set, 'h', sand_all, 3); // Sand rules
    add_rule(set, 'i', beach_all, 3); // Beach rules

    // Additional biome-specific rules
    char tree_all[] = {'d', 'e'}; // Trees can transition into forest or mountain tiles
    add_rule(set, 't', tree_all, 2);

    // Debugging: Print the initialized constraints
    printf("Initialized Constraints:\n");
    for (int i = 0; i < set->count; i++) {
        printf("Rule %d: from %c to ", i, set->rules[i].from);
        for (int j = 0; j < set->rules[i].to_count; j++) {
            printf("%c ", set->rules[i].to[j]);
        }
        printf("\n");
    }
}

/*void init_constraints(ConstraintSet *set) {
    set->count = 0;

    char a1_all[] = {'b'};
    char a2_all[] = {'c'};
    char a3_all[] = {'d'};
    char a4_all[] = {'e', 'a', 'c'};
    char a5_all[] = {'a', 'f', 'x'};
    char x_all[] = {'x'};
    char y_all[] = {'y'};
    char xz_all[] = {'z'};
    char yz_all[] = {'z'};
    char z_all[] = {'x', 'y', 'f'};
    char floor_all[] = {'w', 'f', 'f'};
    char f_all[] = {'a', 'b', 'x', 'y'};
    char b1_all[] = {'b'};
    char b2_all[] = {'c'};
    char b3_all[] = {'d'};
    char b4_all[] = {'e'};
    char b5_all[] = {'b', 'c', 'f', 'y'};
    char wall_all[] = {'f', 'a', 'b', 'c', 'd'};
    char wall_end_all[] = {'f'};

    add_rule(set, 'a', a1_all, 1);
    add_rule(set, 'b', a2_all, 1);
    add_rule(set, 'c', a3_all, 1);
    add_rule(set, 'd', a4_all, 3);
    add_rule(set, 'e', a5_all, 3);
    add_rule(set, 'x', x_all, 1);
    add_rule(set, 'y', y_all, 1);
    add_rule(set, 'x', xz_all, 1);
    add_rule(set, 'y', yz_all, 1);
    add_rule(set, 'z', z_all, 3);
    add_rule(set, 'f', floor_all, 3);
    add_rule(set, 'f', f_all, 4);
    add_rule(set, 'b', b1_all, 1);
    add_rule(set, 'c', b2_all, 1);
    add_rule(set, 'd', b3_all, 1);
    add_rule(set, 'e', b4_all, 1);
    add_rule(set, 'b', b5_all, 4);
    add_rule(set, 'w', wall_all, 5);
    add_rule(set, 'w', wall_end_all, 1);

    / Optional: Random corruptors
    char corrupt_targets[] = {'a', 'b', 'x', 'y'};
    for (int i = 1; i <= 3; ++i) {
        if (rand() % 2 == 0) {
            char name = 'c' + i - 1;
            char to[2] = {
                corrupt_targets[rand() % 4],
                corrupt_targets[rand() % 4]
            };
            add_rule(set, name, to, 2);
        }
    }

    // Debugging: Print the initialized constraints
    printf("Initialized Constraints:\n");
    for (int i = 0; i < set->count; i++) {
        printf("Rule %d: from %c to ", i, set->rules[i].from);
        for (int j = 0; j < set->rules[i].to_count; j++) {
            printf("%c ", set->rules[i].to[j]);
        }
        printf("\n");
    }
}
*/
