#ifndef CONSTANTES_STRUCTURES_H
#define CONSTANTES_STRUCTURES_H


#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h> 
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <time.h>
#include <stdbool.h>


#define MAX_ENEMIES 1000

//#define MAP_SIZE 40
//#define TILE_SIZE 20
#define ENEMY_SIZE 5
#define MAX_CLICKS 1000
#define MAX_TOWERS 100



typedef struct{
	int x,y;
	int cost;
	int damage;
	int range;
	int speed_damage;
	int cooldown;
	int active_shoot ;
	int level;
}Tower;

typedef struct{
	int x, y ;
	int life;
	int speed;
	Uint32 last_move_time;
	Uint32 spawn_time;
	bool isalive;
	int l_p[2][2];
}Enemy;
  
typedef struct {
    int x1, y1, x2, y2;
    int frames_left;
} TirEffect;

int lire_et_modifier_seed(const char *nom_fichier, int nouvelle_seed);
void sauvegarder_partie(const char *filename, int seed, int wave, int vie, int argent, Tower *towers, int tower_count);
int charger_partie(const char *filename, int *seed, int *wave,int*vie,int*argent, Tower **towers1, int *tower_count,int t_size);
void charger_seed(const char *filename, int *seed); 

#endif
