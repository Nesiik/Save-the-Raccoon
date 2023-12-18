#include "sprite.h"
#include <stdio.h>

sprite* charger_image (const char* nomfichier, SDL_Renderer* renderer){
    sprite* sprite = malloc(sizeof(sprite));
    SDL_Surface* bmp = SDL_LoadBMP(nomfichier);
    if(bmp == NULL){
        printf("%s",SDL_GetError());
    }
    sprite->text = SDL_CreateTextureFromSurface(renderer, bmp);
    if (sprite->text == NULL){
        printf("%s \n",SDL_GetError());
    }
    sprite->sprite_w = bmp->w;
    sprite->sprite_h = bmp->h;
    SDL_FreeSurface(bmp);
    return sprite;
}

sprite* charger_image_png(const char* nomfichier, SDL_Renderer* renderer){
    sprite* sprite = malloc(sizeof(*sprite));
    SDL_Surface* surface = IMG_Load(nomfichier);
    if(surface == NULL){
        printf("Loading %s error : %s",nomfichier,SDL_GetError());
    }
    sprite->text = SDL_CreateTextureFromSurface(renderer,surface);
    if (sprite->text == NULL){
        printf("Error creating texture from img '%s' : %s \n",nomfichier,SDL_GetError());
    }
    sprite->sprite_w = surface->w;
    sprite->sprite_h = surface->h;
    SDL_FreeSurface(surface);
    return sprite;
}

sprite* charger_image_transparante(const char* nomfichier, SDL_Renderer* renderer, Uint8 r, Uint8 g, Uint8 b){
    sprite* sprite = malloc(sizeof(sprite));
    SDL_Surface* bmp = SDL_LoadBMP(nomfichier);
    if(bmp == NULL){
        printf("%s",SDL_GetError());
    }
    if(SDL_SetColorKey(bmp,SDL_TRUE,SDL_MapRGB(bmp->format,r,g,b)) < 0){
        printf("%s \n",SDL_GetError());
    }
    sprite->text = SDL_CreateTextureFromSurface(renderer, bmp);
        if (sprite->text == NULL){
        printf("%s \n",SDL_GetError());
    }
    sprite->sprite_w = bmp->w;
    sprite->sprite_h = bmp->h;
    SDL_FreeSurface(bmp);
    return sprite;

}