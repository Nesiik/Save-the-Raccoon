#include "sdl.h"

//For text that should be display 1 time
void afficher_texte(SDL_Renderer* renderer, TTF_Font* police,char type ,const char text[], int x, int y ) {
    if(text == NULL){
        printf("Error : no text provided \n");
        return;
    }
    SDL_Rect dest;
    SDL_Texture* texture = creer_texte_texture(renderer,police,type,NULL,text,x,y,&dest);
   	if(texture != NULL){
        if(SDL_RenderCopy(renderer, texture, NULL, &dest) < 0){
            printf("Error rendering text : %s \n",SDL_GetError());
        }
        SDL_DestroyTexture(texture);
    }
}

//renvoie la surface pour du texte
SDL_Surface* texte_surface(TTF_Font* police,const char text[],char type,SDL_Color* color){
    SDL_Surface* surf;
    if(color == NULL){
        color = &(SDL_Color){ 100, 100, 100, 255 };
    }
    switch (type) { //https://www.libsdl.org/projects/old/SDL_ttf/docs/SDL_ttf.html#SEC42
        case 0: //fast 
            surf = TTF_RenderText_Solid(police,text,*color);
            break;
        case 2: //slow + better looking
            surf = TTF_RenderText_Blended(police,text,*color);
            break;
        default:
            break;
    }
    if (!surf) {
        printf("Error creating text surface : %s \n",TTF_GetError());
    }
    return surf;
}

//creer texture texte et rempli un SDL_rect (si non null)
SDL_Texture* creer_texte_texture(SDL_Renderer* renderer, TTF_Font* police,char type,SDL_Color* color, const char text[],int x, int y, SDL_Rect* info){
    SDL_Surface* surf = texte_surface(police,text,type,color);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
    if(text == NULL){
        printf("Error creating texture from text surface : %s \n",SDL_GetError());
        return NULL;
    }
    if(info != NULL){
        info->x = x;
        info->y = y;
        info->w = surf->w/2;
        info->h = surf->h/2;
    }
    SDL_FreeSurface(surf);
    return tex;
}

SDL_Window* create_window(){
    SDL_Window* window = SDL_CreateWindow(
    "Save the Raccoon",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    WINDOW_WIDTH, 
    WINDOW_HEIGHT, 
    SDL_WINDOW_SHOWN);
    if(window == NULL)
    {
        SDL_Log("Error creating windows : %s", SDL_GetError());
    }
    return window;
}

SDL_Renderer* create_renderer(SDL_Window* window){
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    if(renderer == NULL)
    {
        SDL_Log("Error creating renderer : %s", SDL_GetError());
    }
    if(SDL_RenderSetLogicalSize(renderer,WINDOW_WIDTH,WINDOW_HEIGHT) < 0){
        SDL_Log("%s",SDL_GetError());
    }
    return renderer;
}
