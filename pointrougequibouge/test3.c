// Projet Tower Defense SDL - version optimisée et complète

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#define MAP_SIZE 35
#define TILE_SIZE 20
#define ENEMY_SIZE 5
#define MAX_CLICKS 1000
#define SCREEN_WIDTH (MAP_SIZE * TILE_SIZE)
#define SCREEN_HEIGHT (MAP_SIZE * TILE_SIZE)

// Structures

typedef struct {
    int cost;
    int dmg;
    int range;
    int speed_dmg;
} Tower;

typedef struct {
    int life;
    int speed;
    int x, y;
} Enemy;

// Map
int map[MAP_SIZE][MAP_SIZE];

// Fonctions
void generation_map(int map[MAP_SIZE][MAP_SIZE]) {
    int x = rand() % MAP_SIZE;
    for (int y = 0; y < MAP_SIZE; y++) {
        map[x][y] = 1;
        int dir = rand() % 3;
        if (dir == 0 && x > 0) x--;
        else if (dir == 2 && x < MAP_SIZE - 1) x++;
    }
}

void draw_map(SDL_Renderer *renderer) {
    for (int x = 0; x < MAP_SIZE; x++) {
        for (int y = 0; y < MAP_SIZE; y++) {
            if (map[x][y] == 1) SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);
            else SDL_SetRenderDrawColor(renderer, 34, 139, 34, 255);
            SDL_Rect rect = {x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

void draw_hud(SDL_Renderer *renderer, TTF_Font *font, int vie, int argent) {
    SDL_Color color = {255, 255, 255, 255};
    char buffer[64];

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
    SDL_Rect bg = {0, 0, 200, 50};
    SDL_RenderFillRect(renderer, &bg);

    snprintf(buffer, sizeof(buffer), "Vie : %d", vie);
    SDL_Surface *surf_vie = TTF_RenderText_Blended(font, buffer, color);
    SDL_Texture *tex_vie = SDL_CreateTextureFromSurface(renderer, surf_vie);
    SDL_Rect rect_vie = {10, 5, surf_vie->w, surf_vie->h};
    SDL_RenderCopy(renderer, tex_vie, NULL, &rect_vie);
    SDL_FreeSurface(surf_vie);
    SDL_DestroyTexture(tex_vie);

    snprintf(buffer, sizeof(buffer), "Argent : %d", argent);
    SDL_Surface *surf_arg = TTF_RenderText_Blended(font, buffer, color);
    SDL_Texture *tex_arg = SDL_CreateTextureFromSurface(renderer, surf_arg);
    SDL_Rect rect_arg = {10, 25, surf_arg->w, surf_arg->h};
    SDL_RenderCopy(renderer, tex_arg, NULL, &rect_arg);
    SDL_FreeSurface(surf_arg);
    SDL_DestroyTexture(tex_arg);
}

int place_tower(int mouse_x, int mouse_y, int click_positions[MAX_CLICKS][2], int *click_count, int *argent) {
    int tx = mouse_x / TILE_SIZE;
    int ty = mouse_y / TILE_SIZE;
    if (tx < 0 || tx >= MAP_SIZE || ty < 0 || ty >= MAP_SIZE) return 0;
    if (map[tx][ty] != 0) return 0;

    for (int i = 0; i < *click_count; i++) {
        if (click_positions[i][0] == tx && click_positions[i][1] == ty) return 0;
    }

    if (*argent >= 100) {
        click_positions[*click_count][0] = tx;
        click_positions[*click_count][1] = ty;
        (*click_count)++;
        *argent -= 100;
        return 1;
    }
    return 0;
}

void draw_towers(SDL_Renderer *renderer, SDL_Texture *icon, int click_positions[MAX_CLICKS][2], int click_count) {
    for (int i = 0; i < click_count; i++) {
        SDL_Rect dst = {click_positions[i][0] * TILE_SIZE, click_positions[i][1] * TILE_SIZE, TILE_SIZE, TILE_SIZE};
        SDL_RenderCopy(renderer, icon, NULL, &dst);
    }
}

int main() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    TTF_Init();
    IMG_Init(IMG_INIT_PNG);

    SDL_Window *win = SDL_CreateWindow("Tower Defense", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    TTF_Font *font = TTF_OpenFont("assets/font.TTF", 20);
    SDL_Texture *tower_texture = SDL_CreateTextureFromSurface(renderer, IMG_Load("icon.png"));
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    Mix_Music *music = Mix_LoadMUS("shoot.wav");
    Mix_PlayMusic(music, -1);

    generation_map(map);

    int click_positions[MAX_CLICKS][2];
    int click_count = 0;
    int vie = 10;
    int argent = 250;
    Enemy enemy = {.x = 0, .y = 0, .life = 100, .speed = 300};

    for (int i = 0; i < MAP_SIZE; i++) {
        if (map[i][0] == 1) {
            enemy.x = i;
            break;
        }
    }

    int running = 1;
    SDL_Event e;
    Uint32 last_move = SDL_GetTicks();

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = 0;
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                place_tower(x, y, click_positions, &click_count, &argent);
            }
        }

        Uint32 now = SDL_GetTicks();
        if (now - last_move > enemy.speed) {
            if (enemy.y + 1 < MAP_SIZE && map[enemy.x][enemy.y + 1] == 1) enemy.y++;
            else if (enemy.x + 1 < MAP_SIZE && enemy.y + 1 < MAP_SIZE && map[enemy.x + 1][enemy.y + 1] == 1) { enemy.x++; enemy.y++; }
            else if (enemy.x > 0 && enemy.y + 1 < MAP_SIZE && map[enemy.x - 1][enemy.y + 1] == 1) { enemy.x--; enemy.y++; }
            else vie--;
            last_move = now;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        draw_map(renderer);
        draw_towers(renderer, tower_texture, click_positions, click_count);
        draw_hud(renderer, font, vie, argent);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect enemy_rect = {enemy.x * TILE_SIZE, enemy.y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
        SDL_RenderFillRect(renderer, &enemy_rect);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyTexture(tower_texture);
    TTF_CloseFont(font);
    Mix_FreeMusic(music);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    Mix_CloseAudio();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}

