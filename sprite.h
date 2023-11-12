#ifndef SPRITE_H
#define SPRITE_H

#include <SDL.h>

typedef struct sprite_s{
    SDL_Rect src;
    SDL_Rect dest;
    SDL_Texture* text;
}sprite;


sprite* charger_image (const char* nomfichier, SDL_Renderer* renderer);
sprite* charger_image_transparante(const char* nomfichier, SDL_Renderer* renderer,Uint8 r, Uint8 g, Uint8 b);
#endif