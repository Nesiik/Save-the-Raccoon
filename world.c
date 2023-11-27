#include "world.h"
#include "fonctions_fichiers.h"

void init_data(world_t * world){
    world->gameover = Menu;
    world->cur_level = 0;
    world->levels = malloc(NB_LEVELS*sizeof(level*));
    int taille = sizeof("../levels/level_ii.txt")+3;
    char str[taille];
    for (char i = 0; i < NB_LEVELS; i++)
    {
        sprintf(str, "%s%d%s", "../levels/level_", i ,".txt");
        //printf("%s \n",str);
        world->levels[i] = malloc(sizeof(level));
        world->levels[i]->level_tab = lire_fichier(str);
        if(world->levels[i]->level_tab == NULL){
            SDL_Log("Erreur lecture fichier %s \n",str);
            return;
        }
        taille_fichier(str,&world->levels[i]->nb_ligne_level_tab,&world->levels[i]->nb_col_level_tab);
    }
}

/*
char world_collision(world_t* world,SDL_Rect* pos){
    int tabx = pos->x/DIRT_SIZE;
    int maxtabx = tabx + pos->w/DIRT_SIZE;
    int taby = pos->y/DIRT_SIZE;
    int maxtaby = taby + pos->y/DIRT_SIZE;
    if (tabx > world->levels[world->cur_level].)
    {
        
    }
    
    if (
        pos->x + pos->w >= world->levels[world->cur_level].nb_ligne_level_tab &&
        1
    ){
        return 1;
    }
    return 0;
}
*/

void free_levels(world_t* world){
    for (size_t i = 0; i < NB_LEVELS; i++)
    {
        desallouer_tab_2D(world->levels[i]->level_tab,world->levels[i]->nb_ligne_level_tab);
        free(world->levels[i]);
    }
    free(world->levels);
}

int is_game_over(world_t *world){
    if(world->gameover > Alive) // > 0
        return 1;
    else
        return 0;
}

int fin(world_t* world){
    if(world->gameover  == Quit)
        return 1;
    else
        return 0;
}