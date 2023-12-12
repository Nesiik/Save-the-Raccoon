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

void print_state(player_t* player){
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
    //char empty_under = is_empty(world,(int)x/DIRT_SIZE, ((int)y+(int)player->sprite->sprite_h)/DIRT_SIZE+1);
    //printf("x tile %d, y tile %d, y+1 tile %d, y+h tile %d \n",((int)x + player->sprite->sprite_w)/DIRT_SIZE,(int)y/DIRT_SIZE,(int)y/DIRT_SIZE+1,((int)y+(int)player->sprite->sprite_h)/DIRT_SIZE);
    char empty_under = get_collision(world,((int)x + player->sprite->sprite_w)/DIRT_SIZE, ((int)y+(int)player->sprite->sprite_h)/DIRT_SIZE+1);
    //printf("%d \n",empty_under);
    /*
    printf("test %d \n",get(world,(int)x/DIRT_SIZE,(int)y/DIRT_SIZE));
    printf("test0 %d \n",get(world,(int)x/DIRT_SIZE,(int)y/DIRT_SIZE+1));
    printf("test1 %d \n",get(world,(int)x/DIRT_SIZE,((int)y+(int)player->sprite->sprite_h)/DIRT_SIZE)); 
    printf("test2 %d \n",get(world,(int)x/DIRT_SIZE,((int)y+(int)player->sprite->sprite_h)/DIRT_SIZE+1));
    printf("x :\n");
    printf("test %d \n",get(world,(int)x/DIRT_SIZE,(int)y/DIRT_SIZE+1));
    printf("test0 %d \n",get(world,(int)x/DIRT_SIZE+1,(int)y/DIRT_SIZE+1)); 
    printf("test1 %d \n",get(world,((int)x + player->sprite->sprite_w)/DIRT_SIZE,(int)y/DIRT_SIZE+1));
    printf("test2 %d \n",get(world,((int)x + player->sprite->sprite_w)/DIRT_SIZE+1,(int)y/DIRT_SIZE+1));
    printf("\n");
    */
    if(player->state != FLIGHT && empty_under == 0){
        //printf("test %d \n",get(world,((int)x + player->sprite->sprite_w)/DIRT_SIZE,((int)y+(int)player->sprite->sprite_h)/DIRT_SIZE));
        //printf("test2 %d \n",get(world,((int)x + player->sprite->sprite_w)/DIRT_SIZE,((int)y+(int)player->sprite->sprite_h)/DIRT_SIZE+1));
        player->vy = 100;
        player->state = FALL;
    }
    //printf("après \n");
    //printf("x : %f , y : %f , vx: %f, vy : %f \n",player->x,player->y,player->vx,player->vy);
    x += player->vx * dt;
    //printf("x : %d");
    //char empty = is_empty(world,(player->backward? (int)x/DIRT_SIZE : ((int)x + player->sprite->sprite_w)/DIRT_SIZE), (int)y/DIRT_SIZE);
    char empty_horizon_high = get_collision(world,(player->backward? (int)x/DIRT_SIZE : ((int)x + player->sprite->sprite_w)/DIRT_SIZE), (int)y/DIRT_SIZE);
    char empty_horizon_low = get_collision(world,(player->backward? (int)x/DIRT_SIZE : ((int)x + player->sprite->sprite_w)/DIRT_SIZE), (int)y/DIRT_SIZE+1);
    //printf("horizontal collision high : %d,horizontal collision low : %d \n",empty_horizon_high,empty_horizon_low);
    if (empty_horizon_high == 1 || empty_horizon_low == 1)
    {
        int snap = round(x/DIRT_SIZE) * DIRT_SIZE;
        x = snap + (snap > x ? 1 : -1);
        player->vx = 0;
        //return;
    }
    player->x = x; /* update player x */

    if (player->vy != 0) {
        y += player->vy * dt;
        player->vy += dt * 300; /* gravity */
        char empty_left = get_collision(world,(int)x/DIRT_SIZE, ((int)y+(int)player->sprite->sprite_h)/DIRT_SIZE);
        char empty_right = get_collision(world,(int)x/DIRT_SIZE+1, ((int)y+(int)player->sprite->sprite_h)/DIRT_SIZE);
        //printf("%d \n",empty);
        //printf("verti collision gauche  : %d,verti collision droite : %d \n",empty_horizon_high,empty_horizon_low);
        if (empty_left == 1 || empty_right == 1)
        {
            //printf("pas vide \n");
            int snap = round(y/DIRT_SIZE) * DIRT_SIZE;
            y = snap + (snap > y ? 1 : -1);
            player->vy = 0;
            if (player->state == FALL || player->state == FLIGHT)   
                player->state = LANDING;
            //return;
        }
        player->y = y; /* update player y */
    }
    if(empty_horizon_high == 2 && empty_horizon_low == 2){
        spike_collision(world);
    }
    if( empty_horizon_high == 4 && empty_horizon_low == 4){
        flag_collision(world);
    }
}

void handle_key_down2(player_t* player){
    //int numkey;
    const Uint8 *kbstate = SDL_GetKeyboardState(NULL);
    //printf("%d , q state : %d ,d state : %d \n",numkey,kbstate[SDL_SCANCODE_Q],kbstate[SDL_SCANCODE_D]);
    //int state = REST;
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

/*
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
*/

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