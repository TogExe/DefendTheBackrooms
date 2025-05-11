#include "fichier_h/gestion_enemies.h"
#include "fichier_h/gestion_tourelles.h"
#include "fichier_h/affichages_menus_widgets.h"
#include "fichier_h/generation.h"
#include "fichier_h/constantes_structures.h"
#include "headers/generator.h"
#include "gui.h"
#include "fichier_h/save.h"
#define SAVE "save.json"

void initialize(char *** grid1, char *** map1,Tower ** towers, Enemy ** enemies,
		 int * click_count, int * argent, int * current_enemy_count, 
		 int * seed, int * side, int * tile_size, int * t_count){
	char ** grid = *grid1;
	char ** map  = *map1;
	
	if (grid!=NULL) {
		//printf("help\n");
		for (int i = 0; i<*side;i++){
			if (grid[i]!=NULL){
				free(grid[i]);
			}
		}
		free(grid);
	}
	if (map!=NULL) {
		for (int i = 0; i<*side;i++){
			if (map[i]){
				free(map[i]);
			}
		}
		free(map);
		//printf("map free :D\n");
	}
	
	if(*towers !=NULL){
			free(*towers);
		}
	if(*enemies!=NULL){
		free(*enemies);
	}
		
	srand(*seed); // seed based
	
	*side = 2;
	for(int i=0;i<2+(rand()%3);i++){
		*side=*side*2;
	}
	*tile_size = 512/ *side;
	
	// allocating 
	
	map=(char**) malloc(*side * sizeof(char*));
	for (int i = 0; i<*side;i++){
		map[i]= (char*)malloc(*side* sizeof(char));
	}
	grid=(char**) malloc(*side * sizeof(char*));
	for (int i = 0; i<*side;i++){
		grid[i]= (char*)malloc(*side* sizeof(char));
	}
	//printf("\n GENERATING THE NEW WORLD \n");
	smpl_gen(*seed,*side,map,grid);

	*enemies=(Enemy*)malloc(sizeof(Enemy)*MAX_ENEMIES);
	*towers=(Tower*)malloc(sizeof(Tower)*MAX_TOWERS);
	if(!enemies||!towers){
		printf("TYPE SHIII ! ! !\n");
		exit(-1);
	}
	//printf("idk\n");
	*current_enemy_count = 0;
	*t_count =0; 
	*click_count =0; 
	*argent=32;
	*map1=map;
	*grid1=grid;
	printf("yeah that worked ;-; its not here ig back to main moment T-T\n");
}

// ████████████████████████████████ MAIN ████████████████████████████████
int main() {
	srand(time(NULL));
    // ████████████████████████████████ VARIABLES INITIALES ████████████████████████████████
    SDL_Event e;
    
    int side= 10+(rand()%30);
    int tile_size=500/side;
    
    //char map[side][side];    
    //char grid[side][side];
	char ** map=(char**) malloc(side * sizeof(char*));
	for (int i = 0; i<side;i++){
		map[i]= (char*)malloc(side* sizeof(char));
	}
	
	char ** grid=(char**) malloc(side * sizeof(char*));
	for (int i = 0; i<side;i++){
			grid[i]= (char*)malloc(side* sizeof(char));
	}
    int click_positions[MAX_CLICKS][2];
    int click_count = 0, tower_count = 0;
    int argent = 50, compteur = 0;
    int current_enemy_count = 0;
    Enemy * enemies=malloc(sizeof(Enemy)*MAX_ENEMIES);//[MAX_ENEMIES];
    Tower * towers = malloc(sizeof(Tower)*MAX_TOWERS);//[MAX_TOWERS];
    srand(time(NULL));
	//bool turret_mode = false ;
	
	
	
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
    Mix_Music *musique2 = Mix_LoadMUS("assets/Volumetric.ogg");
    Mix_Chunk *tir_son = Mix_LoadWAV("assets/shoot.wav");
	
    if (!musique1 || !musique2 || !tir_son) {
        printf("Erreur chargement audio : %s\n", Mix_GetError());
    }
	
    // === Initializing fonts : and renderer ===
    TTF_Font *font = TTF_OpenFont("assets/asman.TTF", 22);
    if (!font) {
        printf("Erreur police : %s\n", TTF_GetError());
        return 1;
    }

    TTF_Font *t_font = TTF_OpenFont("assets/asman.TTF", 39);
    if (!t_font) {
        printf("Erreur police : %s\n", TTF_GetError());
        return 1;
    }
	
    SDL_Window *window = SDL_CreateWindow("Map 2D Aleatoire", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 920,512, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!window || !renderer) {
        printf("Erreur création fenêtre/renderer : %s\n", SDL_GetError());
        return 1;
    }

    // === Loading the textures : ===
    SDL_Texture *icon_texture = charge_texture(renderer, "assets/tmonkey.png");
    SDL_Texture *icon_texture1 = charge_texture(renderer, "assets/ninja.png");
    SDL_Texture *bush = charge_texture(renderer, "assets/bush.png");
    SDL_Texture *fleur = charge_texture(renderer, "assets/fleur_rouge.png");
    SDL_Texture *rock = charge_texture(renderer, "assets/rock.png");
    SDL_Texture *path = charge_texture(renderer, "assets/path.png");
    SDL_Texture *pine = charge_texture(renderer, "assets/forest_tree.png");
	SDL_Texture *pink_tree = charge_texture(renderer, "assets/pink_tree.png");
	SDL_Texture *fleur_rose  = charge_texture(renderer, "assets/fleur_rose.png");
	SDL_Texture *panneau  = charge_texture(renderer, "assets/panneau.png");
    if (!icon_texture || !icon_texture1 ||!bush ||!rock||!fleur||!path||!pine||!pink_tree||!fleur_rose||!panneau) return -1;

    // ████████████████████████████████ GÉNÉRATION MAP ████████████████████████████████
    
	smpl_gen(rand()%1000,side,map,grid);
	printf("gen 1 :\n");
	
	argent =1000;
	
	
    // ████████████████████████████████ MENU DE DÉBUT ████████████████████████████████
    Mix_PlayMusic(musique1, -1);
    
    Mix_FreeMusic(musique1);
    Mix_PlayMusic(musique2, -1);

    // ████████████████████████████████ PHASE D’ATTENTE AVANT VAGUE ████████████████████████████████
    //waiting(renderer, window, font, &start, &vie, &argent,side, map,tile_size,  click_positions, &click_count, towers, &tower_count, icon_texture);

    // ████████████████████████████████ INITIALISATION PREMIÈRE VAGUE ████████████████████████████████
	int vie = 1;
    int wave=0;
    current_enemy_count = 2;
    int start_x = find_start(side, map);
    Uint32 current_time = SDL_GetTicks();
    init_wave(enemies, current_enemy_count, wave, start_x, current_time);

    // === | Preparing for the mainloop  | ===
	
	Context c;
	
    c.running = true;
    c.playing = true;
    c.playingis_down = true;
    c.time = 0;
    c.inertia =0;
	c.start = false;
	c.as = false;
	c.menu = START;
	c.turet_mode=false;
	//c.wave=0;
	// === | Making the gui | ===

	Gui gui = {
		.widget_count=0,
		.w = side *tile_size,
		.h = side *tile_size
	};

	// === PLAY menu widgets ===

	//MAIN_MENU
	Text * quitter_p  = make_text_widget(START,(SDL_Rect){570,100,310,90},"QUITTER",(SDL_Color){255,170,100,255},t_font,exit_on_click);
	gui.widgets[gui.widget_count++]=(Widget*)create_collider_for((Widget*)quitter_p);
	gui.widgets[gui.widget_count++]=(Widget*)quitter_p;
	
	gui.widgets[gui.widget_count++]=(Widget*)make_box_widget(PLAY,(SDL_Rect){512,0,450,512},(SDL_Color){20,90,140,225},true,dummy);
	gui.widgets[gui.widget_count++]=(Widget*)make_box_widget(PLAY,(SDL_Rect){512,0,20,512},(SDL_Color){4,60,80,225},true,dummy);
	
	Text * start_b =make_text_widget(START,(SDL_Rect){136,200,200,90},"COMMENCER",(SDL_Color){255,255,255,255},t_font,zto); 
	gui.widgets[gui.widget_count++]=(Widget*)start_b;
	gui.widgets[gui.widget_count++]=(Widget*)create_collider_for((Widget*)start_b);

	Text * cont_b  =make_text_widget(START,(SDL_Rect){136,332,310,90},"CONTINUER",(SDL_Color){255,255,255,255},t_font,last_save); 
		gui.widgets[gui.widget_count++]=(Widget*)cont_b;
		gui.widgets[gui.widget_count++]=(Widget*)create_collider_for((Widget*)cont_b);
	
	Text * s_button = make_text_widget(PLAY,(SDL_Rect){542,450,240,60},"VAGUE SUIVANTE",(SDL_Color){255,255,255,255},font,wave_finished_press_for_next);

	//gui.widgets[gui.widget_count++]=(Widget*)debug_box(s_button);
	gui.widgets[gui.widget_count++]=(Widget*)s_button;
	gui.widgets[gui.widget_count++]=(Widget*)create_collider_for((Widget*)s_button);
	
	

	//adding the placement and upgrade buttons
	Text * upp_button = make_text_widget(PLAY,(SDL_Rect){542,210,300,50}," - MODE: UPGRADE ",(SDL_Color){255,255,255,255},font,placement_mode);
	Text * pla_button = make_text_widget(PLAY,(SDL_Rect){542,270,300,50}," - MODE: PLACEMENT ",(SDL_Color){255,255,255,255},font,upgrade_mode);
	gui.widgets[gui.widget_count++]=(Widget*)upp_button;
	gui.widgets[gui.widget_count++]=(Widget*)pla_button;
	gui.widgets[gui.widget_count++]=(Widget*)create_collider_for((Widget*)upp_button);
	gui.widgets[gui.widget_count++]=(Widget*)create_collider_for((Widget*)pla_button);

	//adding the menu widgets
	Text * pause = make_text_widget(PAUSE,(SDL_Rect){300,300,310,90},"pauser musique",(SDL_Color){255,255,255,255},t_font,pause_unpause);
	gui.widgets[gui.widget_count++]=(Widget*)create_collider_for((Widget*)pause);
	gui.widgets[gui.widget_count++]=(Widget*)pause;

	Text * main_menu_ = make_text_widget(PAUSE,(SDL_Rect){401,200,310,90},"menu principal",(SDL_Color){255,255,255,255},t_font,main_menu);
	gui.widgets[gui.widget_count++]=(Widget*)create_collider_for((Widget*)main_menu_);
	gui.widgets[gui.widget_count++]=(Widget*)main_menu_;
	
	Text * resume = make_text_widget(PAUSE,(SDL_Rect){200,400,310,90},"retour",(SDL_Color){255,255,255,255},t_font,back);
	gui.widgets[gui.widget_count++]=(Widget*)create_collider_for((Widget*)resume);
	gui.widgets[gui.widget_count++]=(Widget*)resume;

	Text * quitter_m  = make_text_widget(PAUSE,(SDL_Rect){532,100,310,90},"QUITTER",(SDL_Color){255,150,162,255},t_font,exit_on_click);
	gui.widgets[gui.widget_count++]=(Widget*)create_collider_for((Widget*)quitter_m);
	gui.widgets[gui.widget_count++]=(Widget*)quitter_m;

	//Game over widgets
	Text * skill_issue = make_text_widget(GAME_OVER,(SDL_Rect){300,200,310,90},"Vous avez perdu :(",(SDL_Color){255,255,255,255},t_font,dummy);
	gui.widgets[gui.widget_count++]=(Widget*)create_collider_for((Widget*)skill_issue);
	gui.widgets[gui.widget_count++]=(Widget*)skill_issue;
	
	Text * nouvelle_partie  = make_text_widget(GAME_OVER,(SDL_Rect){450,300,310,90},"Une autre chance",(SDL_Color){170,255,170,255},t_font,zto);
	gui.widgets[gui.widget_count++]=(Widget*)create_collider_for((Widget*)nouvelle_partie);
	gui.widgets[gui.widget_count++]=(Widget*)nouvelle_partie;

	Text * quitter_o  = make_text_widget(GAME_OVER,(SDL_Rect){100,100,310,90},"Abandonner ! ! !",(SDL_Color){255,150,160,255},t_font,exit_on_click);
	gui.widgets[gui.widget_count++]=(Widget*)create_collider_for((Widget*)quitter_o);
	gui.widgets[gui.widget_count++]=(Widget*)quitter_o;
	

	
	int x,y,final_x,final_y;
	gui_init(&gui);

	int seed = 0;
	
	initialize(&grid, &map, &towers, &enemies,
							 &click_count, &argent, &current_enemy_count, 
							 &seed, &side, &tile_size,&tower_count);
	bool escapeisdown[2];
	bool lu = true;
	while (c.running) {
	
		if (lu!=c.playing&&!c.playingis_down){
			//printf("%d",c.playing);
			lu = c.playing;
			if (c.playing){
				Mix_ResumeMusic();
			}else{
				Mix_PauseMusic();
			}
			lu = c.playing;
		}
    	if(c.menu==START){
			while (SDL_PollEvent(&e)) {
				 if (e.type == SDL_QUIT){ 
				 	c.running = 0;
				 }
			}
			
			//if (vie<=0)
			
			compteur =0;
		
    		SDL_SetRenderDrawColor(renderer, 10, 120, 110, 255);
    		SDL_RenderClear(renderer);
    		interact_gui(&gui);
    		update_gui(&gui,&c);
    		draw_gui_visible_components(c.menu,&gui,renderer);
			SDL_RenderPresent(renderer);
    	}else if(c.menu ==LOADING){
			while (SDL_PollEvent(&e)) {
				 if (e.type == SDL_QUIT){ 
				 	c.running = 0;
				 }
			}
			if(c.new){
				seed=time(NULL);
				wave=1;
				seed +=1;
				vie =rand()%7 + 4;
				//printf("using some magic");
				initialize(&grid, &map, &towers, &enemies,
						 &click_count, &argent, &current_enemy_count, 
						 &seed, &side, &tile_size,&tower_count);
				//printf("\nthe line 258 worked\n");
				c.menu = PLAY;
				save(SAVE,seed,wave,vie,argent,towers,tower_count);
			}else{
				load_seed(SAVE,&seed);
				int seed1 = seed;
				initialize(&grid, &map, &towers, &enemies,
										 &click_count, &argent, &current_enemy_count, 
										 &seed, &side, &tile_size,&tower_count);
				tower_count =0;
				load_game(SAVE,&seed,&wave,&vie,&argent,&towers,&tower_count,tile_size);
				if (seed1==seed){
					c.menu =PLAY;
				}
			}
    		interact_gui(&gui);
    		update_gui(&gui,&c);
    		draw_gui_visible_components(c.menu,&gui,renderer);
    		SDL_RenderPresent(renderer);	
    	}else if(c.menu==GAME_OVER){
			while (SDL_PollEvent(&e)) {
				 if (e.type == SDL_QUIT){ 
				 	c.running = 0;
				 }
			}
			//if (vie<=0)
    		SDL_SetRenderDrawColor(renderer, 90, 130, 110, 100);
    		SDL_RenderClear(renderer);
    		interact_gui(&gui);
    		update_gui(&gui,&c);
    		draw_gui_visible_components(c.menu,&gui,renderer);
			SDL_RenderPresent(renderer);
    	}else if(c.menu==PAUSE){
    		escapeisdown[1]=escapeisdown[0];
    		escapeisdown[0]=false;
			while (SDL_PollEvent(&e)) {
				 if (e.type == SDL_QUIT){ 
				 	c.running = 0;
				 }
				 if (e.type == SDL_KEYDOWN)
				 	if (e.key.keysym.sym==SDLK_ESCAPE){
						if(!escapeisdown[1])c.menu=PLAY;
						escapeisdown[0]=true;
				 }
			}
			//if (vie<=0)
    		SDL_SetRenderDrawColor(renderer, 90, 130, 110, 100);
    		SDL_RenderClear(renderer);
    		interact_gui(&gui);
    		update_gui(&gui,&c);
    		draw_gui_visible_components(c.menu,&gui,renderer);
			SDL_RenderPresent(renderer);
    	}else if(c.menu==PLAY){
				escapeisdown[1]=escapeisdown[0];
				escapeisdown[0]=false;
                while (SDL_PollEvent( & e)) {
                          if (e.type == SDL_QUIT) c.running = 0;
                          if (!c.turet_mode) {
                            if (e.type == SDL_MOUSEBUTTONDOWN && !c.as) {
                             
                              SDL_GetMouseState( & x, & y);

                              if (click_count < MAX_CLICKS && draw_click_zone(renderer, x, y, side, grid, tile_size, & final_x, & final_y, click_positions, click_count, argent)) {
                                click_positions[click_count][0] = final_x;
                                click_positions[click_count][1] = final_y;
                                click_count++;
                                argent -= 10;
                                towers[tower_count++] = (Tower) {
                                  .x = final_x, .y = final_y, .damage = 1, .range = 4 * tile_size, .speed_damage = 9, .cooldown = 0
                                };
                                
                              }
                            }
                          }
                          if (e.type == SDL_KEYDOWN) {
                            switch (e.key.keysym.sym) {
                            case SDLK_z:
                              printf("touche z appuyée !\n");
                              c.turet_mode = false;
                              break;

                            case SDLK_a:
                              printf("Touche a appuyée !\n");
                              c.turet_mode = true;
                              break;
                            case SDLK_ESCAPE:
                              c.menu = PAUSE;
                              if(!escapeisdown[1])escapeisdown[0]=true;
                              break;

                            }
                          }
                          if (c.turet_mode) {
                if (e.type == SDL_MOUSEBUTTONDOWN && !c.as) {
                             
                              SDL_GetMouseState( & x, & y);
                              if (click_count < MAX_CLICKS && draw_click_zone(renderer, x, y, side, grid, tile_size, & final_x, & final_y, click_positions, click_count, argent+2)) {
                                click_positions[click_count][0] = final_x;
                                click_positions[click_count][1] = final_y;
                                upgrade(towers, tower_count, final_x, final_y, tile_size,&argent);
                        }
                        }
                 }
                }

			
	        // ████████████████████████████████ AFFICHAGE ████████████████████████████████
	        //printf("e");
	        SDL_SetRenderDrawColor(renderer, 80, 160, 110, 255);
	        SDL_RenderClear(renderer);

			//printf("\t drawing map");
			//draw_map(renderer,side,tile_size,  map);
			
			for (int x = 0; x < side; x++) {
        		for (int y = 0; y < side; y++) {
        			if (map[x][y]=='a'){
        				SDL_Rect dst = {x*tile_size,y*tile_size,tile_size,tile_size};
        				SDL_RenderCopy(renderer,fleur,NULL,&dst);
        			}else if(map[x][y]=='c'||map[x][y]=='d'){
        				SDL_Rect dst = {x*tile_size,y*tile_size,tile_size,tile_size};
						SDL_RenderCopy(renderer,bush,NULL,&dst);
					}else if(map[x][y]=='o'||map[x][y]=='i'){
        				SDL_Rect dst = {x*tile_size,y*tile_size,tile_size,tile_size};
						SDL_RenderCopy(renderer,pine,NULL,&dst);
					}else if(map[x][y]=='l'){
        				SDL_Rect dst = {x*tile_size,y*tile_size,tile_size,tile_size};
						SDL_RenderCopy(renderer,pink_tree,NULL,&dst);								
        			}else if(map[x][y]=='q'){
        				SDL_Rect dst = {x*tile_size,y*tile_size,tile_size,tile_size};
						SDL_RenderCopy(renderer,rock,NULL,&dst);
					}else if(map[x][y]=='t'||map[x][y]=='w'){
        				SDL_Rect dst = {x*tile_size,y*tile_size,tile_size,tile_size};
						SDL_RenderCopy(renderer,fleur_rose,NULL,&dst);
        			}else if(map[x][y]==' '){
        				SDL_Rect dst = {x*tile_size,y*tile_size,tile_size,tile_size};
						SDL_RenderCopy(renderer,path,NULL,&dst);
					}else if(map[x][y]=='c'){
	       				SDL_Rect dst = {x*tile_size,y*tile_size,tile_size,tile_size};
						SDL_RenderCopy(renderer,rock,NULL,&dst);
					}
        		}
        	}

			
	        for (int i = 0; i < tower_count; i++) {
	            SDL_Rect dst = { towers[i].x-towers[i].level, towers[i].y-towers[i].level, tile_size+2*towers[i].level, tile_size+2*towers[i].level};
	            SDL_RenderCopy(renderer, towers[i].active_shoot > 0 ? icon_texture1 : icon_texture, NULL, &dst);
	        }
			
			c.as = false;
			interact_gui(&gui);
	        update_gui(&gui,&c);
			draw_gui_visible_components(c.menu,&gui,renderer);
			if(vie<=0){
				c.menu = GAME_OVER;
			}
			if (c.start){
		        // ████████████████████████████████ ENNEMIS ████████████████████████████████
		        move_all_enemies(renderer,side, tile_size, map,  &vie, enemies, current_enemy_count, &argent);
		        // ████████████████████████████████ TOURELLES TIRENT ████████████████████████████████
		        turret(towers, tower_count, enemies, current_enemy_count, tir_son, tile_size);
				
		        // ████████████████████████████████ CHECK VAGUE TERMINÉE ████████████████████████████████
		        if (all_enemies_dead(enemies, current_enemy_count)) {
					if (vie>=1)save(SAVE,seed,wave,vie,argent,towers,tower_count);
		            argent+=1;
		            //waiting(renderer, window, font, &start, &vie, &argent,side,  map, tile_size, click_positions, &click_count, towers, &tower_count, icon_texture);
					c.start= false;
		            wave++;
		            current_enemy_count = wave + 1;
		            start_x = find_start(side, map);
		            current_time = SDL_GetTicks();
		            init_wave(enemies, current_enemy_count, wave, start_x, current_time);
		        }

		        // ████████████████████████████████ ENNEMI ARRIVÉ EN BAS ████████████████████████████████
		        // ████████████████████████████████ ENNEMI ARRIVÉ EN BAS ████████████████████████████████
		        for (int i = 0; i < current_enemy_count; i++) {
		            if (enemies[i].y == side - 1 && enemies[i].life > 0) {
		                compteur++;
		                enemies[i].life = 0;
		                c.menu = GAME_OVER;
						if (wave!=0){
		                	vie--;
		                }  // décrémente bien la vie du joueur ici
		                printf(">>> Ennemi %d a atteint la fin ! Vie restante du joueur : %d\n", i, vie);
		        
		                if (vie <= 0) {
		                    printf("Tu es mort\n");
		                    // end_menu(renderer, font, side, tile_size);
		                    c.running = 0;
		                    break;
		                }
		            }
		        }
	        }
	        draw_hud(renderer, font, vie, argent,wave);

	        SDL_RenderPresent(renderer);
	        SDL_Delay(16);
		}
    }
    if (c.start == false)save(SAVE,seed,wave,vie,argent,towers,tower_count);

	if (map!=NULL){
		for(int i=0;i<side;i++){
			if(map[i]!=NULL){
				free(map[i]);
			}
		}
		free(map);
	}
	if (grid!=NULL){
		for(int i=0;i<side;i++){
			if(grid[i]!=NULL){
				free(grid[i]);
			}
		}
		free(grid);
	}
	// YES i'm VERY proud of my gui widgets free system
	free_gui(&gui);
	if (enemies!=NULL){
		free(enemies);
	}
	if (towers!=NULL){
		free(towers);
	}
    // ████████████████████████████████ NETTOYAGE FINAL ████████████████████████████████
    clean_all(renderer, window, icon_texture, icon_texture1, font, musique2, tir_son);
    return 0;
}

