#include "player.h"
#include "sprite.h"

void init_player(SDL_Renderer *renderer,player_t* player){
    player->player = charger_image_png("../assets/raccoon/staticv2.png", renderer);
    player->pos.w = 67;
    //player->pos.w = player->player->src.w;
    //player->pos.h = player->player->src.h;
    player->pos.h = 64;
    player->pos.x = 640;
    player->pos.y = 360;
}

void render_player(SDL_Renderer* renderer,player_t* player){
    if(SDL_RenderCopy(renderer,player->player->text,NULL,&player->pos)<0){
        SDL_Log("Erreur : %s",SDL_GetError());
    }
}

void deplacement(player_t * player, SDL_Keycode code){
    if(code == SDLK_q){
        player->pos.x -=4;
    }
    if(code== SDLK_d){
        player->pos.x +=4;
    }
    if(code == SDLK_s){
        player->pos.y +=4;
    }
    if(code == SDLK_SPACE){
        player->pos.y -=4;
    }
}

void free_player(player_t * player){
    SDL_DestroyTexture(player->player->text);
    free(player->player);
}