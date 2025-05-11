#include "fichier_h/generation.h"

#include "fichier_h/gestion_enemies.h"
#include "fichier_h/gestion_tourelles.h"
#include "fichier_h/affichages_menus_widgets.h"
#include "fichier_h/constantes_structures.h"

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••FONCTIONS_MAP••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••



SDL_Color get_sdl_color_for_tile(char tile) {
    switch (tile) {
        // Forest and Grassland biomes (greens)
        case 'g': return (SDL_Color){100, 180, 100, 255};    // Grass
       	// case 'g': return (SDL_Color){100, 0, 0, 255};    // Grass
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
        case ' ': return (SDL_Color){130, 200, 130, 255};          // Empty space (black background)

        default: return (SDL_Color){255, 255, 255, 255};     // Fallback (white)
    }
}

void generation_map(int side, char ** map) {
    int x = rand() % side; 

    for (int y = 0; y < side; y++) {
        map[x][y] = ' '; // Marquer chemin

        // Choisir direction aléatoire (gauche, droite ou tout droit)
        int direction = rand() % 3; // 0 = gauche, 1 = tout droit, 2 = droite

        if (direction == 0 && x > 0) {
            x--; // Aller à gauche
        } else if (direction == 2 && x < side - 1) {
            x++; // Aller à droite
        }
        // Sinon continuer tout droit
    }
}




void draw_map(SDL_Renderer *renderer, int side, int tile_size,  char** map) {
    for (int x = 0; x < side; x++) {
        for (int y = 0; y < side; y++) {
            if (map[x][y] != '_') {
               SDL_Color bruh= get_sdl_color_for_tile(map[x][y]);
				//SDL_Color bruh= (SDL_Color){110,180,90,225};
                SDL_SetRenderDrawColor(renderer, bruh.r, bruh.g, bruh.b, 255);
                                  // Vert pour sol
            /* AJOUTER switch case pour un caractère "a" = une couleur //NUH UH ! ! ! 
            else if(map[x][y]== "a"){
            	
            	
            */
           
            } else {
            	SDL_Color bruh= get_sdl_color_for_tile(map[x][y]);
                SDL_SetRenderDrawColor(renderer, bruh.r, bruh.g, bruh.b, 255); // Marron pour chemin
              }
            if (y==side -1 && map[x][y]==1){
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
             }
            
            SDL_Rect rect = {x * tile_size, y * tile_size, tile_size, tile_size};
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}


int find_start(int side, char** map) {
    for (int i = 0; i < side; i++)
        if (map[i][0] == ' ') return i;
    return -1;
}
