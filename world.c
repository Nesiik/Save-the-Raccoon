#include "world.h"
#include "fonctions_fichiers.h"

void init_data(world_t * world){
    world->gameover = Menu;
    world->cur_level = 0;
    int taille = sizeof("../levels/level_ii.txt");
    char str[taille];
    for (char i = 0; i < NB_LEVELS; i++)
    {
        sprintf(str, "%s%d%s", "../levels/level_", i ,".txt");
        //printf("%s \n",str);
        world->levels[i].level_tab = lire_fichier(str);
        if(world->levels[i].level_tab == NULL){
            SDL_Log("Erreur lecture fichier %s \n",str);
            return;
        }
        taille_fichier(str,&world->levels[i].nb_ligne_level_tab,&world->levels[i].nb_col_level_tab);
    }
}

void free_levels(world_t* world){
    for (size_t i = 0; i < NB_LEVELS; i++)
    {
        desallouer_tab_2D(world->levels[i].level_tab,world->levels[i].nb_ligne_level_tab);
    }

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