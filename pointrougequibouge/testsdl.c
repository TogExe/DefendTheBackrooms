
#include "fonctions.h"




//•••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••MAIN•••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
int main(int argc, char *argv[]) {
    
    SDL_Event e;
    int map[MAP_SIZE][MAP_SIZE];
    //char map[MAP_SIZE][MAP_SIZE] ;
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
    
    TirEffet tirs[100];
    int tir_count = 0;
    
    



    
    
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


//•••••••••••••••••••••••••••••••••••••••••••••••••••MUSIC••••••••••••••••••••••••••••••••••••••••••••

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

Mix_Chunk *tir_son = Mix_LoadWAV("shoot.wav");
if (!tir_son) {
    printf("Erreur chargement du son : %s\n", Mix_GetError());
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


SDL_Surface *surface_icon = IMG_Load("tmonkey.png");
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



SDL_Surface *surface_icon1 = IMG_Load("ninja.png");
if (!surface_icon1) {
    printf("Erreur chargement image : %s\n", IMG_GetError());
    SDL_Quit();
    return 1;
}
SDL_Texture *icon_texture1 = SDL_CreateTextureFromSurface(renderer, surface_icon1);
SDL_FreeSurface(surface_icon1);
if (!icon_texture1) {
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
                towers[tower_count].speed_damage = 30;        // frame par tir (non fonctionnel/ depend de la boucle)
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
    for (int i = 0; i < tower_count; i++) {
        SDL_Rect dst = { towers[i].x, towers[i].y, TILE_SIZE, TILE_SIZE };
        if (towers[i].tir_actif > 0) {
            SDL_RenderCopy(renderer, icon_texture1, NULL, &dst); // texture "attaque"
        } else {
            SDL_RenderCopy(renderer, icon_texture, NULL, &dst);  // texture normale
        }
    }

        
    
    
    draw_hud(renderer, font, vie, argent);


   
    


    //••••••••••••••••••••••••ENNEMI/TOURELLE••••••••••••••••••••••
    
    

    
    deplacement_ennemi(renderer, &enemy_x, &enemy_y, map, &vie, &current_enemy, towers, num_towers, &argent);
    tourelle(towers, tower_count, &current_enemy, enemy_x, enemy_y, tir_son);
    
    //TOURELLE
    
    

    
    

   //ENNEMY
   
   // Si l'ennemi est tué
if (current_enemy.life <= 0) {
    compteur += 1;
    printf("Ennemi tué !\n");
    argent += 5;  // Récompense
    current_enemy.life = 10 + compteur;  // Vie augmentée à chaque tour
    waiting(renderer, window, font, &start, &vie, &argent, map, click_positions, &click_count, towers, &tower_count, icon_texture);

    // Réinitialiser la position de l’ennemi au début du chemin
    for (int i = 0; i < MAP_SIZE; i++) {
        if (map[i][0] == 1) {
            enemy_x = i;
            enemy_y = 0;
            break;
        }
    }
}

// Si l’ennemi atteint le bas de la map
else if (enemy_y == MAP_SIZE - 1 ) {
    compteur += 1;
    vie-- ;
    if (vie <=0){
        afficher_menu_fin(renderer, font);
    Mix_FreeMusic(musique2);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
    }
    printf("Ennemi arrivé en bas !\n");
    current_enemy.life = 10 + compteur;
    
    if (vie>0){
      waiting(renderer, window, font, &start, &vie, &argent, map, click_positions, &click_count, towers, &tower_count, icon_texture);
    }
    

    // Réinitialiser l’ennemi au début du chemin
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
    printf("aaaaaaaaaaaaaaaaaaa\n");
    afficher_menu_fin(renderer, font);
    printf("bbbbbbbbbbbbbbbbbbbbbbbbbbb\n");
    //Mix_FreeMusic(musique2);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
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
