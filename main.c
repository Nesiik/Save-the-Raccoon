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

    Uint64 lastUp,end,delta;
    double MSPerFrame,fps;

    init(renderer,ressources,world);

    //lastUp = SDL_GetTicks64();

    while(!fin(world)){
        SDL_RenderClear(renderer);
        handle_events(event,world,ressources);
        /*
        for (size_t i = 0; i < 1000; i++)
        {           
            SDL_Rect fillRect = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
            SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE );        
            SDL_RenderFillRect( renderer, &fillRect );
            SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE );  
        }
        */
        /*
        end = SDL_GetTicks64();

        delta = end - lastUp;
        printf("%lu , %lu \n", lastUp,end); 
        int taille = snprintf(NULL,0, "%lu", delta);
        char* str = malloc(taille + 1);
        snprintf(str,taille + 1, "%lu", delta);
        afficher_texte(renderer,font,str,0,0);
        SDL_RenderPresent(renderer);
        lastUp = SDL_GetTicks64();
        */
        switch (world->gameover)
        {
        case Menu:
            render_menu(renderer,ressources);
            break;
        default:
            break;
        }
        SDL_RenderPresent(renderer);
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