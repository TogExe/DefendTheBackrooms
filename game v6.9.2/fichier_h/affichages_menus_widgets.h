#ifndef AFFICHAGES_MENUS_WIDGETS_H
#define AFFICHAGES_MENUS_WIDGETS_H


#include "constantes_structures.h"

void paste_image(SDL_Renderer *renderer, const char *image_path, int x, int y, int tile_size);
void draw_hud(SDL_Renderer *renderer, TTF_Font *font, int vie, int argent,int wave);
int start_menu(SDL_Renderer *renderer, TTF_Font *font, int side, int tile_size);
int end_menu(SDL_Renderer *renderer, TTF_Font *font,int side, int tile_size);
void waiting(SDL_Renderer* renderer, SDL_Window* window, TTF_Font* font, int* start, int* vie, int* argent, int side,
             char map[side][side],int tile_size, int click_positions[MAX_CLICKS][2], int* click_count,
             Tower towers[MAX_TOWERS], int* tower_count, SDL_Texture* icon_texture);
SDL_Texture* charge_texture(SDL_Renderer *renderer, const char *path);
void clean_all(SDL_Renderer *renderer, SDL_Window *window, SDL_Texture *texture1, SDL_Texture *texture2, TTF_Font *font, Mix_Music *music, Mix_Chunk *sound);

#endif
