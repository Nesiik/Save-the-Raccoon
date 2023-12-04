#ifndef SPRITE_H
#define SPRITE_H

#include <SDL.h>
#include <SDL_image.h>

typedef struct sprite_s{
    int sprite_w,sprite_h;
    SDL_Texture* text;
}sprite;


sprite* charger_image (const char* nomfichier, SDL_Renderer* renderer);
sprite* charger_image_transparante(const char* nomfichier, SDL_Renderer* renderer,Uint8 r, Uint8 g, Uint8 b);
sprite* charger_image_png(const char* nomfichier, SDL_Renderer* renderer);
#endif