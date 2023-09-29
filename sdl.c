#include "sdl.h"

void init_ressources(SDL_Renderer *renderer, ressources_t* ressources){
    ressources->MenuItems.ItemList[0].rect = (SDL_Rect){100,200,0,0};
    ressources->MenuItems.ItemList[0].text = "Jouer";
    ressources->MenuItems.ItemList[1].rect = (SDL_Rect){100,250,0,0};
    ressources->MenuItems.ItemList[1].text = "Option";
    ressources->MenuItems.ItemList[2].rect = (SDL_Rect){100,300,0,0};
    ressources->MenuItems.ItemList[2].text = "Quitter";

    ressources->MenuItems.numMenuItems = 3;
    ressources->MenuItems.selectedItem = -1;
}

void handle_events(SDL_Event *event,world_t *world){
    while( SDL_PollEvent( event ) ) {
        if( event->type == SDL_QUIT ) {
            world->gameover = ForceQuit;
        }
        if(event->type == SDL_KEYDOWN){
            if(event->key.keysym.sym == SDLK_ESCAPE){
                world->gameover = ForceQuit;
            }
        }
    }
}

SDL_Window* create_window(){
    SDL_Window* window = SDL_CreateWindow(
    "SDL2Test",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    1280, 
    720, 
    SDL_WINDOW_SHOWN);
    if(window == NULL)
    {
        printf("Erreur lors de la creation de l'image : %s", SDL_GetError());
    }
    return window;
}

SDL_Renderer* create_renderer(SDL_Window* window){
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if(renderer == NULL)
    {
        printf("Erreur lors de la creation du renderer : %s", SDL_GetError());
    }
    if(0 != SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE))
    {
        printf("Erreur lors de la creation du renderer : %s", SDL_GetError());
    }
    return renderer;
}