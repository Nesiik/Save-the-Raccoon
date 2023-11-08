#include "sprite.h"

sprite_t* charger_image (const char* nomfichier, SDL_Renderer* renderer){
    sprite_t* sprite = malloc(sizeof(sprite_t));
    SDL_Surface* bmp = SDL_LoadBMP(nomfichier);
    sprite->text = SDL_CreateTextureFromSurface(renderer, bmp);
    if (sprite->text == NULL){
        printf("%s \n",SDL_GetError());
    }
    SDL_QueryTexture(sprite->text,NULL,NULL,&sprite->src.w,&sprite->src.h);
    sprite->dest.w = sprite->src.w;
    sprite->dest.h = sprite->dest.h;
    SDL_FreeSurface(bmp);
    return sprite;
}

sprite_t* charger_image_transparante(const char* nomfichier, SDL_Renderer* renderer,Uint8 r, Uint8 g, Uint8 b){
    sprite_t* sprite = malloc(sizeof(sprite_t));
    SDL_Surface* bmp = SDL_LoadBMP(nomfichier);
    if(SDL_SetColorKey(bmp,SDL_TRUE,SDL_MapRGB(bmp->format,r,g,b))){
        printf("%s \n",SDL_GetError());
    }
    sprite->text = SDL_CreateTextureFromSurface(renderer, bmp);
        if (sprite->text == NULL){
        printf("%s \n",SDL_GetError());
    }
    SDL_QueryTexture(sprite->text,NULL,NULL,&sprite->src.w,&sprite->src.h);
    sprite->dest.w = sprite->src.w;
    sprite->dest.h = sprite->dest.h;
    SDL_FreeSurface(bmp);
    return sprite;

}