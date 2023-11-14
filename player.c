#include "player.h"

void init_player(player_t * player){
    player->x = 100;
    player->y = 100;
    player->w = 32;
    player->h = 32;
}

void deplacement(player_t * player, SDL_Keycode code){
    if(code == SDLK_q){
        player->x -=5;
    }
    if(code== SDLK_d){
        player->x +=5;
    }
    if(code == SDLK_SPACE){
        player->y+=5;
    }
}