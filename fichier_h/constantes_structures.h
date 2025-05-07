#ifndef CONSTANTES_STRUCTURES_H
#define CONSTANTES_STRUCTURES_H


#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h> 
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <time.h>



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
  }Tower;
  
typedef struct{
  int x, y ;
  int life;
  int speed;
  Uint32 last_move_time;
  Uint32 spawn_time;
  }Enemy;
  
typedef struct {
    int x1, y1, x2, y2;
    int frames_left;
} TirEffect;

#endif
