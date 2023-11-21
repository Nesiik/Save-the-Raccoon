#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>
#include "sprite.h"

typedef struct player_s{
    SDL_Rect pos;
    sprite* player;
}player_t;

void init_player(SDL_Renderer *renderer,player_t* player);
void render_player(SDL_Renderer* renderer, player_t* player);
void deplacement(player_t * player, SDL_Keycode code);
void free_player(player_t * player);

#endif