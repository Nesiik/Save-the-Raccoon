#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include "sdl.h"
#include "option.h"
#include "player.h"

int main( int argc, char* args[] )
{    
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("%s",SDL_GetError());
    }
    if(TTF_Init() < 0){
        printf("%s",TTF_GetError());
    }
    if(IMG_Init(IMG_INIT_PNG) == 0){
        printf("%s",SDL_GetError());
    }

    SDL_Window* window = create_window();
    SDL_Renderer *renderer = create_renderer(window);
    SDL_Event* event = malloc(sizeof(SDL_Event));

    world_t* world = init_world();
    ressources_t* ressources = init_ressources(renderer);
    player_t* player = init_player(renderer);

    set_full_screen(window,FakeFS);
    SDL_SetHint (SDL_HINT_RENDER_SCALE_QUALITY, "0");

    Uint64 time = SDL_GetTicks64(),dt;
    while(!fin(world)){
        handle_events(event,world,ressources,player);
        if(SDL_RenderClear(renderer) < 0){
            printf("%s",SDL_GetError());
        }
        dt = SDL_GetTicks64() - time;
        render_sky(renderer,ressources);
        render_worlds(renderer,ressources,world);
        switch (world->game_state)
        {
            case Menu:
                render_main_menu_text(renderer,ressources);
                break;
            case Alive:
                render_player(renderer,player);
                //printf("%lu \n",dt);
                /*unsigned int time_limit; // a deplacer dans alive plus tard
                unsigned int timer = SDL_GetTicks();    
                unsigned int countdown = (time_limit - timer)/1000; 
                afficher_texte(renderer,"../assets/arial.ttf",countdown,900,600);   //a deplacer dans alive plus tard
                */
                break;
            default:
                break;
        }
        time = SDL_GetTicks64();
        SDL_RenderPresent(renderer);
    }

    free(event);
    free_levels(world);
    free(world);
    free_ressources(ressources);
    free(ressources);
    free_player(player);
    free(player);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();

    return EXIT_SUCCESS;
}