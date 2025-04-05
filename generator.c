#include <stdio.h>
#include <stdlib.h>
#include "generator.h"
#include "tiles.h"
// first char constraint:

const char constraints[6][6] = {
    //        f   w   c   p   l   d
    /*f*/  {1,  1,  1,  1,  1,  1},
    /*w*/  {1,  0,  1,  0,  0,  1},
    /*c*/  {1,  1,  0,  0,  0,  0},
    /*p*/  {1,  0,  0,  0,  0,  0},
    /*l*/  {1,  0,  0,  0,  0,  0},
    /*d*/  {1  ,1,  0,  0,  0,  0}
};

int get_number_from_type_char(char c) {
    // A basic function to index types of materials
    switch (c) {
        case 'f':
            return 0;
        case 'w':
            return 1;
        case 'c':
            return 2;
        case 'p':
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
    return constraints[get_number_from_type_char(a)][get_number_from_type_char(b)];
}
int isTileCompatible(char **a, char **b,char direction) {
    int rep = 0;
    switch (direction) {
        case 0: //compatible below
            for(int i = 0; i < 8; i++) {
                if(isPartCompatible(a[7][i], b[0][i])) {
                    rep++;
                } 
            }return rep;
        case 1://compatible above
            for(int i = 0; i < 8; i++) {
                if(isPartCompatible(a[0][i], b[7][i])) {
                    rep++;
                }
            }return rep;
        case 2: //compatible on the right
            for(int i = 0; i < 8; i++) {
                if(isPartCompatible(a[i][7], b[i][0])) {
                    rep++;
                }
            }return rep;
        case 3:
            for(int i = 0; i < 8; i++) {
                if(isPartCompatible(a[i][0], b[i][7])) {
                    rep++;
                }
            }return rep;
        default: return -1;
    }
}

