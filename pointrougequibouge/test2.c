#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h> 
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <time.h>

#define MAP_SIZE 70
#define TILE_SIZE 10
#define ENEMY_SIZE 5

void generation_map(int map[MAP_SIZE][MAP_SIZE]) {
    int x = rand() % MAP_SIZE;

    for (int y = 0; y < MAP_SIZE; y++) {
        map[x][y] = 1;

        int direction = rand() % 3;
        if (direction == 0 && x > 0) x--;
        else if (direction == 2 && x < MAP_SIZE - 1) x++;
    }
}

void draw_map(SDL_Renderer *renderer, int map[MAP_SIZE][MAP_SIZE]) {
    for (int x = 0; x < MAP_SIZE; x++) {
        for (int y = 0; y < MAP_SIZE; y++) {
            if (map[x][y] == 0) {
                SDL_SetRenderDrawColor(renderer, 34, 139, 34, 255); // Vert
            } else {
                SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255); // Marron
            }

            if (y == MAP_SIZE - 1 && map[x][y] == 1) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Fin du chemin
            }

            SDL_Rect rect = {x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

void draw_click_zone(SDL_Renderer *renderer, int mouse_x, int mouse_y, int map[MAP_SIZE][MAP_SIZE]) {
    int tile_x = mouse_x / TILE_SIZE;
    int tile_y = mouse_y / TILE_SIZE;

    if (tile_x < 0 || tile_x >= MAP_SIZE || tile_y < 0 || tile_y >= MAP_SIZE)
        return;

    if (map[tile_x][tile_y] == 1) {
        int best_x = -1, best_y = -1;
        double min_dist = 1e9;

        for (int radius = 1; radius <= 3; radius++) {
            for (int dx = -radius; dx <= radius; dx++) {
                for (int dy = -radius; dy <= radius; dy++) {
                    int nx = tile_x + dx;
                    int ny = tile_y + dy;

                    if (nx >= 0 && nx < MAP_SIZE && ny >= 0 && ny < MAP_SIZE && map[nx][ny] == 0) {
                        int px = nx * TILE_SIZE + TILE_SIZE / 2;
                        int py = ny * TILE_SIZE + TILE_SIZE / 2;

                        double dist = sqrt((mouse_x - px) * (mouse_x - px) + (mouse_y - py) * (mouse_y - py));
                        if (dist < min_dist) {
                            min_dist = dist;
                            best_x = nx;
                            best_y = ny;
                        }
                    }
                }
            }
        }

        if (best_x != -1 && best_y != -1) {
            tile_x = best_x;
            tile_y = best_y;
        } else {
            return;
        }
    }

    int aligned_x = tile_x * TILE_SIZE;
    int aligned_y = tile_y * TILE_SIZE;

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect rect = { aligned_x, aligned_y, TILE_SIZE, TILE_SIZE };
    SDL_RenderFillRect(renderer, &rect);
}

void affiche_image(SDL_Renderer *renderer, const char *image_path, int x, int y) {
    SDL_Surface *surface = IMG_Load(image_path);
    if (!surface) {
        printf("Erreur chargement image : %s\n", IMG_GetError());
        return;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        printf("Erreur création texture : %s\n", SDL_GetError());
        return;
    }

    SDL_Rect dst = { x, y, 32, 32 };
    SDL_RenderCopy(renderer, texture, NULL, &dst);
    SDL_DestroyTexture(texture);
}

void draw_hud(SDL_Renderer *renderer, TTF_Font *font, int points_de_vie, int argent) {
    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Rect background = {0, 0, 150, 60};

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
    SDL_RenderFillRect(renderer, &background);

    char buffer[64];

    snprintf(buffer, sizeof(buffer), "PV : %d", points_de_vie);
    SDL_Surface *surface_pv = TTF_RenderText_Blended(font, buffer, textColor);
    SDL_Texture *texture_pv = SDL_CreateTextureFromSurface(renderer, surface_pv);
    SDL_Rect dest_pv = {10, 10, surface_pv->w, surface_pv->h};
    SDL_FreeSurface(surface_pv);
    SDL_RenderCopy(renderer, texture_pv, NULL, &dest_pv);
    SDL_DestroyTexture(texture_pv);

    snprintf(buffer, sizeof(buffer), "Argent : %d", argent);
    SDL_Surface *surface_arg = TTF_RenderText_Blended(font, buffer, textColor);
    SDL_Texture *texture_arg = SDL_CreateTextureFromSurface(renderer, surface_arg);
    SDL_Rect dest_arg = {10, 30, surface_arg->w, surface_arg->h};
    SDL_FreeSurface(surface_arg);
    SDL_RenderCopy(renderer, texture_arg, NULL, &dest_arg);
    SDL_DestroyTexture(texture_arg);
}

int main(int argc, char *argv[]) {
    int map[MAP_SIZE][MAP_SIZE];
    srand(time(NULL)); 
    int enemy_x, enemy_y;
    int click_positions[1000][2];
    int click_count = 0;
    int vie = 100;
    int argent = 250;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Erreur SDL_Init : %s\n", SDL_GetError());
        return 1;
    }

    if (IMG_Init(IMG_INIT_PNG) == 0) {
        printf("Erreur IMG_Init : %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    if (TTF_Init() != 0) {
        printf("Erreur TTF_Init : %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    TTF_Font *font = TTF_OpenFont("asman.TTF", 16);
    if (!font) {
        printf("Erreur chargement police : %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Map 2D Aleatoire", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, MAP_SIZE * TILE_SIZE, MAP_SIZE * TILE_SIZE, 0);
    if (!window) {
        printf("Erreur SDL_CreateWindow : %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Erreur SDL_CreateRenderer : %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Surface *surface_icon = IMG_Load("icon.png");
    if (!surface_icon) {
        printf("Erreur chargement image : %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_Texture *icon_texture = SDL_CreateTextureFromSurface(renderer, surface_icon);
    SDL_FreeSurface(surface_icon);
    if (!icon_texture) {
        printf("Erreur création texture : %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    for (int x = 0; x < MAP_SIZE; x++)
        for (int y = 0; y < MAP_SIZE; y++)
            map[x][y] = 0;

    generation_map(map);

    for (int i = 0; i < MAP_SIZE; i++) {
        if (map[i][0] == 1) {
            enemy_x = i;
            enemy_y = 0;
            break;
        }
    }

    SDL_Event event;
    int running = 1;
    Uint32 last_move_time = 0;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = 0;

            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (click_count < 1000) {
                    click_positions[click_count][0] = x;
                    click_positions[click_count][1] = y;
                    click_count++;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        draw_map(renderer, map);

        for (int i = 0; i < click_count; i++) {
            int x = click_positions[i][0] - 16;
            int y = click_positions[i][1] - 16;
            SDL_Rect dst = { x, y, 32, 32 };
            SDL_RenderCopy(renderer, icon_texture, NULL, &dst);
        }

        draw_hud(renderer, font, vie, argent);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect enemy_rect = { enemy_x * TILE_SIZE, enemy_y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
        SDL_RenderFillRect(renderer, &enemy_rect);

        Uint32 now = SDL_GetTicks();
        if (now - last_move_time > 300) {
            if (enemy_y < MAP_SIZE - 1 && map[enemy_x][enemy_y + 1] == 1)
                enemy_y++;
            else if (enemy_y < MAP_SIZE - 1 && enemy_x < MAP_SIZE - 1 && map[enemy_x + 1][enemy_y + 1] == 1) {
                enemy_x++; enemy_y++;
            } else if (enemy_y < MAP_SIZE - 1 && enemy_x > 0 && map[enemy_x - 1][enemy_y + 1] == 1) {
                enemy_x--; enemy_y++;
            } else if (enemy_x > 0 && map[enemy_x - 1][enemy_y] == 1) {
                enemy_x--;
            } else if (enemy_x < MAP_SIZE - 1 && map[enemy_x + 1][enemy_y] == 1) {
                enemy_x++;
            }

            last_move_time = now;
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(100);
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(icon_texture);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}

