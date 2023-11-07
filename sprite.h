#ifndef SPRITE_H
#define SPRITE_H

#include <SDL.h>

struct sprite_s{
    SDL_Rect src;
    SDL_Rect dest;
    SDL_Texture* text;
};
typedef struct sprite_s sprite_t;

#endif