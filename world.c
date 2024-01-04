#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <list.h>
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
                    world->cur_level = list_elem_front(world->levels);
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
    int i = 0;
    list_each_elem(ressources->MenuItems->ItemList,pItem){
        if(SDL_PointInRect(&(SDL_Point){button.x,button.y},&(*pItem)->rect ) ) {
            ressources->MenuItems->lastselectedItem = ressources->MenuItems->curselectedItem;
            ressources->MenuItems->curselectedItem = pItem;
            switch (i) {
                case 0: // Jouer
                    world->game_state = Alive;
                    if(world->last_level != NULL){
                        world->cur_level = world->last_level;
                        world->last_level = NULL;
                    }
                    else{
                        world->cur_level = list_elem_next(list_elem_front(world->levels));
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
        i++;
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
        Item_t* selectedItem = *ressources->MenuItems->curselectedItem;
        SDL_Color color = (SDL_Color){ 180, 180, 180, 255 };
        //Bouton sélectionné
        SDL_DestroyTexture(selectedItem->texture);
        selectedItem->texture = creer_texte_texture(renderer,ressources->font,2,&color,selectedItem->text,selectedItem->rect.x,selectedItem->rect.y,&selectedItem->rect);
        if(ressources->MenuItems->lastselectedItem != NULL){ // si il y a eu une sélection avant cette sélection
            color = (SDL_Color){ 100, 100, 100, 255 };
            Item_t* lastItem = *ressources->MenuItems->lastselectedItem;
            SDL_DestroyTexture(lastItem->texture);
            lastItem->texture = creer_texte_texture(renderer,ressources->font,2,&color,lastItem->text,lastItem->rect.x,lastItem->rect.y,&lastItem->rect);
            ressources->MenuItems->lastselectedItem = ressources->MenuItems->curselectedItem;
        }
        ressources->MenuItems->lastselectedItem = ressources->MenuItems->curselectedItem;
    }
    list_each(ressources->MenuItems->ItemList,item){
        if(SDL_RenderCopy(renderer, item->texture, NULL, &item->rect) < 0){
            printf("Renderer main menu error : %s \n",SDL_GetError());
        }
    }
}

void render_worlds(SDL_Renderer* renderer,ressources_t* ressources,world_t* world){
    level_t* cur_level = *world->cur_level;
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
    level_t* cur_level = *world->cur_level;
    if(i>=0 && j>=0 && i< cur_level->nb_col_level_tab && j< cur_level->nb_ligne_level_tab){
        return cur_level->level_tab[j][i];
    }
    return -1;
}

char is_empty(world_t* world,const int i, const int j) { //x,y
    level_t* cur_level = *world->cur_level;
    if(i < 0 || j < 0 || i >= cur_level->nb_col_level_tab || j >= cur_level->nb_ligne_level_tab)
        return -1; // invalid
    if (cur_level->level_tab[j][i] == ' ') {
        return 1;
    }
    return 0;
}

char get_collision(world_t* world,const int i, const int j) { //x,y
    level_t* cur_level = *world->cur_level;
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
    world->last_level = NULL;
    world->cur_level = list_elem_front(world->levels);
    world->need_player_pos_update = 1;
    list_each_elem(world->levels_times, time_ptr) {
       list_elem_remove (time_ptr);
   }
}

void flag_collision(world_t* world){
    world->game_state = Win;
}

void save_time(world_t* world){
    FILE *fp = fopen(RESULT_FILE, "w");
    if (fp) {
        int i = 1;
        list_each_elem(world->levels_times,time_ptr){
            fprintf(fp, "level %d : %0.6fs\n", i, *time_ptr);
            list_elem_remove (time_ptr);
            i++;
        }
        fclose(fp);
    }
}

void won(SDL_Renderer* renderer,world_t* world,ressources_t* ressources){
    world->end_level_time = SDL_GetPerformanceCounter();
    //printf("end : %"PRIu64" , start : %"PRIu64" , freq : %lf \n",world->end_level_time,world->start_level_time,(double)SDL_GetPerformanceFrequency());
    double end_time = (world->end_level_time - world->start_level_time )/ (double)SDL_GetPerformanceFrequency();
    int taille = snprintf(NULL,0,"You won in %.4fs.",end_time);
    char str[taille];
    if(sprintf(str,"You won in %.4fs.",end_time) < 0){
        printf("Error creating string of the win time.");
    }
    afficher_texte(renderer,ressources->font,SLOW_TEXT,str,WINDOW_WIDTH/2,WINDOW_HEIGHT/2);
    list_push(world->levels_times,end_time);
    if(list_back(world->levels) == *world->cur_level){ //last level
        world->cur_level = list_elem_front(world->levels);
        world->game_state = Finished;
        save_time(world);
    }
    else{
        world->cur_level = list_elem_next(world->cur_level);
    }
    world->need_player_pos_update = 1;
    world->last_level = NULL;
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
    world->last_level = NULL;
    world->need_player_pos_update = 0;
    world->player_spawn_x = 0;
    world->player_spawn_y = 0; 
    list(level_t*,levels);
    memset(&levels,0,sizeof(levels));
    int taille = sizeof("../levels/level_.txt")+countDigit(NB_LEVELS);
    char str[taille];
    for (unsigned int i = 0; i < NB_LEVELS; i++) {
        sprintf(str, "%s%d%s", "../levels/level_", i ,".txt");
        level_t* new_level = malloc(sizeof(level_t));
        new_level->level_tab = lire_fichier(str,&new_level->nb_ligne_level_tab,&new_level->nb_col_level_tab);
        if(new_level->level_tab == NULL){
            SDL_Log("Error reading : %s \n",str);
        }
        list_push(levels,new_level);
    }
    world->cur_level = list_elem_front(levels);
    world->levels = levels;
    list(double,levels_times);
    memset(&levels_times,0,sizeof(levels_times));
    world->levels_times = levels_times;
    return world;
}

void free_levels(world_t* world){
    list_each(world->levels,elem){
        desallouer_tab_2D(elem->level_tab);
        free(elem);
    }
    list_clear(world->levels);
    list_clear(world->levels_times);
}
