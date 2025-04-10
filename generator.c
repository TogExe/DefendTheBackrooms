#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "generator.h"
#include "tiles.h"

#define MAP_SIZE 10
#define TILE_SIZE 3

// first char constraint:
const char constraints[6][6] = {
    //        .   x   +   #   l   d
    /*f*/  {1,  1,  1,  0,  0,  0},
    /*w*/  {1,  0,  0,  0,  0,  0},
    /*c*/  {1,  0,  0,  0,  0,  0},
    /*p*/  {0,  0,  0,  1,  0,  0},
    /*l*/  {1,  0,  0,  0,  0,  0},
    /*d*/  {0  ,0,  0,  0,  0,  0}
};

int get_number_from_type_char(char c) {
    // A basic function to index types of materials
    switch (c) {
        case '.':
            return 0;
        case 'x':
            return 1;
        case '+':
            return 2;
        case '#':
            return 3;
        case 'l':
            return 4;
        case 'd':
            return 5;
        default:
            return -1;
    }
}

int isPartCompatible(char a, char b) {
    int indexA = get_number_from_type_char(a);
    int indexB = get_number_from_type_char(b);
    if (indexA == -1 || indexB == -1) return 0;
    return constraints[indexA][indexB];
}

int isTileCompatible(char **a, char **b, char direction) {
    int rep = 0;
    switch (direction) {
        case 0: // compatible below
            for (int i = 0; i < 3; i++) {
                if (isPartCompatible(a[2][i], b[0][i])) {
                    rep++;
                }
            }
            return rep == 3;
        case 1: // compatible above
            for (int i = 0; i < 3; i++) {
                if (isPartCompatible(a[0][i], b[2][i])) {
                    rep++;
                }
            }
            return rep == 3;
        case 2: // compatible on the right
            for (int i = 0; i < 3; i++) {
                if (isPartCompatible(a[i][2], b[i][0])) {
                    rep++;
                }
            }
            return rep == 3;
        case 3: // compatible on the left
            for (int i = 0; i < 3; i++) {
                if (isPartCompatible(a[i][0], b[i][2])) {
                    rep++;
                }
            }
            return rep == 3;
        default:
            return 0;
    }
}

const char tiles[13][3][3] = {
    {
        {'.','.','.'},
        {'#','#','#'},
        {'.','.','.'}
    },
    {
        {'.','#','.'},
        {'.','#','.'},
        {'.','#','.'}
    },
    {
        {'.','.','.'},
        {'.','x','.'},
        {'.','.','.'}
    },
    {
        {'+','#','.'},
        {'#','#','.'},
        {'.','.','.'}
    },
    {
        {'.','#','+'},
        {'.','#','#'},
        {'.','.','.'}
    },
    {
        {'.','.','.'},
        {'.','#','#'},
        {'.','#','+'}
    },
    {
        {'.','.','.'},
        {'#','#','.'},
        {'+','#','.'}
    },
    {
        {'+','#','+'},
        {'#','#','#'},
        {'.','.','.'}
    },
    {
        {'.','#','+'},
        {'.','#','#'},
        {'.','#','+'}
    },
    {
        {'.','.','.'},
        {'#','#','#'},
        {'+','#','+'}
    },
    {
        {'+','#','.'},
        {'#','#','.'},
        {'+','#','.'}
    },
    {
        {'.','.','.'},
        {'.','x','.'},
        {'.','.','.'}
    },
    {
        {'.','.','.'},
        {'.','.','.'},
        {'.','.','.'}
    }
};

void initialize_map(tile map[MAP_SIZE][MAP_SIZE], int size) {
    for (int x = 0; x < size; x++) {
        for (int y = 0; y < size; y++) {
            map[x][y].opt_count = 13;
            map[x][y].opt = malloc(sizeof(char**) * 13);
            for (int k = 0; k < 13; k++) {
                map[x][y].opt[k] = malloc(sizeof(char*) * 3);
                map[x][y].choice = NULL;
                for (int i = 0; i < 3; i++) {
                    map[x][y].opt[k][i] = malloc(sizeof(char) * 3);
                    memcpy(map[x][y].opt[k][i], tiles[k][i], 3 * sizeof(char));
                }
            }
        }
    }
}

void free_map(tile map[MAP_SIZE][MAP_SIZE], int size) {
    for (int x = 0; x < size; x++) {
        for (int y = 0; y < size; y++) {
            for (int k = 0; k < 13; k++) {
                for (int i = 0; i < 3; i++) {
                    free(map[x][y].opt[k][i]);
                }
                free(map[x][y].opt[k]);
            }
            free(map[x][y].opt);
        }
    }
}

void generation(tile map[MAP_SIZE][MAP_SIZE], int size) {
    srand(time(NULL));
    int d = size * size;
    do {
        int x_ = rand() % size;
        int y_ = rand() % size;
        if (!map[x_][y_].choice) {
            int tile_index = rand() % 13;
            char **a = map[x_][y_].opt[tile_index];
            if (a) {
                map[x_][y_].choice = a;
                for (int k = 0; k < map[x_][y_].opt_count-1; k++) {
                    if (y_ + 1 < size && !isTileCompatible(a, map[x_][y_ + 1].opt[k], 1)) {
                        map[x_][y_ + 1].opt[k] = NULL;
                        map[x_][y_ + 1].opt_count--;
                    }
                }
                for(int l; l<map[x_+1][y_].opt_count-1; l++){
                    if (x_ + 1 < size && !isTileCompatible(a, map[x_ + 1][y_].opt[l], 2)) {
                        map[x_ + 1][y_].opt[l] = NULL;
                        map[x_ + 1][y_].opt_count--;
                    }
                }
            }
        }
        d--;
        printf("%d \t", d);
    } while (d > 0);
}
