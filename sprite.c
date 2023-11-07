#include "sprite.h"

SDL_Texture* charger_image (const char* nomfichier, SDL_Renderer* renderer){
    SDL_Surface* bmp = SDL_LoadBMP(nomfichier);
    SDL_Texture* text = SDL_CreateTextureFromSurface(renderer, bmp);
    if (text == NULL){
        printf("%s \n",SDL_GetError());
    }
    return text;
}

SDL_Texture* charger_image_transparante(const char* nomfichier, SDL_Renderer* renderer,Uint8 r, Uint8 g, Uint8 b){
    SDL_Surface* bmp = SDL_LoadBMP(nomfichier);
    int res = SDL_SetColorKey(bmp,SDL_TRUE,SDL_MapRGB(bmp->format,r,g,b));
    SDL_Texture* text = SDL_CreateTextureFromSurface(renderer, bmp);
        if (text == NULL){
        printf("%s \n",SDL_GetError());
    }
    return text;

}