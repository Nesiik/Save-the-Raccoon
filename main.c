#include <SDL.h>
#include <SDL_ttf.h>
#include "world.h"
#include "sdl.h"

void init(SDL_Window **window, SDL_Renderer ** renderer, ressources_t *textures, world_t * world){
    init_sdl(window,renderer,1280, 720);
    init_data(world);
    init_ressources(*renderer, textures);
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

int main( int argc, char* args[] )
{
    SDL_Init(SDL_INIT_VIDEO);

    TTF_Init();

    SDL_Window *window = SDL_CreateWindow(
        "SDL2Test",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        1280, 
        720, 
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);


    SDL_Event event;
    world_t world;
    ressources_t ressources;

    init(&window,&renderer,&ressources,&world);
    while(!is_game_over(&world)){
        handle_events(&event,&world);
        SDL_Delay(10);
    }

    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}