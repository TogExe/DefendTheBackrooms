//
// Created by TogExe on 05/04/2025.
//

#ifndef GENERATOR_H
#define GENERATOR_H

#define MAX_OPTIONS 4

typedef struct {
    int options[MAX_OPTIONS];
    int option_count;
    int collapsed; // 1 if a tile has been chosen
} Cell;

char* build_amap_please_i_want_one_very_very_much(int size);


#endif //GENERATOR_H
