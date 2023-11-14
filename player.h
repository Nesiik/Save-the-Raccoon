#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>

typedef struct player_s{
    int x;
    int y;
    int w;
    int h;
}player_t;

void init_player(player_t * player);

void deplacement(player_t * player, SDL_Keycode code);

#endif