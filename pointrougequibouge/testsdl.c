

#include "fonctions.h"

// ████████████████████████████████ MAIN ████████████████████████████████
int main(int argc, char *argv[]) {

    // ████████████████████████████████ VARIABLES INITIALES ████████████████████████████████
    SDL_Event e;
    int map[MAP_SIZE][MAP_SIZE];
    srand(time(NULL));

    int enemy_x = 0, enemy_y = 0;
    int click_positions[MAX_CLICKS][2];
    int click_count = 0;
    int vie = 2, argent = 50;
    int tower_count = 0, num_towers = 0, start = 0;
    int compteur = 0, wave = 0, tir_count = 0;
    enemy current_enemy = {.life = 5, .speed = 1};
    enemy enemies[MAX_ENEMIES];
    tower towers[MAX_TOWERS];
    tower t;
    TirEffet tirs[100];
    int current_enemy_count = 0;


    // ████████████████████████████████ INITIALISATION SDL ████████████████████████████████
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

    // ████████████████████████████████ INITIALISATION AUDIO ████████████████████████████████
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("Erreur SDL_mixer : %s\n", Mix_GetError());
        return 1;
    }

    Mix_Music *musique1 = Mix_LoadMUS("menu.mp3");
    Mix_Music *musique2 = Mix_LoadMUS("play.mp3");
    Mix_Chunk *tir_son = Mix_LoadWAV("shoot.wav");

    if (!musique1 || !musique2 || !tir_son) {
        printf("Erreur chargement audio : %s\n", Mix_GetError());
    }

    // ████████████████████████████████ FONTS & RENDERER ████████████████████████████████
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

    // ████████████████████████████████ TEXTURES TOURELLES ████████████████████████████████
    SDL_Surface *surface_icon = IMG_Load("tmonkey.png");
    SDL_Surface *surface_icon1 = IMG_Load("ninja.png");
    SDL_Texture *icon_texture = SDL_CreateTextureFromSurface(renderer, surface_icon);
    SDL_Texture *icon_texture1 = SDL_CreateTextureFromSurface(renderer, surface_icon1);
    SDL_FreeSurface(surface_icon);
    SDL_FreeSurface(surface_icon1);

    if (!icon_texture || !icon_texture1) {
        printf("Erreur création texture : %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // ████████████████████████████████ GÉNÉRATION MAP ████████████████████████████████
    for (int x = 0; x < MAP_SIZE; x++)
        for (int y = 0; y < MAP_SIZE; y++)
            map[x][y] = 0;

    generation_map(map);

    // ████████████████████████████████ MENU DEBUT ████████████████████████████████
    Mix_PlayMusic(musique1, -1);
    if (!afficher_menu_debut(renderer, font)) {
        // Nettoyage en cas de fermeture
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_CloseFont(font);
        Mix_FreeMusic(musique1);
        TTF_Quit(); IMG_Quit(); SDL_Quit();
        return 0;
    }
    Mix_FreeMusic(musique1);
    Mix_PlayMusic(musique2, -1);

    // ████████████████████████████████ PHASE D’ATTENTE AVANT VAGUE ████████████████████████████████
    waiting(renderer, window, font, &start, &vie, &argent, map, click_positions, &click_count, towers, &tower_count, icon_texture);

    // ████████████████████████████████ POSITION INITIALE ENNEMI ████████████████████████████████
    for (int i = 0; i < MAP_SIZE; i++) {
        if (map[i][0] == 1) {
            enemy_x = i;
            enemy_y = 0;
            break;
        }
    }

    // ████████████████████████████████ BOUCLE PRINCIPALE ████████████████████████████████
    int running = 1;
    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = 0;

            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x, y, final_x, final_y;
                SDL_GetMouseState(&x, &y);

                if (click_count < MAX_CLICKS && draw_click_zone(renderer, x, y, map, &final_x, &final_y, click_positions, click_count, argent)) {
                    click_positions[click_count][0] = final_x;
                    click_positions[click_count][1] = final_y;
                    click_count++;
                    argent -= 10;
                    towers[tower_count++] = (tower){.x = final_x, .y = final_y, .damage = 1, .range = 3 * TILE_SIZE, .speed_damage = 30, .cooldown = 0};
                    num_towers = tower_count;
                }
            }
        }

        // ████████████████████████████████ AFFICHAGE ████████████████████████████████
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        draw_map(renderer, map);
        for (int i = 0; i < tower_count; i++) {
            SDL_Rect dst = { towers[i].x, towers[i].y, TILE_SIZE, TILE_SIZE };
            SDL_RenderCopy(renderer, towers[i].tir_actif > 0 ? icon_texture1 : icon_texture, NULL, &dst);
        }

        draw_hud(renderer, font, vie, argent);

        // ████████████████████████████████ TOURELLES & ENNEMIS ████████████████████████████████
       deplacement_ennemi(renderer, &enemy_x, &enemy_y, map, &vie, enemies, current_enemy_count, towers, num_towers, &argent, current_enemy);

        tourelle(towers, tower_count, &current_enemy, enemy_x, enemy_y, tir_son);

        // ████████████████████████████████ ENNEMI MORT ████████████████████████████████
        if (current_enemy.life <= 0) {
            compteur++;
            argent += 5;
            current_enemy.life = 10 + compteur;
            waiting(renderer, window, font, &start, &vie, &argent, map, click_positions, &click_count, towers, &tower_count, icon_texture);
            for (int i = 0; i < MAP_SIZE; i++) {
                if (map[i][0] == 1) {
                    enemy_x = i;
                    enemy_y = 0;
                    break;
                }
            }
        }
        // ████████████████████████████████ ENNEMI ARRIVÉ EN BAS ████████████████████████████████
        else if (enemy_y == MAP_SIZE - 1) {
            compteur++;
            vie--;
            if (vie <= 0) {
                afficher_menu_fin(renderer, font);
                break;
            }
            current_enemy.life = 10 + compteur;
            waiting(renderer, window, font, &start, &vie, &argent, map, click_positions, &click_count, towers, &tower_count, icon_texture);
            for (int i = 0; i < MAP_SIZE; i++) {
                if (map[i][0] == 1) {
                    enemy_x = i;
                    enemy_y = 0;
                    break;
                }
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);

        if (vie < 1) {
            afficher_menu_fin(renderer, font);
            break;
        }
    }

    // ████████████████████████████████ NETTOYAGE FINAL ████████████████████████████████
    Mix_FreeMusic(musique2);
    Mix_FreeChunk(tir_son);

    Mix_HaltMusic();
    Mix_CloseAudio();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(icon_texture);
    SDL_DestroyTexture(icon_texture1);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}

