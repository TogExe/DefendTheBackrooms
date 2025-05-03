
#include "fonctions.h"




//•••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••MAIN•••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
int main(int argc, char *argv[]) {
    
    SDL_Event e;
    int map[MAP_SIZE][MAP_SIZE];
    srand(time(NULL)); 
    int enemy_x=0;
    int enemy_y=0;
    int click_positions[MAX_CLICKS][2];
    int click_count = 0;
    int vie = 2;
    int argent = 50;
    int tower_count = 0;
    enemy current_enemy;
    current_enemy.life = 10;
    current_enemy.speed = 1;
    tower towers[MAX_TOWERS];
    tower t;
    int num_towers;
    int start = 0;
    int compteur=0; 
    
    
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


if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
    printf("Erreur SDL_mixer : %s\n", Mix_GetError());
    return 1;
}


Mix_Music* musique1 = Mix_LoadMUS("menu.mp3");
if (!musique1) {
    printf("Erreur chargement musique : %s\n", Mix_GetError());
}

Mix_Music* musique2 = Mix_LoadMUS("play.mp3");
if (!musique2) {
    printf("Erreur chargement musique : %s\n", Mix_GetError());
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
    
    

SDL_Event event; // Structure pour gérer les événements (clavier, souris, fermeture, etc.)


  
int running = 1; // Variable de contrôle pour la boucle principale (1 = continuer, 0 = arrêter)




//••••••••••••••••••••••••••••••••••••••••••MENU••••••••••••••••••••••••••••••••••••••••••••••••


//••••••••••••••••••••••••••••••••••••••trucs_initiaux••••••••••••••••••••••••••••••••••••••••
Mix_PlayMusic(musique1, -1);  // -1 = boucle infinie

if (!afficher_menu_debut(renderer, font)) {
    // L'utilisateur a fermé la fenêtre avant de commencer
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    Mix_FreeMusic(musique1);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}


Mix_FreeMusic(musique1);
Mix_PlayMusic(musique2, -1);

waiting(renderer, window, font, &start, &vie, &argent, map, click_positions, &click_count, towers, &tower_count, icon_texture);




//•••••••••••••••••••••••••••••POSITION INIT ENEMY••••••••••••••••••••••••••••

for (int i=0; i< MAP_SIZE ; i++){
  if(map[i][0]==1){
    enemy_x=i;
    enemy_y=0;
    break;
    }
  } 
  

// Boucle principale du programme
while (running) {
    // Boucle qui récupère tous les événements (s'il y en a) dans la file d'attente SDL
    
    //widget(&window, renderer);

    //if (vie<=0){
      
      

    while (SDL_PollEvent(&event)) {
        // Si un événement "fermeture de la fenêtre" est détecté
        if (event.type == SDL_QUIT) {
            running = 0; // On arrête la boucle principale => fermeture du programme
        }
      
     
            //•••••••••••••••••••••••••••••••••••••clic souris•••••••••••••••••••••••••••••••••••••

        if (event.type == SDL_MOUSEBUTTONDOWN) {
            int x, y;
            SDL_GetMouseState(&x, &y);
            printf("Clic détecté en (%d, %d)\n", x, y);
            

            int final_x, final_y;
            if (click_count < MAX_CLICKS && draw_click_zone(renderer, x, y, map, &final_x, &final_y, click_positions, click_count, argent)) {
                click_positions[click_count][0] = final_x;
                click_positions[click_count][1] = final_y;
                click_count++;
                argent-=10 ; //plus tard towern.cost de type tower d'indice n pour le coût
                towers[tower_count].x = final_x;
                towers[tower_count].y = final_y;
                towers[tower_count].damage = 1;
                towers[tower_count].range = 3 * TILE_SIZE;  // rayon en pixels
                towers[tower_count].speed_damage = 30;         // frames entre chaque tir
                towers[tower_count].cooldown = 0;
                tower_count++;
                num_towers = tower_count;


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
        int x = click_positions[i][0];
        int y = click_positions[i][1];
        SDL_Rect dst = { x, y, TILE_SIZE, TILE_SIZE };
        SDL_RenderCopy(renderer, icon_texture, NULL, &dst);
        
    }
    
    draw_hud(renderer, font, vie, argent);


   
    


    //••••••••••••••••••••••••ENNEMI/TOURELLE••••••••••••••••••••••
    
    


    deplacement_ennemi(renderer, &enemy_x, &enemy_y, map, &vie, &current_enemy, towers, num_towers, &argent);
    tourelle(towers, tower_count, &current_enemy, enemy_x, enemy_y);

    
      
 /*  deplacement_ennemi(renderer, &enemy_x, &enemy_y, map, &vie, &current_enemy, towers, num_towers, &argent);


    // Gestion des dégâts infligés par les tours
    
    tourelle(towers, tower_count, &current_enemy, enemy_x, enemy_y);
*/
    
    
    
    

    // Si l'ennemi est tué, on le réinitialise
    
    if (current_enemy.life <= 0) {
          
        compteur+=1;
        printf("Ennemi tué !\n");
        // Réinitialiser l'ennemi et sa position
        current_enemy.life = compteur + 10;
        argent+=5;
        // Remettre au début du chemin :
        SDL_Delay(16);
        waiting(renderer, window, font, &start, &vie, &argent, map, click_positions, &click_count, towers, &tower_count, icon_texture);

        for (int i = 0; i < MAP_SIZE; i++) {
            if (map[i][0] == 1) {
                enemy_x = i;
                enemy_y = 0;
                break;
            }
        }
    }



    



    // Présente à l'écran tout ce qu'on a dessiné depuis le dernier Clear()
    SDL_RenderPresent(renderer);

    //••••••••••••••••••••••••••••••••••••DELAI ENTRE CHAQUE BOUCLE••••••••••••••••••••••••••••••••••
    
    SDL_Delay(16);
   
   

     if (vie < 1) {
    afficher_menu_fin(renderer, font);
    Mix_FreeMusic(musique2);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    Mix_FreeMusic(musique2);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
    }

    
  
    
}

// Quand  boucle est finie, nettoyage :

Mix_HaltMusic();  // pour l'arrêter à un moment donné


Mix_CloseAudio();

TTF_CloseFont(font);
TTF_Quit();


SDL_DestroyRenderer(renderer); // libère la mémoire
SDL_DestroyTexture(icon_texture);

SDL_DestroyWindow(window);     // Détruit la fenêtre SDL
SDL_Quit();                    // Arrête proprement SDL




return 0; 
}
