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

void print_state(player_t* player){ // use for debugging
    switch (player->state)
    {
    case REST:
        printf("REST \n");
        break;
    case WALK:
        printf("WALK \n");
        break;
    case TAKEOFF:
        printf("TAKEOFF \n");
        break;
    case FLIGHT:
        printf("FLIGHT \n");
        break;
    case FALL:
        printf("FALL \n");
        break;
    case LANDING:
        printf("LANDING \n");
        break;
    default:
        break;
    }
}

void move_player(player_t* player,world_t* world,double dt){
    if(player->state == TAKEOFF)
        player->state = FLIGHT;
    double x = player->x,y = player->y;
    /*
    printf("y : \n");
    printf("test %d \n",get(world,(int)x/DIRT_SIZE,(int)y/DIRT_SIZE));
    printf("test0 %d \n",get(world,(int)x/DIRT_SIZE,(int)y/DIRT_SIZE+1));
    printf("test1 %d \n",get(world,(int)x/DIRT_SIZE,((int)y+(int)player->sprite->sprite_h)/DIRT_SIZE)); 
    printf("test2 %d \n",get(world,(int)x/DIRT_SIZE,((int)y+(int)player->sprite->sprite_h)/DIRT_SIZE+1));
    
    printf("x :\n");
    printf("test %d \n",get(world,(int)x/DIRT_SIZE,((int)y+(int)player->sprite->sprite_h)/DIRT_SIZE+1));
    printf("test0 %d \n",get(world,(int)x/DIRT_SIZE+1,((int)y+(int)player->sprite->sprite_h)/DIRT_SIZE+1)); 
    printf("test1 %d \n",get(world,((int)x + player->sprite->sprite_w)/DIRT_SIZE,((int)y+(int)player->sprite->sprite_h)/DIRT_SIZE+1));
    printf("test2 %d \n",get(world,((int)x + player->sprite->sprite_w)/DIRT_SIZE+1,((int)y+(int)player->sprite->sprite_h)/DIRT_SIZE+1));
    //printf("test3 %d \n",get(world,((int)x + player->sprite->sprite_w)/DIRT_SIZE+1,((int)y+(int)player->sprite->sprite_h)/DIRT_SIZE));
    printf("\n");
    */
    //printf("test1 %d \n",get(world,player->backward? (int)x/DIRT_SIZE : ((int)x + player->sprite->sprite_w)/DIRT_SIZE,(int)y/DIRT_SIZE+1));
    //printf("test2 %d \n",get(world,player->backward? (int)x/DIRT_SIZE : ((int)x + player->sprite->sprite_w)/DIRT_SIZE,((int)y+(int)player->sprite->sprite_h)/DIRT_SIZE));
    //printf("\n");
    char empty_under = get_collision(world,((int)x + player->sprite->sprite_w)/DIRT_SIZE, ((int)y+(int)player->sprite->sprite_h)/DIRT_SIZE+1);
    if(player->state != FLIGHT && empty_under == 0){
        player->vy = 100;
        player->state = FALL;
    }
    if(player->vx != 0){
        x += player->vx * dt;
        char empty_horizon_high = get_collision(world,(player->backward? (int)x/DIRT_SIZE : ((int)x + player->sprite->sprite_w)/DIRT_SIZE), (int)y/DIRT_SIZE+1);
        char empty_horizon_low = get_collision(world,(player->backward? (int)x/DIRT_SIZE : ((int)x + player->sprite->sprite_w)/DIRT_SIZE), ((int)y+(int)player->sprite->sprite_h)/DIRT_SIZE);
        //printf("horizontal collision high : %d,horizontal collision low : %d \n",empty_horizon_high,empty_horizon_low);
        if (empty_horizon_high == 1 || empty_horizon_low == 1)
        {
            int snap = round(x/DIRT_SIZE) * DIRT_SIZE;
            x = snap + (snap > x ? 1 : -1);
            player->vx = 0;
        }
        else if(empty_horizon_high == 2 || empty_horizon_low == 2){
            spike_collision(world);
        }
        else if( empty_horizon_high == 4 || empty_horizon_low == 4){
            //flag_collision(world);
        }
        player->x = x; /* update player x */
    }
    if (player->vy != 0) {
        y += player->vy * dt;
        player->vy += dt * 300; /* gravity */
        char empty_left = get_collision(world,(int)x/DIRT_SIZE, ((int)y+(int)player->sprite->sprite_h)/DIRT_SIZE);
        char empty_right = get_collision(world,(int)x/DIRT_SIZE+1, ((int)y+(int)player->sprite->sprite_h)/DIRT_SIZE);
        if (empty_left == 1 || empty_right == 1)
        {
            int snap = round(y/DIRT_SIZE) * DIRT_SIZE;
            y = snap + (snap > y ? 1 : -1);
            player->vy = 0;
            if (player->state == FALL || player->state == FLIGHT)   
                player->state = LANDING;
        }
        else if(empty_left == 2 || empty_right == 2){
            spike_collision(world);
        }
        player->y = y; /* update player y */
    }
}

void handle_keyboard_player(player_t* player){
    const Uint8 *kbstate = SDL_GetKeyboardState(NULL);
    char key_pressed = 0;
    //printf("q : %d, d : %d, z : %d \n",kbstate[SDL_SCANCODE_A],kbstate[SDL_SCANCODE_D],kbstate[SDL_SCANCODE_W]);
    if (kbstate[SDL_SCANCODE_A] || kbstate[SDL_SCANCODE_D]) {
        if (kbstate[SDL_SCANCODE_A] && kbstate[SDL_SCANCODE_D]) {
            player->vx = 0;
        }
        else
        {
            player->backward = kbstate[SDL_SCANCODE_A] ? 1 : 0;
            player->vx = kbstate[SDL_SCANCODE_A] ? -150 : 150; 
        }
        if(player->state == REST || player->state == LANDING){
            player->state = WALK;
        }
        key_pressed++;
    }
    if (kbstate[SDL_SCANCODE_W]) {
        if (player->state == WALK || player->state == REST )
        {
            player->vy = -200;
            player->state = TAKEOFF;
        }
        key_pressed++;
    }
    if(key_pressed == 0) {
        player->vx = 0;
        if (player->state == WALK || player->state == LANDING )
        {
            player->state = REST;
        }
    }
}

void free_player(player_t* player){
    SDL_DestroyTexture(player->sprite->text);
    free(player->sprite);
}