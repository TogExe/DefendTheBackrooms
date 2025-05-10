
#include "fichier_h/affichages_menus_widgets.h"

#include "fichier_h/gestion_enemies.h"
#include "fichier_h/gestion_tourelles.h"
#include "fichier_h/generation.h"
#include "fichier_h/constantes_structures.h"


void paste_image(SDL_Renderer *renderer, const char *image_path, int x, int y, int tile_size) {
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

    SDL_Rect dst = { x, y, tile_size, tile_size }; // Ajuste selon la taille souhaitée
    SDL_RenderCopy(renderer, texture, NULL, &dst);
    SDL_DestroyTexture(texture);

  // Détruire après affichage (si temporaire)
}

//••••••••••••••••••••••••••••••••••FONCTTION WIDGET VIE/ARGENT•••••••••••••••••••••••••••••••••••••••••••••••

void draw_hud(SDL_Renderer *renderer, TTF_Font *font, int vie, int argent) {
    SDL_Color textColor = {255, 255, 255, 255}; // Blanc
    SDL_Rect background = {0, 0, 150, 60}; // Rectangle noir semi-transparent en haut à gauche

    // Dessiner le fond
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200); // Noir semi-transparent
    SDL_RenderFillRect(renderer, &background);

    // Préparer les textes
    char buffer[64];
    
    // PV
    snprintf(buffer, sizeof(buffer), "PV : %d", vie);
    SDL_Surface *surface_pv = TTF_RenderText_Blended(font, buffer, textColor);
    SDL_Texture *texture_pv = SDL_CreateTextureFromSurface(renderer, surface_pv);
    SDL_Rect dest_pv = {10, 10, surface_pv->w, surface_pv->h};
    SDL_FreeSurface(surface_pv);
    SDL_RenderCopy(renderer, texture_pv, NULL, &dest_pv);
    SDL_DestroyTexture(texture_pv);

    // Argent
    snprintf(buffer, sizeof(buffer), "Argent : %d", argent);
    SDL_Surface *surface_arg = TTF_RenderText_Blended(font, buffer, textColor);
    SDL_Texture *texture_arg = SDL_CreateTextureFromSurface(renderer, surface_arg);
    SDL_Rect dest_arg = {10, 30, surface_arg->w, surface_arg->h};
    SDL_FreeSurface(surface_arg);
    SDL_RenderCopy(renderer, texture_arg, NULL, &dest_arg);
    SDL_DestroyTexture(texture_arg);
    
    
}


//••••••••••••••••••••••••••••••fonction menu return 0 si jeu non lancé et 1 si passe au jeu•••••••••••••••••••
/*
int start_menu(SDL_Renderer *renderer, TTF_Font *font, int side, int tile_size) {
   	 SDL_Event event;
    int menu_running = 1;
    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Color bgColor = {0, 0, 0, 255};

    SDL_Surface *surface = TTF_RenderText_Blended(font, "CLIQUEZ POUR COMMENCER", textColor);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    
    int texW = 0, texH = 0;
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
    SDL_Rect dstrect = {side * tile_size / 2 - texW / 2, side * tile_size / 2 - texH / 2, texW, texH};

    while (menu_running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                SDL_DestroyTexture(texture);
                return 0; // Quitte le jeu
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                menu_running = 0;
            }
        }

        SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, &dstrect);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    return 1; // Passe au jeu
}

int end_menu(SDL_Renderer *renderer, TTF_Font *font, int side, int tile_size) {
    
    SDL_Event event;
    int menu_running = 1;
    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Color bgColor = {0, 0, 0, 255};

    SDL_Surface *surface = TTF_RenderText_Blended(font, "GAME OVER", textColor);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    
    int texW = 0, texH = 0;
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
    SDL_Rect dstrect = {side * tile_size / 2 - texW / 2, side * tile_size / 2 - texH / 2, texW, texH};

    while (menu_running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                SDL_DestroyTexture(texture);
                return 0; // Quitte le jeu
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                menu_running = 0;
            }
        }

        SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, &dstrect);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    return 1; // termine le jeu
    return 0;
}

void waiting(SDL_Renderer* renderer, SDL_Window* window, TTF_Font* font, int* start, int* vie, int* argent, int side,
	
             char** map, int tile_size, int click_positions[MAX_CLICKS][2], int* click_count,
             Tower towers[MAX_TOWERS], int* tower_count, SDL_Texture* icon_texture) {
	
    SDL_Event event;
    SDL_Color text_color = {0, 0, 0, 255};
    SDL_Color bg_color = {200, 200, 200, 255};
    SDL_Color border_color = {0, 0, 0, 255};

    int window_width, window_height;
    SDL_GetWindowSize(window, &window_width, &window_height);

    SDL_Rect widget_rect = {
        window_width - 50,
        window_height / 2 - 75,
        43,
        16                           
    };

    *start = 0;

    while (!*start) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                SDL_Quit();
                exit(0);
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mx = event.button.x;
                int my = event.button.y;

                // Clic sur le bouton START
                if (mx >= widget_rect.x -20  && mx <= widget_rect.x +60  &&
                    my >= widget_rect.y -20 && my <= widget_rect.y +40){
                    *start = 1;
                }
                else {
                    // Clic pour placer une tourelle
                    int final_x, final_y;
                    if (*click_count < MAX_CLICKS &&
                        draw_click_zone(renderer, mx, my,side,   map,tile_size,  &final_x, &final_y, click_positions, *click_count, *argent)) {

                        click_positions[*click_count][0] = final_x;
                        click_positions[*click_count][1] = final_y;
                        (*click_count)++;
                        *argent -= 10;

                        towers[*tower_count].x = final_x;
                        towers[*tower_count].y = final_y;
                        towers[*tower_count].damage = 1;
                        towers[*tower_count].range = 3 * tile_size;
                        towers[*tower_count].speed_damage = 30;
                        towers[*tower_count].cooldown = 0;
                        (*tower_count)++;
                    }
                }
            }
        }

        // Nettoyage + affichage map
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        draw_map(renderer,side, tile_size , map);

        // Dessiner les tourelles déjà posées
        for (int i = 0; i < *click_count; i++) {
            int x = click_positions[i][0];
            int y = click_positions[i][1];
            SDL_Rect dst = { x, y, tile_size, tile_size };
            SDL_RenderCopy(renderer, icon_texture, NULL, &dst);
        }

        // Dessiner HUD
        draw_hud(renderer, font, *vie, *argent);

        // Afficher bouton START
        SDL_SetRenderDrawColor(renderer, bg_color.r, bg_color.g, bg_color.b, bg_color.a);
        SDL_RenderFillRect(renderer, &widget_rect);

        SDL_SetRenderDrawColor(renderer, border_color.r, border_color.g, border_color.b, border_color.a);
        SDL_RenderDrawRect(renderer, &widget_rect);

        SDL_Surface* text_surface = TTF_RenderText_Blended(font, "START", text_color);
        SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
        SDL_Rect text_rect = {
            widget_rect.x ,
            widget_rect.y ,
            text_surface->w,
            text_surface->h
        };
        SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);
        SDL_FreeSurface(text_surface);
        SDL_DestroyTexture(text_texture);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
    
    //return 0;
}*/

SDL_Texture* charge_texture(SDL_Renderer *renderer, const char *path) {
    SDL_Surface *surface = IMG_Load(path);
    if (!surface) return NULL;
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void clean_all(SDL_Renderer *renderer, SDL_Window *window, SDL_Texture *texture1, SDL_Texture *texture2, TTF_Font *font, Mix_Music *music, Mix_Chunk *sound) {
    if (music) Mix_FreeMusic(music);
    if (sound) Mix_FreeChunk(sound);

    Mix_HaltMusic();
    Mix_CloseAudio();

    if (texture1) SDL_DestroyTexture(texture1);
    if (texture2) SDL_DestroyTexture(texture2);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    if (font) TTF_CloseFont(font);

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
