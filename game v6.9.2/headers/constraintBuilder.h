#ifndef CONSTRAINTBUILDER_H
#define CONSTRAINTBUILDER_H

#define MAX_RULES 256              // Increased to allow more rule entries
#define MAX_NAME_LEN 10
#define MAX_CHOICES 10            // Allow more neighbor choices per rule
#define MAX_TILE_TYPES 52       // 'a'-'z' and 'A'-'Z'

typedef struct {
    char from;                    // The tile this rule applies to
    char to[MAX_CHOICES];        // Valid neighboring tile types
    int to_count;                // Number of allowed neighbors
} ConstraintRule;

typedef struct {
    ConstraintRule rules[MAX_RULES];
    int count;
} ConstraintSet;

// Initializes the constraint set with default or predefined rules
void init_constraints(ConstraintSet *set);

// Converts tile character to an internal ID (e.g. 'a' = 0, 'z' = 25, 'A' = 26, ..., 'Z' = 51)
int get_tile_id(char name);
void convert_constraints(Constraint constraints[], int *constraint_count);
#endif // CONSTRAINTBUILDER_H
