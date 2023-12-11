#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>
#include "sprite.h"
#include "world.h"

#define PLAYER_SPRITE_SIZE 64
#define SPEED 10

enum States { REST=0, WALK=1, TAKEOFF =2, FLIGHT=3, FALL=4, LANDING=5 };

typedef struct player_s{
    double x,y;
    double vx,vy;
    //Uint64 time;
    int state;
    sprite* sprite;
    unsigned char backward;
}player_t;

player_t* init_player(SDL_Renderer *renderer);
//void set_state(player_t* player,unsigned char state,unsigned char back);
void move_player(player_t* player,world_t* world,double dt);
void handle_key_down2(player_t* player);
void handle_key_down(player_t* player, SDL_Keycode code);
void render_player(SDL_Renderer* renderer, const player_t* player);
void deplacement(player_t* player,world_t* world, SDL_Keycode code);
void free_player(player_t* player);

#endif