#include "player.h"
#include "world.h"
#include "sdl.h"
#include <math.h>

player_t* init_player(){
    player_t* player = malloc(sizeof(player_t));
    player->state = REST;
    player->backward = 0;
    player->x = 0.;
    player->y = 0.;
    player->vx = 0;
    player->vy = 0;
    return player;
}

void render_player(SDL_Renderer* renderer, const player_t* player ,ressources_t* ressources){
    SDL_Rect dest = {player->x,player->y,ressources->player->sprite_w,ressources->player->sprite_h};
    if(SDL_RenderCopyEx(renderer, ressources->player->text, NULL, &dest, 0, NULL, player->backward ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE )<0){
        SDL_Log("Player rendering error : %s \n",SDL_GetError());
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

void get_player_spawn(world_t* world){
    level_t* level = *world->cur_level;
    for (int i = 0; i < level->nb_ligne_level_tab; i++)
    {
        for (int j = 0; j < level->nb_col_level_tab; j++)
        {
            char tabij = get(world,j,i);
            if (tabij == 'x')
            {
                world->player_spawn_x = j*DIRT_SIZE;
                world->player_spawn_y = i*DIRT_SIZE;
                return;
            }
            
        }
        
    }
    
}

void set_spawn(world_t* world,player_t* player){
    get_player_spawn(world);
    player->x = world->player_spawn_x;
    player->y = world->player_spawn_y;
    world->player_spawn_x = 0;
    world->player_spawn_y = 0;
    world->need_player_pos_update = 0;
}

void reset_state(player_t* player){
    player->vx=0;
    player->vy=0;
    player->state = REST;
}

void move_player(player_t* player,world_t* world,ressources_t* ressources,double dt){
    if(player->state == TAKEOFF)
        player->state = FLIGHT;
    double x = player->x,y = player->y;
    char empty_under = get_collision(world,((int)x + PLAYER_SPRITE_SIZE)/DIRT_SIZE, ((int)y+PLAYER_SPRITE_SIZE)/DIRT_SIZE+1);
    if(player->state != FLIGHT && empty_under == 0){
        player->vy = 100;
        player->state = FALL;
    }
    if(player->state == REST){
        return; // we don't need to do physics if we're not moving
    }

    x += player->vx * dt;
    if ((int)x <= 0) {
        x = 0;
        player->vx = 0;
    }
    else if (((int)x + PLAYER_SPRITE_SIZE) >= WINDOW_WIDTH) {
        x = WINDOW_WIDTH - ressources->player->sprite_w;
        player->vx=0;
    }

    char empty_horizon_high = get_collision(world,(player->backward? (int)x/DIRT_SIZE : ((int)x + PLAYER_SPRITE_SIZE)/DIRT_SIZE), (int)y/DIRT_SIZE+1);
    char empty_horizon_low = get_collision(world,(player->backward? (int)x/DIRT_SIZE : ((int)x + PLAYER_SPRITE_SIZE)/DIRT_SIZE), ((int)y+PLAYER_SPRITE_SIZE)/DIRT_SIZE);
    if (empty_horizon_high == 1 || empty_horizon_low == 1)
    {
        x = player->x;
        player->vx=0;
    }
    else if(empty_horizon_high == 2 || empty_horizon_low == 2){
        spike_collision(world);
        reset_state(player);
    }
    else if( empty_horizon_high == 4 || empty_horizon_low == 4){
        flag_collision(world);
        reset_state(player);
    }
    player->x = x; /* update player x */

    if(player->state > TAKEOFF){ // do gravity/vertical physics only when in the air (FLIGHT=3, FALL=4, LANDING=5)
        if ((int)y <= 0) {
            y = 0;
            player->vy = 0;
        }
        else if ((int)y > WINDOW_HEIGHT){
            spike_collision(world);
            reset_state(player);
        }
        y += player->vy * dt;
        player->vy += dt * 300; /* gravity */
        char empty_left,empty_right;
        empty_left = get_collision(world,(int)x/DIRT_SIZE+1, ((int)y+PLAYER_SPRITE_SIZE)/DIRT_SIZE);
        empty_right = get_collision(world,((int)x + PLAYER_SPRITE_SIZE)/DIRT_SIZE, ((int)y+PLAYER_SPRITE_SIZE)/DIRT_SIZE);
        if (empty_left == 1 || empty_right == 1)
        {
            y = player->y;
            player->vy = 0;
            if (player->state == FALL || player->state == FLIGHT)   
                player->state = LANDING;
        }
        else if(empty_left == 2 || empty_right == 2){
            spike_collision(world);
            reset_state(player);
        }
        else if( empty_left == 4 || empty_right == 4){
            flag_collision(world);
            reset_state(player);
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