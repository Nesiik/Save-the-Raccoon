#include "player.h"
#include "sprite.h"

void init_player(SDL_Renderer *renderer,player_t* player){
    player->player = charger_image_png("../assets/raccoon/staticv2.png", renderer);
    player->pos.w = 64;
    //player->pos.w = player->player->src.w;
    //player->pos.h = player->player->src.h;
    player->pos.h = 64;
    player->pos.x = 300;
    player->pos.y = 360;
}

void render_player(SDL_Renderer* renderer,player_t* player){
    if(SDL_RenderCopy(renderer,player->player->text,NULL,&player->pos)<0){
        SDL_Log("Erreur : %s",SDL_GetError());
    }
}

void deplacement(player_t * player, SDL_Keycode code){
    printf("x : %d , y : %d \n",player->pos.x,player->pos.y);
    if(code == SDLK_q){
        if (player->pos.x - 5 <= 0)
        {
            player->pos.x = 0;
            return;
        }
        player->pos.x -=5;
    }
    if(code== SDLK_d){
        if (player->pos.x + player->pos.w + 5 >= 1280)
        {
            player->pos.x = 1280 - player->pos.w;
            return;
        }
        player->pos.x +=5;
    }
    if(code == SDLK_s){
        if (player->pos.y + player->pos.h + 5 >= 720)
        {
            player->pos.y = 720 - player->pos.h;
            return;
        }
        player->pos.y +=5;
    }
    if(code == SDLK_SPACE){
        if (player->pos.y - 5 <= 0)
        {
            player->pos.y = 0;
            return;
        }
        player->pos.y -=5;
    }
}

void free_player(player_t * player){
    SDL_DestroyTexture(player->player->text);
    free(player->player);
}