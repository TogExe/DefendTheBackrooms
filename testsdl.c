
#include "fichier_h/gestion_enemies.h"
#include "fichier_h/gestion_tourelles.h"
#include "fichier_h/affichages_menus_widgets.h"
#include "fichier_h/generation.h"
#include "fichier_h/constantes_structures.h"


// ████████████████████████████████ MAIN ████████████████████████████████
int main() {

    // ████████████████████████████████ VARIABLES INITIALES ████████████████████████████████
    SDL_Event e;
    int side=40;
        int tile_size=20;
        
    char map[side][side];
    int click_positions[MAX_CLICKS][2];
    int click_count = 0, tower_count = 0, num_towers = 0, start = 0;
    int vie =  rand()%3 + 2, argent = 50, compteur = 0, wave = 0;
    int current_enemy_count = 0;
    Enemy enemies[MAX_ENEMIES];
    Tower towers[MAX_TOWERS];
    srand(time(NULL));


    

    // ████████████████████████████████ INITIALISATION SDL & LIBS ████████████████████████████████
    if (SDL_Init(SDL_INIT_VIDEO) || IMG_Init(IMG_INIT_PNG) == 0 || TTF_Init() != 0) {
        printf("Erreur initialisation SDL/IMG/TTF : %s\n", SDL_GetError());
        return 1;
    }

    // ████████████████████████████████ INITIALISATION AUDIO ████████████████████████████████
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("Erreur SDL_mixer : %s\n", Mix_GetError());
        return 1;
    }

    Mix_Music *musique1 = Mix_LoadMUS("assets/menu.mp3");
    Mix_Music *musique2 = Mix_LoadMUS("assets/play.mp3");
    Mix_Chunk *tir_son = Mix_LoadWAV("assets/shoot.wav");

    if (!musique1 || !musique2 || !tir_son) {
        printf("Erreur chargement audio : %s\n", Mix_GetError());
    }

    // ████████████████████████████████ FENÊTRE, RENDERER & POLICE ████████████████████████████████
    TTF_Font *font = TTF_OpenFont("assets/asman.TTF", 16);
    if (!font) {
        printf("Erreur police : %s\n", TTF_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Map 2D Aleatoire", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, side * tile_size, side * tile_size, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!window || !renderer) {
        printf("Erreur création fenêtre/renderer : %s\n", SDL_GetError());
        return 1;
    }

    // ████████████████████████████████ TEXTURES TOURELLES ████████████████████████████████
    SDL_Texture *icon_texture = charge_texture(renderer, "assets/tmonkey.png");
    SDL_Texture *icon_texture1 = charge_texture(renderer, "assets/ninja.png");
    if (!icon_texture || !icon_texture1) return 1;

    // ████████████████████████████████ GÉNÉRATION MAP ████████████████████████████████
    memset(map, 0, sizeof(map));
    generation_map(side, map);

    // ████████████████████████████████ MENU DE DÉBUT ████████████████████████████████
    Mix_PlayMusic(musique1, -1);
    if (!start_menu(renderer, font, side, tile_size)) {
        clean_all(renderer, window, icon_texture, icon_texture1, font, musique1, tir_son);
        return 0;
    }
    Mix_FreeMusic(musique1);
    Mix_PlayMusic(musique2, -1);

    // ████████████████████████████████ PHASE D’ATTENTE AVANT VAGUE ████████████████████████████████
    waiting(renderer, window, font, &start, &vie, &argent,side, map,tile_size,  click_positions, &click_count, towers, &tower_count, icon_texture);

    // ████████████████████████████████ INITIALISATION PREMIÈRE VAGUE ████████████████████████████████
    wave++;
    current_enemy_count = 2;
    int start_x = find_start(side, map);
    Uint32 current_time = SDL_GetTicks();
    init_wave(enemies, current_enemy_count, wave, start_x, current_time);

    // ████████████████████████████████ BOUCLE PRINCIPALE ████████████████████████████████
    int running = 1;
    while (running) {
        // ████████████████████████████████ ÉVÉNEMENTS UTILISATEUR ████████████████████████████████
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = 0;

            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x, y, final_x, final_y;
                SDL_GetMouseState(&x, &y);

                if (click_count < MAX_CLICKS && draw_click_zone(renderer, x, y, side, map,tile_size,  &final_x, &final_y, click_positions, click_count, argent)) {
                    click_positions[click_count][0] = final_x;
                    click_positions[click_count][1] = final_y;
                    click_count++;
                    argent -= 10;
                    towers[tower_count++] = (Tower){.x = final_x, .y = final_y, .damage = 1, .range = 3 * tile_size, .speed_damage = 30, .cooldown = 0};
                    num_towers = tower_count;
                }
            }
        }

        // ████████████████████████████████ AFFICHAGE ████████████████████████████████
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        draw_map(renderer,side,tile_size,  map);
        for (int i = 0; i < tower_count; i++) {
            SDL_Rect dst = { towers[i].x, towers[i].y, tile_size, tile_size };
            SDL_RenderCopy(renderer, towers[i].active_shoot > 0 ? icon_texture1 : icon_texture, NULL, &dst);
        }

        draw_hud(renderer, font, vie, argent);

        // ████████████████████████████████ ENNEMIS ████████████████████████████████
        for (int i = 0; i < current_enemy_count; i++) {
            if (enemies[i].life > 0)
                move(renderer,side, tile_size, map,  &vie, enemies, i, towers, num_towers, &argent);
        }

        // ████████████████████████████████ TOURELLES TIRENT ████████████████████████████████
        turret(towers, tower_count, enemies, current_enemy_count, tir_son, tile_size);

        // ████████████████████████████████ CHECK VAGUE TERMINÉE ████████████████████████████████
        if (all_enemies_dead(enemies, current_enemy_count)) {
            argent += 5 * current_enemy_count;
            waiting(renderer, window, font, &start, &vie, &argent,side,  map, tile_size, click_positions, &click_count, towers, &tower_count, icon_texture);
            wave++;
            current_enemy_count = wave + 1;
            start_x = find_start(side, map);
            current_time = SDL_GetTicks();
            init_wave(enemies, current_enemy_count, wave, start_x, current_time);
        }

        // ████████████████████████████████ ENNEMI ARRIVÉ EN BAS ████████████████████████████████
        for (int i = 0; i < current_enemy_count; i++) {
            if (enemies[i].y == side - 1) {
                compteur++;
                enemies[i].life = 0;
                //vie--;
                if (vie <= 0) {
                    printf("tu es mort");
                    end_menu(renderer, font, side, tile_size);
                    running = 0;
                    break;
                }
                //waiting(renderer, window, font, &start, &vie, &argent, map, click_positions, &click_count, towers, &tower_count, icon_texture);
                start_x = find_start(side, map);
                enemies[i] = (Enemy){.x = start_x, .y = 0, .life = 10 + compteur, .speed = 1, .last_move_time = 0, .spawn_time = SDL_GetTicks()};
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    // ████████████████████████████████ NETTOYAGE FINAL ████████████████████████████████
    clean_all(renderer, window, icon_texture, icon_texture1, font, musique2, tir_son);
    return 0;
}
