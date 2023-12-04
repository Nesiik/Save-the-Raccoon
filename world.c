#include <assert.h>
#include "world.h"
#include "fonctions_fichiers.h"

world_t* init_world(){
    world_t* world = malloc(sizeof(world_t));
    world->game_state = Menu;
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
        }
        taille_fichier(str,&world->levels[i]->nb_ligne_level_tab,&world->levels[i]->nb_col_level_tab);
    }
    return world;
}

char is_empty(world_t* world,const int i, const int j) {
    if(i>=0 && j>=0 && i<world->levels[world->cur_level]->nb_col_level_tab && j<world->levels[world->cur_level]->nb_ligne_level_tab){
        if (world->levels[world->cur_level]->level_tab[j][i] == ' ')
        {
            return 1;
        }
    }
    return 0;
}

void free_levels(world_t* world){
    for (size_t i = 0; i < NB_LEVELS; i++)
    {
        desallouer_tab_2D(world->levels[i]->level_tab,world->levels[i]->nb_ligne_level_tab);
        free(world->levels[i]);
    }
    free(world->levels);
}

int is_game_over(world_t *world){
    if(world->game_state > Alive) // > 0
        return 1;
    else
        return 0;
}

int fin(world_t* world){
    if(world->game_state  == Quit)
        return 1;
    else
        return 0;
}