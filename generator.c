#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "generator.h"

#define MAP_SIZE 120 // must be a multiple of 3
#define TILE_SIZE 3
#define NUM_TILES 13

// misc
#define DEBUG 1

// first char constraint:
const char constraints[6][6] = {
    //        .   x   +   #   l   d
    /*.*/  {1,  1,  1,  0,  0,  0},
    /*x*/  {1,  0,  0,  0,  0,  0},
    /*+*/  {1,  0,  0,  0,  0,  0},
    /*#*/  {0,  0,  0,  1,  0,  0},
    /*l*/  {1,  0,  0,  0,  0,  0},
    /*d*/  {0  ,0,  0,  0,  0,  0}
};

int get_number_from_type_char(char c) {
    if(DEBUG==3) {
        printf("get_number_from_type_char(%c)\n", c);
    }
    switch (c) {
        case '.': return 0;
        case 'x': return 1;
        case '+': return 2;
        case '#': return 3;
        case 'l': return 4;
        case 'd': return 5;
        default: return -1;
    }
}

int isPartCompatible(char a, char b) {
    int indexA = get_number_from_type_char(a);
    int indexB = get_number_from_type_char(b);
    if (indexA == -1 || indexB == -1) return 0;
    return constraints[indexA][indexB];
}

int isTileCompatible(char a[3][3], char b[3][3], char direction) {
    if(DEBUG==2) {
        printf("isTileCompatible(char ** a,char ** b, char direction)\n");
    }
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

const char tiles[NUM_TILES+1][3][3] = {
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
    },
    {
        {'d','d','d'},
        {'d','.','d'},
        {'d','d','d'}
    }
};

void init_chunks(tile **tile_buffer, int count) {
    // Basic setup
    if (tile_buffer != NULL) {
        for (int x = 0; x < count; x++) {
            for (int y = 0; y < count; y++) {
                tile_buffer[x][y].status = 3;
                tile_buffer[x][y].choice = 14;
                for (int j = 0; j < NUM_TILES; j++) {
                    tile_buffer[x][y].opt[j] = 4;
                }
            }
        }
    }else{printf("Error in init_chunks :  tile_buffer missing address\n");}
}

void noise(tile **tile_buffer, int count) {
    // Noising for favorising choices
    srand(time(NULL));
    if(tile_buffer!=NULL) {
        for (int x = 0; x < count; x++) {
            for (int y = 0; y < count; y++) {
                const char t = rand() % NUM_TILES;
                if (tile_buffer[x][y].status==4 && rand() % 2 == 0) {
                    if (tile_buffer[x][y].opt[t] >= 4) {
                        tile_buffer[x][y].choice = t;
                        tile_buffer[x][y].status -= 1;
                    }
                }
            }
        }
    }else{printf("Error in noise() :  tile_buffer missing address\n");}
}

void make_coherent(tile **tile_buffer, int count) {
    int decided =0;
    // In progress
    int c_matrix_up[NUM_TILES+1][NUM_TILES+1];
    int c_matrix_lf[NUM_TILES+1][NUM_TILES+1];
    for (int i = 0; i < NUM_TILES+1; i++) {
        for (int j = 0; j < NUM_TILES+1; j++) {
            c_matrix_up[i][j] = isTileCompatible(tiles[i], tiles[j], 1);
            c_matrix_lf[i][j] = isTileCompatible(tiles[i], tiles[j], 2);
            //printf("\t%d,%d", c_matrix_up[i][j], c_matrix_lf[i][j]);
        }
        //printf("\n");
    }
    if (tile_buffer!= NULL) {
        for (int x = 0; x < count; x++) {
            for (int y = 0; y < count; y++) {
                // Implement coherence logic here
                if (tile_buffer[x][y].status<4 && tile_buffer[x][y].status>0) {
                    if(x+1<count) {
                        if (tile_buffer[x+1][y].status<4) {
                            if(!c_matrix_lf[tile_buffer[x+1][y].choice][tile_buffer[x+1][y].choice]) {
                                if (tile_buffer[x][y].status<tile_buffer[x+1][y].status) {
                                    tile_buffer[x][y].status -=1;
                                    tile_buffer[x+1][y].choice =13;
                                    tile_buffer[x+1][y].status = 4;
                                }else if (tile_buffer[x][y].status==tile_buffer[x+1][y].status) {
                                    tile_buffer[x][y].status = 4;
                                    tile_buffer[x][y].choice = 13;
                                    tile_buffer[x+1][y].status = 4;
                                    tile_buffer[x+1][y].choice = 13;
                                }else {
                                    tile_buffer[x][y].choice = 13;
                                    tile_buffer[x][y].status = 4;
                                    tile_buffer[x+1][y].status -= 1;
                                }
                            }else {
                                tile_buffer[x][y].status -=1;
                                tile_buffer[x+1][y].status -=1;
                            }
                        }
                    }
                    //Choosing or keeping both
                    if(y+1<count) {
                        if (tile_buffer[x][y+1].status<4) {
                            if(!c_matrix_up[tile_buffer[x][y+1].choice][tile_buffer[x][y+1].choice]) {
                                if (tile_buffer[x][y].status<tile_buffer[x][y+1].status) {
                                    tile_buffer[x][y].status -=1;
                                    tile_buffer[x][y+1].choice = 13;
                                    tile_buffer[x][y+1].choice = 13;
                                } else if (tile_buffer[x][y].status==tile_buffer[x][y+1].status) {
                                    tile_buffer[x][y].status = 4;
                                    tile_buffer[x][y].choice = 13;
                                    tile_buffer[x][y+1].status = 4;
                                    tile_buffer[x][y+1].choice = 13;
                                }
                                else {
                                    tile_buffer[x][y].choice = 13;
                                    tile_buffer[x][y].status = 4;
                                    tile_buffer[x][y+1].status -= 1;
                                }
                            }else {
                                tile_buffer[x][y].status -=1;
                                tile_buffer[x][y+1].status -=1;
                            }
                        }
                    }
                }else if (tile_buffer[x][y].status<=0) {
                    tile_buffer[x][y].status =0;
                    decided+=1;
                }
            }
        }
    }else{printf("Error in make_coherent: tile_buffer missing address\n");}
    if(DEBUG) {printf("%d tiles collapsed\n",decided);}
}

tile **generate_chunks(int count) {
    tile **chunks = (tile **)malloc(count * sizeof(tile *));
    for (int i = 0; i < count; i++) {
        chunks[i] = (tile *)malloc(count * sizeof(tile));
    }
    init_chunks(chunks, count);
    for(int j =0;j<1000;j++) {
        noise(chunks, count);
        make_coherent(chunks, count);
    }
    return chunks;
}

char **generate_world() {
    // Implement world generation logic here
    return NULL;
}

int main() {
    int count = MAP_SIZE / TILE_SIZE;
    tile **chunks = generate_chunks(count);
    // Free allocated memory
    generate_chunks(count);
    for (int i = 0; i < count; i++) {
        free(chunks[i]);
    }
    free(chunks);
    return 0;
}
