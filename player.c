#include "player.h"

void init_player(player_t * player){
    player->pos.x = 640;
    player->pos.y = 360;
}

void deplacement(player_t * player, SDL_Keycode code){
    if(code == SDLK_q){
        player->pos.x -=5;
    }
    if(code== SDLK_d){
        player->pos.x +=5;
    }
    if(code == SDLK_SPACE){
        player->pos.y+=5;
    }
}