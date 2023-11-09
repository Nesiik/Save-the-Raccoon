#ifndef SPRITE_H
#define SPRITE_H

#include <SDL.h>

typedef struct sprite_s{
    SDL_Rect src;
    SDL_Rect dest;
    SDL_Texture* text;
}sprite;

#endif