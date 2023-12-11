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
    player->backward = 0;
    player->x = 90.;
    player->y = 550.;
    player->vx = 0;
    player->vy = 0;
    return player;
}

void render_player(SDL_Renderer* renderer, const player_t* player){
    SDL_Rect dest = {player->x,player->y,player->sprite->sprite_w,player->sprite->sprite_h};
    if(SDL_RenderCopyEx(renderer, player->sprite->text, NULL, &dest, 0, NULL, player->backward ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE )<0){
        SDL_Log("Player rendering error : %s",SDL_GetError());
    }
}

/*
void set_state(player_t* player,unsigned char state,unsigned char back){
    player->state = state;
    player->backward = back;
}
*/

void move_player(player_t* player,world_t* world,double dt){
    double x = player->x,y = player->y;
    x += player->vx * dt;
    //printf("x : %d");
    char empty = is_empty(world,(player->backward? (int)x/DIRT_SIZE : ((int)x + player->sprite->sprite_w)/DIRT_SIZE), (int)y/DIRT_SIZE);
    printf("%d \n",empty);
    if (!empty)
    {
        player->vx = 0;
        //return;
    }
    player->x = x; /* update player x */

    y += player->vy * dt;
    player->vy += dt * 300; /* gravity */
    if(player->state == TAKEOFF)
        player->state = FLIGHT;
    empty = is_empty(world,(int)x/DIRT_SIZE, ((int)y+(int)player->sprite->sprite_h)/DIRT_SIZE);
    printf("%d \n",empty);
    if (!empty)
    {
        printf("pas vide \n");
        player->vy = 0;
        //return;
    }
    player->y = y; /* update player y */
}



void handle_key_down2(player_t* player){
    int numkey;
    const Uint8 *kbstate = SDL_GetKeyboardState(&numkey);
    printf("%d , q state : %d ,d state : %d \n",numkey,kbstate[SDL_SCANCODE_Q],kbstate[SDL_SCANCODE_D]);
    //int state = REST;
    if (kbstate[SDL_SCANCODE_Q] || kbstate[SDL_SCANCODE_D]) {
        if (kbstate[SDL_SCANCODE_Q] && kbstate[SDL_SCANCODE_D]) {
            player->vx = 0;
        }
        else
        {
            player->backward = kbstate[SDL_SCANCODE_Q] ? 1 : 0;
            player->vx = kbstate[SDL_SCANCODE_Q] ? -150 : 150; 
        }
        if(player->state == REST){
            player->state = WALK;
        }
    }
    else if (kbstate[SDL_SCANCODE_Z]) {
        if (player->state == WALK || player->state == REST )
        {
            player->vy = -200;
            player->state = TAKEOFF;
        }
    }
    else {
        if (player->state == WALK)
        {
            player->vx = 0;
            player->state = REST;
        }
    }
    
}

void handle_key_down(player_t* player, SDL_Keycode code){
    int state = REST;
    unsigned char back = 0;
    switch (code) {
        case SDLK_q:
            player->vx = -150;
            back = 1; 
            state = WALK;
            break;
        case SDLK_d:
            if(player->vx == -150){
                player->vx = 0; // if both q and d are pressed
            }
            player->vx = 150;
            state = WALK;
            break;
        case SDLK_z:
            if (player->state == FLIGHT || player->state == TAKEOFF)
            {
                break;
            }
            player->vy = -200;
            state = TAKEOFF;
        default:
            break;
    }
    //set_state(player,state,back);
}

//void handle_key_up(player_t* player, SDL_Keycode code){player;code;}

void deplacement(player_t* player,world_t* world, SDL_Keycode code){
    //printf("x : %d , y : %d \n",player->x,player->y);
    if(code == SDLK_q){
        if (player->x - 5 <= 0)
        {
            player->x = 0;
            return;
        }
        if(!is_empty(world,(player->x - 5 )/DIRT_SIZE ,(player->y + player->sprite->sprite_h )/DIRT_SIZE)){
            return;
            int snap = round(player->x/DIRT_SIZE) * DIRT_SIZE;
            //printf("snap : %d , x : %lf \n",snap,player->x);
            player->x = snap + (snap > player->x ? 1 : -1);
        }
        player->x -=SPEED;
    }
    if(code== SDLK_d){
        if (player->x + player->sprite->sprite_w + 5 >= 1280)
        {
            player->x = 1280 - player->sprite->sprite_w;
            return;
        }
        if(!is_empty(world,(player->x + player->sprite->sprite_w + 5 )/DIRT_SIZE ,(player->y + player->sprite->sprite_h )/DIRT_SIZE)){
            return;
            int snap = round(player->x/DIRT_SIZE) * DIRT_SIZE;
            //printf("snap : %d , x : %lf \n",snap,player->x);
            player->x = snap + (snap > player->x ? 1 : -1);
            return;
        }
        player->x +=SPEED;
    }
    if(code == SDLK_s){
        if (player->y + player->sprite->sprite_h + 5 >= 720)
        {
            player->y = 720 - player->sprite->sprite_h;
            return;
        }
        if(!is_empty(world,(player->x + (player->sprite->sprite_w)/2 )/DIRT_SIZE ,(player->y + player->sprite->sprite_h + 5 )/DIRT_SIZE)){
            return;
            int snap = round(player->y/DIRT_SIZE) * DIRT_SIZE;
            //printf("snap : %d , y : %lf \n",snap,player->y);
            player->y = snap + (snap > player->y ? 1 : -1);
            return;
        }
        player->y +=10;
    }
    if(code == SDLK_z){
        if (player->y + player->sprite->sprite_h - 5 <= 0)
        {
            player->y = 0;
            return;
        }
        if(!is_empty(world,(player->x + (player->sprite->sprite_w)/2)/DIRT_SIZE ,(player->y - 5 )/DIRT_SIZE)){
            return;
            int snap = round(player->y/DIRT_SIZE) * DIRT_SIZE;
            //printf("snap : %d , y : %lf \n",snap,player->y);
            player->y = snap + (snap > player->y ? 1 : -1);
            return;
        }
        player->y -=10;
    }
}

void free_player(player_t* player){
    SDL_DestroyTexture(player->sprite->text);
    free(player->sprite);
}