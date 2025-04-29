// constraintBuilder.h
#ifndef CONSTRAINTBUILDER_H
#define CONSTRAINTBUILDER_H

#define MAX_RULES 100
#define MAX_NAME_LEN 10
#define MAX_CHOICES 10

typedef struct {
    char from;
    char to[MAX_CHOICES];
    int to_count;
} ConstraintRule;

typedef struct {
    ConstraintRule rules[MAX_RULES];
    int count;
} ConstraintSet;

void init_constraints(ConstraintSet *set);
int get_tile_id(char name);

#endif // CONSTRAINTBUILDER_H
