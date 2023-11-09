#include <SDL.h>
#include <SDL_ttf.h>
#include "sdl.h"
#include "option.h"

void init(SDL_Renderer * renderer, ressources_t *textures, world_t * world){
    get_window_options();
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

    init(renderer,ressources,world);
    while(!fin(world)){
        SDL_RenderClear(renderer);
        handle_events(event,world,ressources);
        switch (world->gameover)
        {
            case Menu:
                render_main_menu(renderer,ressources);
                break;
            /*case Alive:
                unsigned int time_limit; // a deplacer dans alive plus tard
                unsigned int timer = SDL_GetTicks();    
                unsigned int countdown = (time_limit - timer)/1000; 
                afficher_texte(renderer,"../assets/arial.ttf",countdown,900,600);   //a deplacer dans alive plus tard
                break;*/
            default:
                break;
        }
        SDL_RenderPresent(renderer);
    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    free(event);
    free(world);
    free(ressources);
    TTF_Quit();
    SDL_Quit();

    return 0;
}