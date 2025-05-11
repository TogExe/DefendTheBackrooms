#ifndef SAVE_H
#define SAVE_H
#include "constantes_structures.h"

int read_write_seed(const char *nom_fichier, int nouvelle_seed);
void save(const char *filename, int seed, int wave, int vie, int argent, Tower *towers, int tower_count);
int load_game(const char *filename, int *seed, int *wave, int *vie, int *argent, Tower **towers1, int *tower_count,int t_size) ;
void load_seed(const char *filename, int *seed) ;

#endif

