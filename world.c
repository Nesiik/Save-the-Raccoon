#include <assert.h>
#include <stdio.h>
#include <math.h>
#include "world.h"
#include "sdl.h"
#include "fonctions_fichiers.h"

void world_event(SDL_Event *event,world_t *world , ressources_t* ressources){
    switch (event->type){
        case SDL_QUIT:
            world->game_state = Quit;
        break;
        case SDL_KEYDOWN:
            if(world->game_state == Menu){
                if(event->key.keysym.sym == SDLK_ESCAPE){
                    world->game_state = Quit;
                }
            }
            else if(world->game_state == Alive){
                if(event->key.keysym.sym == SDLK_ESCAPE){
                    world->game_state = Menu;
                    world->last_level = world->cur_level;
                    world->cur_level = 0;
                }
            }
        break;
        case SDL_MOUSEBUTTONDOWN:
            if(world->game_state == Menu && event->button.button == SDL_BUTTON_LEFT)
                mouse_menu_events(event->button,world,ressources);
        default:
            break;
    }
}


void mouse_menu_events(SDL_MouseButtonEvent button,world_t* world, ressources_t* ressources){
    for(unsigned int i = 0; i < MAIN_MENU_ITEM_COUNT; i++) {
        if(SDL_PointInRect(&(SDL_Point){button.x,button.y},&ressources->MenuItems->ItemList[i]->rect) ) {
            ressources->MenuItems->lastselectedItem = ressources->MenuItems->curselectedItem;
            ressources->MenuItems->curselectedItem = i;
            switch (i) {
                case 0: // Jouer
                    world->game_state = Alive;
                    if(world->last_level != -1){
                        world->cur_level = world->last_level;
                        world->last_level = -1;
                    }
                    else{
                        world->cur_level = 1;
                        world->start_level_time = SDL_GetPerformanceCounter();
                        world->need_player_pos_update = 1;
                    }
                    break;
                case 1: // Option
                    break;
                case 2: // Quitter
                    world->game_state = Quit;
                    break;
                
                default:
                    break;
            }
        }
    }
}

void render_sky(SDL_Renderer* renderer, ressources_t* ressources){
    SDL_Rect rect = {0,0,WINDOW_WIDTH,WINDOW_HEIGHT};
    if(SDL_RenderCopy(renderer, ressources->sky->text, NULL, &rect) < 0){
        printf("Error rendering the sky : %s \n",SDL_GetError());
    }
}

void render_main_menu_text(SDL_Renderer *renderer,ressources_t *ressources){
    if (ressources->MenuItems->curselectedItem != ressources->MenuItems->lastselectedItem){
        SDL_DestroyTexture(ressources->MenuItems->ItemList[ressources->MenuItems->curselectedItem]->texture);
        SDL_Color color = (SDL_Color){ 180, 180, 180, 255 };
        ressources->MenuItems->ItemList[ressources->MenuItems->curselectedItem]->texture = creer_texte_texture(renderer,ressources->font,2,&color,ressources->MenuItems->ItemList[ressources->MenuItems->curselectedItem]->text,ressources->MenuItems->ItemList[ressources->MenuItems->curselectedItem]->rect.x,ressources->MenuItems->ItemList[ressources->MenuItems->curselectedItem]->rect.y,&ressources->MenuItems->ItemList[ressources->MenuItems->curselectedItem]->rect);
        if(ressources->MenuItems->lastselectedItem != -1){ // if first selected item
            color = (SDL_Color){ 100, 100, 100, 255 };
            SDL_DestroyTexture(ressources->MenuItems->ItemList[ressources->MenuItems->lastselectedItem]->texture);
            ressources->MenuItems->ItemList[ressources->MenuItems->lastselectedItem]->texture = creer_texte_texture(renderer,ressources->font,2,&color,ressources->MenuItems->ItemList[ressources->MenuItems->lastselectedItem]->text,ressources->MenuItems->ItemList[ressources->MenuItems->lastselectedItem]->rect.x,ressources->MenuItems->ItemList[ressources->MenuItems->lastselectedItem]->rect.y,&ressources->MenuItems->ItemList[ressources->MenuItems->lastselectedItem]->rect);
            ressources->MenuItems->lastselectedItem = ressources->MenuItems->curselectedItem;
        }
        ressources->MenuItems->lastselectedItem = ressources->MenuItems->curselectedItem;
    }

    for(unsigned int i = 0; i < MAIN_MENU_ITEM_COUNT; i++) {
        if(SDL_RenderCopy(renderer,ressources->MenuItems->ItemList[i]->texture, NULL, &ressources->MenuItems->ItemList[i]->rect) < 0){
            printf("Renderer main menu error : %s \n",SDL_GetError());
        }
    }
}

void render_worlds(SDL_Renderer* renderer,ressources_t* ressources,world_t* world){
    level_t* cur_level = world->levels[world->cur_level];
    if(cur_level->nb_ligne_level_tab == 0 || cur_level->nb_col_level_tab == 0) {
        afficher_texte(renderer,ressources->font,FAST_TEXT,"Map empty,please check the map.",50,50);
    }
    char cur_char;
    int tabij,dirt_y,dirt_x,one_sprite_w,one_sprite_h;
    for (int i = 0; i < cur_level->nb_ligne_level_tab; i++) {
        for (int j = 0; j < cur_level->nb_col_level_tab; j++) {
            cur_char = cur_level->level_tab[i][j];
            if(cur_char >= 'A' && cur_char <= 'X'){ //dirt/grass ascii : 65-88
                tabij = cur_char - 'A'; // conversion ascii -> int
                dirt_y = tabij/6;
                dirt_x = tabij - dirt_y*6;
                one_sprite_w = (ressources->dirt->sprite_w/N_WIDTH_DIRT_SPRITE)-1;
                one_sprite_h = (ressources->dirt->sprite_h/N_HEIGHT_DIRT_SPRITE)-1;
                SDL_Rect src = {dirt_x*one_sprite_w + (dirt_x+1),dirt_y*one_sprite_w + (dirt_y+1),one_sprite_w,one_sprite_h};
                SDL_Rect dest = {j*one_sprite_w,i*one_sprite_h , one_sprite_w , one_sprite_h};
                if(SDL_RenderCopy(renderer,ressources->dirt->text,&src,&dest)<0){
                    SDL_Log("Rendering dirt error : %s \n",SDL_GetError());
                }
            }
            else if(cur_char >= '<' && cur_char <= '?'){ //spike ascii : 60-63
                tabij = cur_char - '<'; // conversion ascii -> int
                SDL_Rect dest = {j*ressources->spike->sprite_w,i*ressources->spike->sprite_h , ressources->spike->sprite_w , ressources->spike->sprite_h};
                if(SDL_RenderCopyEx(renderer, ressources->spike->text, NULL, &dest, tabij*90, NULL, SDL_FLIP_NONE )<0){
                    SDL_Log("Rendering spike error : %s \n",SDL_GetError());
                }
            }    
            else if(cur_char == 't'){ //tree ascii : 116
                SDL_Rect dest = {j*DIRT_SIZE,i*DIRT_SIZE - DIRT_SIZE /* tree only but removed for better grass compatibility- (TREE_SIZE - DIRT_SIZE)*/ , TREE_SIZE , TREE_SIZE};
                if(SDL_RenderCopy(renderer, ressources->tree->text, NULL, &dest)<0){
                    SDL_Log("Rendering tree error : %s \n",SDL_GetError());
                }
            }
            else if(cur_char == 'f'){ //flag ascii : 102
                SDL_Rect dest = {j*DIRT_SIZE,i*DIRT_SIZE , ressources->flag->sprite_w , ressources->flag->sprite_h};
                if(SDL_RenderCopy(renderer, ressources->flag->text, NULL, &dest)<0){
                    SDL_Log("Rendering flag error : %s \n",SDL_GetError());
                }
            }
            else if(cur_char == 'c'){ //coin ascii : 99
                SDL_Rect dest = {j*DIRT_SIZE,i*DIRT_SIZE, DIRT_SIZE , DIRT_SIZE};
                if(SDL_RenderCopy(renderer, ressources->coin->text, NULL, &dest)<0){
                    SDL_Log("Rendering coin error : %s \n",SDL_GetError());
                }
            }    
        }
    }
}

char get(world_t* world,const int i,const int j){ // x,y
    level_t* cur_level = world->levels[world->cur_level];
    if(i>=0 && j>=0 && i< cur_level->nb_col_level_tab && j< cur_level->nb_ligne_level_tab){
        return cur_level->level_tab[j][i];
    }
    return -1;
}

char is_empty(world_t* world,const int i, const int j) { //x,y
    level_t* cur_level = world->levels[world->cur_level];
    if(i < 0 || j < 0 || i >= cur_level->nb_col_level_tab || j >= cur_level->nb_ligne_level_tab)
        return -1; // invalid
    if (cur_level->level_tab[j][i] == ' ') {
        return 1;
    }
    return 0;
}

char get_collision(world_t* world,const int i, const int j) { //x,y
    level_t* cur_level = world->levels[world->cur_level];
    if(i < 0 || j < 0 || i >= cur_level->nb_col_level_tab || j >= cur_level->nb_ligne_level_tab)
        return -1; // invalid
    char tile = get(world,i,j);
    if ((tile >= 'C' && tile <= 'D') || (tile >= 'G' && tile <= 'X')) { // dirt (ignore grass)
        return 1;
    }
    else if (tile >= '<' && tile <= '?') { // spike
        return 2;
    }
    else if (tile == 'c') { // coin
        return 3;
    }
    else if (tile == 'f') { // flags
        return 4;
    } 
    return 0;
}

void spike_collision(world_t* world){
    world->game_state = Dead;
}

void death(SDL_Renderer* renderer, world_t* world,ressources_t* ressources){
    afficher_texte(renderer,ressources->font,SLOW_TEXT,"You died :(",WINDOW_WIDTH/2,WINDOW_HEIGHT/2);
    world->last_level = -1;
    world->cur_level = 0;
    world->need_player_pos_update = 1;
}

void flag_collision(world_t* world){
    world->game_state = Win;
}

void save_time(world_t* world){
    FILE *fp = fopen(RESULT_FILE, "w");
    if (fp) {
        for (int i = 1; i < NB_LEVELS; i++){
            fprintf(fp, "level %d : %0.6fs\n", i, world->levels_times[i]);
        }
        fclose(fp);
    }
}

void won(SDL_Renderer* renderer,world_t* world,ressources_t* ressources){
    world->end_level_time = SDL_GetPerformanceCounter();
    //printf("end : %"PRIu64" , start : %"PRIu64" , freq : %lf \n",world->end_level_time,world->start_level_time,(double)SDL_GetPerformanceFrequency());
    double end_time = (world->end_level_time - world->start_level_time) / (double)SDL_GetPerformanceFrequency();
    int taille = snprintf(NULL,0,"You won in %.4fs.",end_time);
    char str[taille];
    if(sprintf(str,"You won in %.4fs.",end_time) < 0){
        printf("Error creating string of the win time.");
    }
    afficher_texte(renderer,ressources->font,SLOW_TEXT,str,WINDOW_WIDTH/2,WINDOW_HEIGHT/2);
    world->levels_times[world->cur_level] = end_time;
    if(world->cur_level == NB_LEVELS-1){ //last level
        world->cur_level = 0;
        world->game_state = Finished;
        save_time(world);
    }
    else{
        world->cur_level += 1;
    }
    world->need_player_pos_update = 1;
    world->last_level = -1;
}

int fin(world_t* world){
    if(world->game_state == Quit)
        return 1;
    else
        return 0;
}

int countDigit(unsigned int n) { 
    if (n/10 == 0) 
        return 1; 
    return 1 + countDigit(n / 10); 
}

world_t* init_world(){
    world_t* world = malloc(sizeof(world_t));
    world->game_state = Menu;
    world->last_level = -1;
    world->cur_level = 0;
    world->need_player_pos_update = 0;
    world->player_spawn_x = 0;
    world->player_spawn_y = 0;
    world->levels = malloc(NB_LEVELS*sizeof(level_t*));
    int taille = sizeof("../levels/level_.txt")+countDigit(NB_LEVELS);
    char str[taille];
    for (unsigned int i = 0; i < NB_LEVELS; i++) {
        sprintf(str, "%s%d%s", "../levels/level_", i ,".txt");
        level_t* new_level = malloc(sizeof(level_t));
        new_level->level_tab = lire_fichier(str,&new_level->nb_ligne_level_tab,&new_level->nb_col_level_tab);
        if(new_level->level_tab == NULL){
            SDL_Log("Error reading : %s \n",str);
        }
        world->levels[i] = new_level;
    }
    world->levels_times = malloc(sizeof(double) * NB_LEVELS);
    return world;
}

void free_levels(world_t* world){
    for (size_t i = 0; i < NB_LEVELS; i++){
        desallouer_tab_2D(world->levels[i]->level_tab);
        free(world->levels[i]);
    }
    free(world->levels);
    free(world->levels_times);
}
