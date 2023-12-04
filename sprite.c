#include "sprite.h"

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
    SDL_Surface* surf = IMG_Load(nomfichier);
    if(surf == NULL){
        printf("%s",SDL_GetError());
    }
    sprite->text = SDL_CreateTextureFromSurface(renderer,surf);
    if (sprite->text == NULL){
        printf("%s \n",SDL_GetError());
    }
    sprite->sprite_w = surf->w;
    sprite->sprite_h = surf->h;
    SDL_FreeSurface(surf);
    return sprite;
}

sprite* charger_image_transparante(const char* nomfichier, SDL_Renderer* renderer,Uint8 r, Uint8 g, Uint8 b){
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