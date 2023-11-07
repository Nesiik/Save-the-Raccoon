#ifndef SPRITE_H
#define SPRITE_H

#include <SDL.h>

struct sprite_s{
    SDL_Rect src;
    SDL_Rect dest;
};
typedef struct sprite_s sprite_t;

#endif