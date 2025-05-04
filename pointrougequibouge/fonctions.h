// game.h

#ifndef FONCTIONS_H
#define FONCTIONS_H


#include <stdlib.h>

#include <SDL2/SDL.h> 
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <time.h>





#define MAP_SIZE 30
#define TILE_SIZE 25
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
  int tir_actif ;
  }tower;
  
typedef struct{
  int life;
  int speed;
  }enemy;
  
typedef struct {
    int x1, y1, x2, y2;
    int frames_left;
} TirEffet;






// Déclaration des fonctions utilisées dans le main
void generation_map(int map[MAP_SIZE][MAP_SIZE]);
void draw_map(SDL_Renderer *renderer, int map[MAP_SIZE][MAP_SIZE]);
int draw_click_zone(SDL_Renderer *renderer, int mouse_x, int mouse_y, int map[MAP_SIZE][MAP_SIZE],
                    int *out_x, int *out_y, int click_positions[MAX_CLICKS][2], int click_count, int argent);
void affiche_image(SDL_Renderer *renderer, const char *image_path, int x, int y);
void draw_hud(SDL_Renderer *renderer, TTF_Font *font, int vie, int argent);
int afficher_menu_debut(SDL_Renderer *renderer, TTF_Font *font);
int afficher_menu_fin(SDL_Renderer *renderer, TTF_Font *font);
void deplacement_ennemi(SDL_Renderer *renderer, int* enemy_x, int* enemy_y, int map[MAP_SIZE][MAP_SIZE], int* vie, enemy* current_enemy, tower* towers, int num_towers, int* argent);
void widget(SDL_Window **window, SDL_Renderer *renderer);
void tourelle(tower* towers, int tower_count, enemy* current_enemy, int enemy_x, int enemy_y, Mix_Chunk *tir_son);
void waiting(SDL_Renderer* renderer, SDL_Window* window, TTF_Font* font, int* start, int* vie, int* argent,
             int map[MAP_SIZE][MAP_SIZE], int click_positions[MAX_CLICKS][2], int* click_count,
             tower towers[MAX_TOWERS], int* tower_count, SDL_Texture* icon_texture) ;

#endif

