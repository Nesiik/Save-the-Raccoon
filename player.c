#include "player.h"
#include "sprite.h"
#include "world.h"
#include <math.h>

player_t* init_player(SDL_Renderer* renderer){
    player_t* player = malloc(sizeof(player_t));
    player->sprite = charger_image_png("../assets/raccoon/staticv2.png", renderer);
    player->sprite->sprite_w = PLAYER_SPRITE_SIZE;
    player->sprite->sprite_h = PLAYER_SPRITE_SIZE;
    player->state = REST;
    player->x = 300;
    player->y = 360;
    return player;
}

void render_player(SDL_Renderer* renderer,player_t* player){
    SDL_Rect dest = {player->x,player->y,player->sprite->sprite_w,player->sprite->sprite_h};
    if(SDL_RenderCopy(renderer,player->sprite->text,NULL,&dest)<0){
        SDL_Log("Erreur : %s",SDL_GetError());
    }
}

void deplacement(player_t* player,world_t* world, SDL_Keycode code){
    //printf("x : %d , y : %d \n",player->x,player->y);
    if(code == SDLK_q){
        if (player->x - 5 <= 0)
        {
            player->x = 0;
            return;
        }
        if(!is_empty(world,(player->x - 5 )/DIRT_SIZE ,(player->y + player->sprite->sprite_h )/DIRT_SIZE)){
            int snap = round(player->x/DIRT_SIZE) * DIRT_SIZE;
            //printf("snap : %d , x : %lf \n",snap,player->x);
            player->x = snap + (snap > player->x ? 1 : -1);
            return;
        }
        player->x -=5;
    }
    if(code== SDLK_d){
        if (player->x + player->sprite->sprite_w + 5 >= 1280)
        {
            player->x = 1280 - player->sprite->sprite_w;
            return;
        }
        if(!is_empty(world,(player->x + player->sprite->sprite_w + 5 )/DIRT_SIZE ,(player->y + player->sprite->sprite_h )/DIRT_SIZE)){
            int snap = round(player->x/DIRT_SIZE) * DIRT_SIZE;
            //printf("snap : %d , x : %lf \n",snap,player->x);
            player->x = snap + (snap > player->x ? 1 : -1);
            return;
        }
        player->x +=5;
    }
    if(code == SDLK_s){
        if (player->y + player->sprite->sprite_h + 5 >= 720)
        {
            player->y = 720 - player->sprite->sprite_h;
            return;
        }
        if(!is_empty(world,(player->x + (player->sprite->sprite_w)/2 )/DIRT_SIZE ,(player->y + player->sprite->sprite_h + 5 )/DIRT_SIZE)){
            int snap = round(player->y/DIRT_SIZE) * DIRT_SIZE;
            //printf("snap : %d , y : %lf \n",snap,player->y);
            player->y = snap + (snap > player->y ? 1 : -1);
            return;
        }
        player->y +=5;
    }
    if(code == SDLK_z){
        if (player->y + player->sprite->sprite_h - 5 <= 0)
        {
            player->y = 0;
            return;
        }
        if(!is_empty(world,(player->x + (player->sprite->sprite_w)/2)/DIRT_SIZE ,(player->y - 5 )/DIRT_SIZE)){
            int snap = round(player->y/DIRT_SIZE) * DIRT_SIZE;
            //printf("snap : %d , y : %lf \n",snap,player->y);
            player->y = snap + (snap > player->y ? 1 : -1);
            return;
        }
        player->y -=5;
    }
}

void free_player(player_t* player){
    SDL_DestroyTexture(player->sprite->text);
    free(player->sprite);
}