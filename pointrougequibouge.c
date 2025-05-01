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
#define MAX_CLICKS 1000

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••FONCTIONS_MAP••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••

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




void draw_click_zone(SDL_Renderer *renderer, int mouse_x, int mouse_y, int map[MAP_SIZE][MAP_SIZE]) {
    // Convertir coordonnées pixels → indices de case
    int tile_x = mouse_x / TILE_SIZE;
    int tile_y = mouse_y / TILE_SIZE;

    // Si on clique sur un chemin (valeur 1), chercher une case sol (0) autour
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
                    // Coordonnées pixels du centre de cette case
                    int px = nx * TILE_SIZE + TILE_SIZE / 2;
                    int py = ny * TILE_SIZE + TILE_SIZE / 2;

                    // Distance au clic (en pixels)
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

    // Si une case libre trouvée, l'utiliser
    if (best_x != -1 && best_y != -1) {
        tile_x = best_x;
        tile_y = best_y;
    } else {
        return; // Rien trouvé, ne rien dessiner
    }
}


    // Aligner parfaitement sur une case de la grille (multiples de TILE_SIZE)
    int aligned_x = tile_x * TILE_SIZE;
    int aligned_y = tile_y * TILE_SIZE;

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Rouge
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

    SDL_Rect dst = { x, y, 32, 32 }; // Ajuste selon la taille souhaitée
    SDL_RenderCopy(renderer, texture, NULL, &dst);
    SDL_DestroyTexture(texture);

  // Détruire après affichage (si temporaire)
}







//••••••••••••••••••••••••••••••••••FONCTTION WIDGET VIE/ARGENT•••••••••••••••••••••••••••••••••••••••••••••••

void draw_hud(SDL_Renderer *renderer, TTF_Font *font, int points_de_vie, int argent) {
    SDL_Color textColor = {255, 255, 255, 255}; // Blanc
    SDL_Rect background = {0, 0, 150, 60}; // Rectangle noir semi-transparent en haut à gauche

    // Dessiner le fond
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200); // Noir semi-transparent
    SDL_RenderFillRect(renderer, &background);

    // Préparer les textes
    char buffer[64];
    
    // PV
    snprintf(buffer, sizeof(buffer), "PV : %d", points_de_vie);
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


//•••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••MAIN•••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
int main(int argc, char *argv[]) {
    int map[MAP_SIZE][MAP_SIZE];
    srand(time(NULL)); 
    int enemy_x;
    int enemy_y;
    int click_positions[MAX_CLICKS][2];
    int click_count = 0;
    int vie = 10;
    int argent = 250;
    
    
    
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••erreurs•••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
   // Initialiser SDL avec le sous-système vidéo
if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    // Si SDL_Init échoue (retourne ≠ 0), afficher l'erreur
    printf("Erreur SDL_Init : %s\n", SDL_GetError());
    return 1; // Terminer le programme avec une erreur
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


// Créer une fenêtre SDL
SDL_Window *window = SDL_CreateWindow(
    "Map 2D Aleatoire",         // Titre de la fenêtre
    SDL_WINDOWPOS_CENTERED,     // Position X (centrée sur l'écran)
    SDL_WINDOWPOS_CENTERED,     // Position Y (centrée sur l'écran)
    MAP_SIZE * TILE_SIZE,        // Largeur de la fenêtre (ex: 20 * 20 = 400 pixels)
    MAP_SIZE * TILE_SIZE,        // Hauteur de la fenêtre (idem)
    0                            // Flags (ici 0 = pas d'option spéciale)
);

// Vérifier que la fenêtre a bien été créée
if (!window) {
    // Si la création de la fenêtre échoue, afficher l'erreur
    printf("Erreur SDL_CreateWindow : %s\n", SDL_GetError());
    SDL_Quit(); // Nettoyer SDL avant de quitter
    return 1;   // Quitter avec une erreur
}


//......................................création renderer............................................


// Créer un "renderer" pour dessiner dans la fenêtre
SDL_Renderer *renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED     
);
// La fenêtre dans laquelle on veut dessiner
// Choisir automatiquement le meilleur driver de rendu
 // Utiliser l'accélération matérielle (GPU) si possible
 
 

// Vérifier que le renderer a bien été créé
if (!renderer) {
    // Si la création du renderer échoue, afficher l'erreur
    printf("Erreur SDL_CreateRenderer : %s\n", SDL_GetError());
    SDL_DestroyWindow(window); // Détruire la fenêtre (on l'avait créée)
    SDL_Quit();                // Nettoyer SDL
    return 1;                  // Quitter avec une erreur
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


//•••••••••••••••••••••••••••••••••••••••••••••••••••••••••••GENERATION••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
   
   // Initialiser map en sol
    for (int x = 0; x < MAP_SIZE; x++) { 
        for (int y = 0; y < MAP_SIZE; y++) {
            map[x][y] = 0;
        }
    }
    
    
// CREATION CHEMIN
    generation_map(map); 
    
    
    
  for (int i=0; i< MAP_SIZE ; i++){
  if(map[i][0]==1){
    enemy_x=i;
    enemy_y=0;
    break;
    }
  }

SDL_Event event; // Structure pour gérer les événements (clavier, souris, fermeture, etc.)
int running = 1; // Variable de contrôle pour la boucle principale (1 = continuer, 0 = arrêter)



// Boucle principale du programme
while (running) {
    // Boucle qui récupère tous les événements (s'il y en a) dans la file d'attente SDL

    while (SDL_PollEvent(&event)) {
        // Si un événement "fermeture de la fenêtre" est détecté
        if (event.type == SDL_QUIT) {
            running = 0; // On arrête la boucle principale => fermeture du programme
        }
        
            //•••••••••••••••••••••••••••••••••••••clic souris•••••••••••••••••••••••••••••••••••••
    // Clic de souris
        if (event.type == SDL_MOUSEBUTTONDOWN) {
          int x, y;
          SDL_GetMouseState(&x, &y);

          printf("Clic détecté en (%d, %d)\n", x, y);
          

            if (click_count < MAX_CLICKS) {
              click_positions[click_count][0] = x;
              click_positions[click_count][1] = y;
              click_count++;
            }
            
      }
}
    
    
 //...........................NETTOYAGE FENETRE
    
    // Définir la couleur de fond : ici (0, 0, 0, 255) = noir opaque
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    
    
    
    // nettoyer l'écran
    SDL_RenderClear(renderer);

    //.............................. dessiner la map (sol et chemin)
    draw_map(renderer, map);
    
    //....................................tourelle au clic............................
    for (int i = 0; i < click_count; i++) {
    int x = click_positions[i][0] - 16;
    int y = click_positions[i][1] - 16;
    SDL_Rect dst = { x, y, 32, 32 };
    SDL_RenderCopy(renderer, icon_texture, NULL, &dst);
  }
    
    draw_hud(renderer, font, vie, argent);

    


    
    
    //dessiner l'ennemi
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Rouge pour l'ennemi
    SDL_Rect enemy_rect = { enemy_x * TILE_SIZE, enemy_y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
    SDL_RenderFillRect(renderer, &enemy_rect);

// Déplacer l'ennemi petit à petit
static Uint32 last_move_time = 0;
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


    // Présente à l'écran tout ce qu'on a dessiné depuis le dernier Clear()
    SDL_RenderPresent(renderer);

    //••••••••••••••••••••••••••••••••••••DELAI ENTRE CHAQUE BOUCLE••••••••••••••••••••••••••••••••••
    SDL_Delay(100);
}

// Quand la boucle est finie, nettoyage :
TTF_CloseFont(font);
TTF_Quit();

SDL_DestroyRenderer(renderer); // libère la mémoire
SDL_DestroyTexture(icon_texture);

SDL_DestroyWindow(window);     // Détruit la fenêtre SDL
SDL_Quit();                    // Arrête proprement SDL




return 0; 
}
