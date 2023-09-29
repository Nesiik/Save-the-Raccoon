#include <SDL.h>
#include <SDL_ttf.h>
#include "sdl.h"

void init(SDL_Renderer * renderer, ressources_t *textures, world_t * world){
    init_data(world);
    init_ressources(renderer, textures);
}

int main( int argc, char* args[] )
{
    SDL_Event* event = malloc(sizeof(SDL_Event));
    world_t* world = malloc(sizeof(world_t));
    ressources_t* ressources = malloc(sizeof(ressources_t));
    
    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
    TTF_Init();

    SDL_Window* window = create_window();

    SDL_Renderer *renderer = create_renderer(window);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    init_data(world);
    while(!fin(world)){
        handle_events(event,world);
        SDL_Delay(10);
    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    free(event);
    free(world);
    free(ressources);
    TTF_Quit();
    SDL_Quit();

    return 0;
}