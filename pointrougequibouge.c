#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h> 
#include <time.h>

#define MAP_SIZE 100
#define TILE_SIZE 10
#define ENEMY_SIZE 5

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
            SDL_Rect rect = {x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}


//•••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••MAIN•••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
int main(int argc, char *argv[]) {
    int map[MAP_SIZE][MAP_SIZE];
    srand(time(NULL)); 
    int enemy_x;
    int enemy_y;
    


    
    
    
    
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••erreurs•••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
   // Initialiser SDL avec le sous-système vidéo
if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    // Si SDL_Init échoue (retourne ≠ 0), afficher l'erreur
    printf("Erreur SDL_Init : %s\n", SDL_GetError());
    return 1; // Terminer le programme avec une erreur
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

int running = 1; // Variable de contrôle pour la boucle principale (1 = continuer, 0 = arrêter)
SDL_Event event; // Structure pour gérer les événements (clavier, souris, fermeture, etc.)




// Boucle principale du programme
while (running) {
    // Boucle qui récupère tous les événements (s'il y en a) dans la file d'attente SDL

    while (SDL_PollEvent(&event)) {
        // Si un événement "fermeture de la fenêtre" est détecté
        if (event.type == SDL_QUIT) {
            running = 0; // On arrête la boucle principale => fermeture du programme
        }
    }
    
    
 //...........................NETTOYAGE FENETRE
    
    // Définir la couleur de fond : ici (0, 0, 0, 255) = noir opaque
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    
    // nettoyer l'écran
    SDL_RenderClear(renderer);




    //.............................. dessiner la map (sol et chemin)
    draw_map(renderer, map);
    
    
    
    
    
    
    //dessiner l'ennemi
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Rouge pour l'ennemi
SDL_Rect enemy_rect = { enemy_x * TILE_SIZE, enemy_y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
SDL_RenderFillRect(renderer, &enemy_rect);

// Déplacer l'ennemi petit à petit
static Uint32 last_move_time = 0;
 Uint32 now = SDL_GetTicks();

        if (now - last_move_time > 300) {
            // 1) Descendre tout droit
	    if (enemy_y < MAP_SIZE - 1 && map[enemy_x ][enemy_y+1] == 1) {
		enemy_y++;

	    // 2) Descendre en diagonale vers la droite
	    } else if (enemy_y < MAP_SIZE - 1 && enemy_x < MAP_SIZE - 1
		       && map[enemy_x + 1][enemy_y + 1] == 1) {
		enemy_x++;
		enemy_y++;

	    // 3) Descendre en diagonale vers la gauche
	    } else if (enemy_y < MAP_SIZE - 1 && enemy_x > 0
		       && map[enemy_x - 1][enemy_y + 1] == 1) {
		enemy_x--;
		enemy_y++;

	    // 4) Sinon, se déplacer latéralement si possible
	    } else if (enemy_x > 0 && map[enemy_x-1][enemy_y] == 1) {
		enemy_x--;
	    } else if (enemy_x < MAP_SIZE - 1 && map[enemy_x + 1][enemy_y] == 1) {
		enemy_x++;
	    }
	}


    // Présente à l'écran tout ce qu'on a dessiné depuis le dernier Clear()
    SDL_RenderPresent(renderer);

    //••••••••••••••••••••••••••••••••••••DELAI ENTRE CHAQUE BOUCLE••••••••••••••••••••••••••••••••••
    SDL_Delay(100);
}

// Quand la boucle est finie, nettoyage :
SDL_DestroyRenderer(renderer); // libère la mémoire
SDL_DestroyWindow(window);     // Détruit la fenêtre SDL
SDL_Quit();                    // Arrête proprement SDL




return 0; 
}
