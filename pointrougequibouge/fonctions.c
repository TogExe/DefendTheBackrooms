

#include "fonctions.h"



//••••••••••••••••••••••••••••••••••••••••••••••••••••••••FONCTIONS_MAP••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••



    SDL_Color get_sdl_color_for_tile(char tile) {
    switch (tile) {
        // Forest and Grassland biomes (greens)
        case 'g': return (SDL_Color){100, 180, 100, 255};    // Grass
        case 'f': return (SDL_Color){120, 200, 120, 255};    // Light Grass
        case 'a': return (SDL_Color){140, 220, 140, 255};    // Forest or grasslands
        case 'b': return (SDL_Color){130, 200, 130, 255};    // Lush grass, grassland mix
        case 'c': return (SDL_Color){160, 220, 160, 255};    // Fertile land and grass
        case 'd': return (SDL_Color){140, 200, 140, 255};    // Lush or rich forest
        case 'e': return (SDL_Color){130, 180, 130, 255};    // Dry forest or field
        case 'h': return (SDL_Color){80, 60, 40, 255};       // Wetlands/mud
        case 'i': return (SDL_Color){60, 50, 30, 255};       // Dark soil (swamp)
        case 'j': return (SDL_Color){100, 140, 80, 255};     // Wet grassland or transition
        case 'k': return (SDL_Color){120, 160, 100, 255};    // Bog or swamp
        case 'l': return (SDL_Color){110, 150, 90, 255};     // Swamp transition

        // Desert and Rocky biomes (yellows, oranges, browns)
        case 'm': return (SDL_Color){200, 180, 100, 255};    // Desert scrub or arid land
        case 'n': return (SDL_Color){180, 150, 100, 255};    // Rocky outcrops
        case 'o': return (SDL_Color){210, 180, 110, 255};    // Sand dunes and rocky desert
        case 'p': return (SDL_Color){230, 200, 120, 255};    // Dry desert, scrub, or dunes
        case 'q': return (SDL_Color){180, 160, 130, 255};    // Rocky desert terrain
        case 'r': return (SDL_Color){160, 140, 110, 255};    // Rocky desert with sparse vegetation
        case 's': return (SDL_Color){190, 160, 120, 255};    // Desert cliffs or rocky transition

        // Frozen biomes (snow, ice)
        case 't': return (SDL_Color){210, 220, 255, 255};    // Snow and ice terrain
        case 'u': return (SDL_Color){190, 200, 255, 255};    // Ice plains or frozen lakes
        case 'v': return (SDL_Color){180, 190, 255, 255};    // Glacial terrain
        case 'w': return (SDL_Color){160, 170, 255, 255};    // Snow-covered mountains

        // Special transitional or mixed biomes
        case 'x': return (SDL_Color){180, 160, 255, 255};    // Mixed biome, desert transitioning to frozen
        case 'y': return (SDL_Color){170, 150, 255, 255};    // Mountainous regions with forests and snow
        case 'z': return (SDL_Color){190, 170, 255, 255};    // Desert transition to mountains

        // Empty space (black background)
        case ' ': return (SDL_Color){0, 0, 0, 255};          // Empty space (black background)

        default: return (SDL_Color){255, 255, 255, 255};     // Fallback (white)
    }
}

void generation_map(int map[MAP_SIZE][MAP_SIZE]) {
    int x = rand() % MAP_SIZE; 

    for (int y = 0; y < MAP_SIZE; y++) {
        map[x][y] = 1; // Marquer chemin

        // Choisir direction aléatoire (gauche, droite ou tout droit)
        int direction = rand() % 3; // 0 = gauche, 1 = tout droit, 2 = droite

        if (direction == 0 && x > 0) {
            x--; // Aller à gauche
        } else if (direction == 2 && x < MAP_SIZE - 1) {
            x++; // Aller à droite
        }
        // Sinon continuer tout droit
    }
}

void draw_map(SDL_Renderer *renderer, int map[MAP_SIZE][MAP_SIZE]) {
    for (int x = 0; x < MAP_SIZE; x++) {
        for (int y = 0; y < MAP_SIZE; y++) {
            if (map[x][y] == 0) {
                SDL_SetRenderDrawColor(renderer, 34, 139, 34, 255); // Vert pour sol
                
            /* AJOUTER switch case pour un caractère "a" = une couleur 
            else if(map[x][y]== "a"){
            	
            	
            */
           
            } else {
                SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255); // Marron pour chemin
              }
            if (y==MAP_SIZE -1 && map[x][y]==1){
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
              }
            
            SDL_Rect rect = {x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}




// Renvoie 1 si une position a été trouvée et placée dans out_x/out_y
int draw_click_zone(SDL_Renderer *renderer, int mouse_x, int mouse_y, int map[MAP_SIZE][MAP_SIZE],
                    int *out_x, int *out_y, int click_positions[MAX_CLICKS][2], int click_count, int argent){
    (void)renderer;
    int tile_x = mouse_x / TILE_SIZE;
    int tile_y = mouse_y / TILE_SIZE;
    
    
    
    if (argent < 10){ //plus tard : towern.cost typed tower d'indice n pour savoir son coût)
      return 0;
    }
    
    if (tile_x < 0 || tile_x >= MAP_SIZE || tile_y < 0 || tile_y >= MAP_SIZE) {
        return 0;
    }

    if (map[tile_x][tile_y] != 0 && map[tile_x][tile_y] != 1) {
    return 0;

    }

    int best_x = -1, best_y = -1;
    double min_dist = 1e9;

    for (int radius = 1; radius <= 3; radius++) {
        for (int dx = -radius; dx <= radius; dx++) {
            for (int dy = -radius; dy <= radius; dy++) {
                int nx = tile_x + dx;
                int ny = tile_y + dy;

                if (nx >= 0 && nx < MAP_SIZE && ny >= 0 && ny < MAP_SIZE && map[nx][ny] == 0) {
                    // Vérifier qu'aucune tourelle n'est déjà placée ici
                    int pixel_x = nx * TILE_SIZE;
                    int pixel_y = ny * TILE_SIZE;
                    int already_placed = 0;
                    for (int i = 0; i < click_count; i++) {
                        int cx = click_positions[i][0] / TILE_SIZE * TILE_SIZE;
                        int cy = click_positions[i][1] / TILE_SIZE * TILE_SIZE;
                        if (cx == pixel_x && cy == pixel_y) {
                            already_placed = 1;
                            break;
                        }
                    }

                    if (!already_placed) {
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
    }

    if (best_x != -1 && best_y != -1) {
        *out_x = best_x * TILE_SIZE;
        *out_y = best_y * TILE_SIZE;
        return 1;
    }

    return 0;
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

    SDL_Rect dst = { x, y, TILE_SIZE, TILE_SIZE }; // Ajuste selon la taille souhaitée
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

int afficher_menu_debut(SDL_Renderer *renderer, TTF_Font *font) {
    SDL_Event event;
    int menu_running = 1;
    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Color bgColor = {0, 0, 0, 255};

    SDL_Surface *surface = TTF_RenderText_Blended(font, "CLIQUEZ POUR COMMENCER", textColor);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    
    int texW = 0, texH = 0;
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
    SDL_Rect dstrect = {MAP_SIZE * TILE_SIZE / 2 - texW / 2, MAP_SIZE * TILE_SIZE / 2 - texH / 2, texW, texH};

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

int afficher_menu_fin(SDL_Renderer *renderer, TTF_Font *font) {
    
    SDL_Event event;
    int menu_running = 1;
    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Color bgColor = {0, 0, 0, 255};

    SDL_Surface *surface = TTF_RenderText_Blended(font, "GAME OVER", textColor);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    
    int texW = 0, texH = 0;
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
    SDL_Rect dstrect = {MAP_SIZE * TILE_SIZE / 2 - texW / 2, MAP_SIZE * TILE_SIZE / 2 - texH / 2, texW, texH};

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
}




void deplacement_ennemi(SDL_Renderer *renderer, int* enemy_x, int* enemy_y, int map[MAP_SIZE][MAP_SIZE], int* vie, enemy* enemies, int num_enemies, tower* towers, int num_towers, int* argent, enemy current_enemy){

    
    // Afficher l'image de l'ennemi à la position actuelle
    affiche_image(renderer, "crabe.png", (*enemy_x) * TILE_SIZE, (*enemy_y) * TILE_SIZE);

    // Déplacement de l'ennemi
    static Uint32 last_move_time = 0;
    Uint32 now = SDL_GetTicks();

    if (now - last_move_time > 300) {
        if ((*enemy_y) < MAP_SIZE - 1 && map[*enemy_x][*enemy_y + 1] == 1) {
            (*enemy_y)++;
        } else if ((*enemy_y) < MAP_SIZE - 1 && (*enemy_x) < MAP_SIZE - 1 && map[*enemy_x + 1][*enemy_y + 1] == 1) {
            (*enemy_x)++; (*enemy_y)++;
        } else if ((*enemy_y) < MAP_SIZE - 1 && (*enemy_x) > 0 && map[*enemy_x - 1][*enemy_y + 1] == 1) {
            (*enemy_x)--; (*enemy_y)++;
        } else if ((*enemy_x) > 0 && map[*enemy_x - 1][*enemy_y] == 1) {
            (*enemy_x)--;
        } else if ((*enemy_x) < MAP_SIZE - 1 && map[*enemy_x + 1][*enemy_y] == 1) {
            (*enemy_x)++;
        }

        // Vérification si l'ennemi atteint le bas de la carte
        if ((*enemy_y) == MAP_SIZE  && map[*enemy_x][*enemy_y] == 1) {
            (*vie)--;
            
            // Afficher que le joueur a perdu une vie
            printf("Aie ! Le joueur a perdu une vie. Vie restante du joueur : %d\n", *vie);
            

            // Revenir au premier point du chemin (haut)
            for (int i = 0; i < MAP_SIZE; i++) {
                if (map[i][0] == 1) {
                    *enemy_x = i;
                    *enemy_y = 0;
                    break;
                }
            }
        }

        // Vérification des tours qui touchent l'ennemi
        for (int i = 0; i < num_towers; i++) {
            if (towers[i].range >= sqrt(pow(*enemy_x - towers[i].x, 2) + pow(*enemy_y - towers[i].y, 2))) {
                // L'ennemi est dans la portée de la tour, il prend des dégâts
                current_enemy.life -= towers[i].damage;
                
                // Afficher l'ennemi touché et sa vie restante
                printf("Ennemi touché par une tour ! Vie restante de l'ennemi : %d\n", current_enemy.life);

                
            }
        }

        last_move_time = now;
    }
}

/*
void tourelle(tower* towers, int tower_count, enemy* current_enemy, int enemy_x, int enemy_y, Mix_Chunk *tir_son) {
    // Convertir les coordonnées de l'ennemi en pixels (au centre de la tuile)
    int enemy_px = enemy_x * TILE_SIZE + TILE_SIZE / 2;
    int enemy_py = enemy_y * TILE_SIZE + TILE_SIZE / 2;

    for (int i = 0; i < tower_count; i++) {
        // Gérer le cooldown de la tour
        if (towers[i].cooldown > 0) {
            towers[i].cooldown--;
            continue;
        }

        // Calculer la distance entre la tourelle et l'ennemi
        int tower_cx = towers[i].x + TILE_SIZE / 2;
        int tower_cy = towers[i].y + TILE_SIZE / 2;

        int dx = enemy_px - tower_cx;
        int dy = enemy_py - tower_cy;
        int distance_squared = dx * dx + dy * dy;

        // Vérifier si l'ennemi est dans la portée de la tourelle (en pixels)
        if (distance_squared <= towers[i].range * towers[i].range) {
            // Appliquer les dégâts
            current_enemy->life -= towers[i].damage;
            towers[i].cooldown = towers[i].speed_damage; // Réinitialiser le cooldown
            towers[i].tir_actif = 3;

            printf("Tour %d a tiré ! Vie ennemie = %d\n", i, current_enemy->life);
            Mix_PlayChannel(-1, tir_son, 0);

        }
    }
}
*/
void tourelle(tower* towers, int tower_count, enemy* current_enemy, int enemy_x, int enemy_y, Mix_Chunk *tir_son) {
    int enemy_px = enemy_x * TILE_SIZE + TILE_SIZE / 2;
    int enemy_py = enemy_y * TILE_SIZE + TILE_SIZE / 2;

    for (int i = 0; i < tower_count; i++) {
        // Réduction de tir_actif s’il est actif
        if (towers[i].tir_actif > 0) {
            towers[i].tir_actif--;
        }

        if (towers[i].cooldown > 0) {
            towers[i].cooldown--;
            continue;
        }

        int tower_cx = towers[i].x + TILE_SIZE / 2;
        int tower_cy = towers[i].y + TILE_SIZE / 2;

        int dx = enemy_px - tower_cx;
        int dy = enemy_py - tower_cy;
        int distance_squared = dx * dx + dy * dy;

        if (distance_squared <= towers[i].range * towers[i].range) {
            current_enemy->life -= towers[i].damage;
            towers[i].cooldown = towers[i].speed_damage;
            towers[i].tir_actif = 10; // 10 frames de texture différente

            printf("Tour %d a tiré ! Vie ennemie = %d\n", i, current_enemy->life);
            Mix_PlayChannel(-1, tir_son, 0);
        }
    }
}




void waiting(SDL_Renderer* renderer, SDL_Window* window, TTF_Font* font, int* start, int* vie, int* argent,
             int map[MAP_SIZE][MAP_SIZE], int click_positions[MAX_CLICKS][2], int* click_count,
             tower towers[MAX_TOWERS], int* tower_count, SDL_Texture* icon_texture) {

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
                        draw_click_zone(renderer, mx, my, map, &final_x, &final_y, click_positions, *click_count, *argent)) {

                        click_positions[*click_count][0] = final_x;
                        click_positions[*click_count][1] = final_y;
                        (*click_count)++;
                        *argent -= 10;

                        towers[*tower_count].x = final_x;
                        towers[*tower_count].y = final_y;
                        towers[*tower_count].damage = 1;
                        towers[*tower_count].range = 3 * TILE_SIZE;
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

        draw_map(renderer, map);

        // Dessiner les tourelles déjà posées
        for (int i = 0; i < *click_count; i++) {
            int x = click_positions[i][0];
            int y = click_positions[i][1];
            SDL_Rect dst = { x, y, TILE_SIZE, TILE_SIZE };
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
}



/*void widget(SDL_Window **window, SDL_Renderer *renderer) {
    int w, h;
    SDL_GetWindowSize(*window, &w, &h);

    int new_height =10 * TILE_SIZE;

    // Agrandit la fenêtre
    SDL_SetWindowSize(*window, w, new_height);

    // Charger les images des tourelles
    SDL_Surface *surf1 = IMG_Load("tower.png");
    SDL_Surface *surf2 = IMG_Load("turret2.png");
    SDL_Surface *surf3 = IMG_Load("turret3.png");

    if (!surf1 || !surf2 || !surf3) {
        printf("Erreur de chargement des images de tourelles : %s\n", IMG_GetError());
        return;
    }

    SDL_Texture *tex1 = SDL_CreateTextureFromSurface(renderer, surf1);
    SDL_Texture *tex2 = SDL_CreateTextureFromSurface(renderer, surf2);
    SDL_Texture *tex3 = SDL_CreateTextureFromSurface(renderer, surf3);

    SDL_FreeSurface(surf1);
    SDL_FreeSurface(surf2);
    SDL_FreeSurface(surf3);

    if (!tex1 || !tex2 || !tex3) {
        printf("Erreur de création des textures\n");
        return;
    }

    // Position des tourelles dans le HUD
    SDL_Rect turretRects[3] = {
        { w / 4 - TILE_SIZE / 2, h + TILE_SIZE, TILE_SIZE, TILE_SIZE },
        { w / 2 - TILE_SIZE / 2, h + TILE_SIZE, TILE_SIZE, TILE_SIZE },
        { 3 * w / 4 - TILE_SIZE / 2, h + TILE_SIZE, TILE_SIZE, TILE_SIZE }
    };

    // Affichage des tourelles
    SDL_RenderCopy(renderer, tex1, NULL, &turretRects[0]);
    SDL_RenderCopy(renderer, tex2, NULL, &turretRects[1]);
    SDL_RenderCopy(renderer, tex3, NULL, &turretRects[2]);

    // Ne pas oublier d’actualiser le rendu
    SDL_RenderPresent(renderer);

    // Libération des textures
    SDL_DestroyTexture(tex1);
    SDL_DestroyTexture(tex2);
    SDL_DestroyTexture(tex3);
}
*/

