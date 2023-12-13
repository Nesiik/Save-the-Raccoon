#include <assert.h>
#include <stdio.h>
#include <list.h>
#include "world.h"
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
    for (unsigned char i = 0; i < MAIN_MENU_ITEM_COUNT; i++) {
        Item menuItem = ressources->MenuItems->ItemList[i];

        if(SDL_PointInRect(&(SDL_Point){button.x,button.y},&menuItem.rect ) ) {
            ressources->MenuItems->lastselectedItem = ressources->MenuItems->curselectedItem;
            ressources->MenuItems->curselectedItem = i;
            switch (i) {
                case 0: // Jouer
                    world->game_state = Alive;
                    if(world->last_level != -1){
                        world->cur_level = world->last_level;
                    }
                    else{
                        world->cur_level = 1;
                        world->start_level_time = SDL_GetTicks();
                    }
                    //world->depart = SDL_GetTicks();
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
        printf("%s",SDL_GetError());
    }
}

void render_worlds(SDL_Renderer* renderer,ressources_t* ressources,world_t* world){
    //level* cur_level = lis world->levels;
    int level = world->cur_level;
    if(level == 0){ // Menu
        render_main_menu_text(renderer,ressources);
    }
    char cur_char;
    int tabij,dirt_y,dirt_x,one_sprite_w,one_sprite_h;
    if(world->levels[level]->nb_ligne_level_tab == 0 || world->levels[level]->nb_col_level_tab == 0) {
        afficher_texte(renderer,ressources->font,"Map empty,please check the map.",50,50);
    }
    for (int i = 0; i < world->levels[level]->nb_ligne_level_tab; i++)
    {
        for (int j = 0; j < world->levels[level]->nb_col_level_tab; j++)
        {
            cur_char = world->levels[level]->level_tab[i][j];
            if(cur_char >= 'A' && cur_char <= 'X'){ //dirt ascii : 65-88
                tabij = cur_char - 'A'; // conversion ascii -> int
                dirt_y = tabij/6;
                dirt_x = tabij - dirt_y*6;
                one_sprite_w = (ressources->dirt->sprite_w/N_WIDTH_DIRT_SPRITE)-1;
                one_sprite_h = (ressources->dirt->sprite_h/N_HEIGHT_DIRT_SPRITE)-1;
                SDL_Rect src = {dirt_x*one_sprite_w + (dirt_x+1),dirt_y*one_sprite_w + (dirt_y+1),one_sprite_w,one_sprite_h};
                SDL_Rect dest = {j*one_sprite_w,i*one_sprite_h , one_sprite_w , one_sprite_h};
                if(SDL_RenderCopy(renderer,ressources->dirt->text,&src,&dest)<0){
                    SDL_Log("Rendering dirt error : %s",SDL_GetError());
                }
            }
            else if(cur_char >= '<' && cur_char <= '?'){ //spike ascii : 60-63
                tabij = cur_char - '<'; // conversion ascii -> int
                SDL_Rect dest = {j*ressources->spike->sprite_w,i*ressources->spike->sprite_h , ressources->spike->sprite_w , ressources->spike->sprite_h};
                if(SDL_RenderCopyEx(renderer, ressources->spike->text, NULL, &dest, tabij*90, NULL, SDL_FLIP_NONE )<0){
                    SDL_Log("Rendering spike error : %s",SDL_GetError());
                }
            }    
            else if(cur_char == 't'){ //tree ascii : 116
                SDL_Rect dest = {j*DIRT_SIZE,i*DIRT_SIZE - DIRT_SIZE /* tree only but removed for better grass compatibility- (TREE_SIZE - DIRT_SIZE)*/ , TREE_SIZE , TREE_SIZE};
                if(SDL_RenderCopy(renderer, ressources->tree->text, NULL, &dest)<0){
                    SDL_Log("Rendering tree error : %s",SDL_GetError());
                }
            }
            else if(cur_char == 'f'){ //flag ascii : 102
                SDL_Rect dest = {j*DIRT_SIZE,i*DIRT_SIZE , ressources->flag->sprite_w , ressources->flag->sprite_h};
                if(SDL_RenderCopy(renderer, ressources->flag->text, NULL, &dest)<0){
                    SDL_Log("Rendering flag error : %s",SDL_GetError());
                }
            }
            else if(cur_char == 'c'){ //coin ascii : 99
                SDL_Rect dest = {j*DIRT_SIZE,i*DIRT_SIZE, DIRT_SIZE , DIRT_SIZE};
                if(SDL_RenderCopy(renderer, ressources->coin->text, NULL, &dest)<0){
                    SDL_Log("Rendering coin error : %s",SDL_GetError());
                }
            }           
        }
    }
}

char get(world_t* world,const int i,const int j){ // x,y
    if(i>=0 && j>=0 && i<world->levels[world->cur_level]->nb_col_level_tab && j<world->levels[world->cur_level]->nb_ligne_level_tab){
        return world->levels[world->cur_level]->level_tab[j][i];
    }
    return -1;
}

char is_empty(world_t* world,const int i, const int j) { //x,y
    if(i < 0 || j < 0 || i >= world->levels[world->cur_level]->nb_col_level_tab || j >= world->levels[world->cur_level]->nb_ligne_level_tab)
        return -1; // invalid
    if (world->levels[world->cur_level]->level_tab[j][i] == ' ')
    {
        return 1;
    }
    return 0;
}

char get_collision(world_t* world,const int i, const int j) { //x,y
    if(i < 0 || j < 0 || i >= world->levels[world->cur_level]->nb_col_level_tab || j >= world->levels[world->cur_level]->nb_ligne_level_tab)
        return -1; // invalid
    char tile = get(world,i,j);
    if ((tile >= 'C' && tile <= 'D') || (tile >= 'G' && tile <= 'X')) { // dirt
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

void death(world_t * world){
    if(world->game_state = Dead){
        fin(world);
    }
}

void flag_collision(world_t* world){
    world->game_state = Win;
}

/*void next_level(world_t* world){
    if(world->game_state = Win){

    }
}*/

int fin(world_t* world){
    if(world->game_state  == Quit)
        return 1;
    else
        return 0;
}

int win(world_t* world){
    if(world->game_state == Win)
        return 1;
    else
        return 0;
}

int countDigit(unsigned int n) 
{ 
    if (n/10 == 0) 
        return 1; 
    return 1 + countDigit(n / 10); 
}

world_t* init_world(){
    world_t* world = malloc(sizeof(world_t));
    world->game_state = Menu;
    world->cur_level = 0;
    world->last_level = -1;
    /*
    list(level*,levels);
    memset(&levels,0,sizeof(level*)); */
    world->levels = malloc(NB_LEVELS*sizeof(level*)); 
    int taille = sizeof("../levels/level_.txt")+countDigit(NB_LEVELS);
    char str[taille+1]; // +1 for \0 
    for (unsigned int i = 0; i < NB_LEVELS; i++)
    {
        sprintf(str, "%s%d%s", "../levels/level_", i ,".txt");
        level* new_level = malloc(sizeof(level));
        new_level->level_tab = lire_fichier(str);
        if(new_level->level_tab == NULL){
            SDL_Log("Error reading : %s \n",str);
        }
        taille_fichier(str,&new_level->nb_ligne_level_tab,&new_level->nb_col_level_tab);
        world->levels[i] = new_level;
        //list_push(levels,new_level);
        /*
        world->levels[i] = malloc(sizeof(level));
        world->levels[i]->level_tab = lire_fichier(str);
        if(world->levels[i]->level_tab == NULL){
            SDL_Log("Error reading : %s \n",str);
        }
        taille_fichier(str,&world->levels[i]->nb_ligne_level_tab,&world->levels[i]->nb_col_level_tab);
        */
    }
    //world->levels = levels;
    return world;
}

void free_levels(world_t* world){
    for (size_t i = 0; i < NB_LEVELS; i++)
    {
        desallouer_tab_2D(world->levels[i]->level_tab);
        free(world->levels[i]);
    }
    free(world->levels);
}
