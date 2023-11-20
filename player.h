#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>

typedef struct player_s{
    SDL_Rect pos;
}player_t;

void init_player(player_t * player);

void deplacement(player_t * player, SDL_Keycode code);

#endif