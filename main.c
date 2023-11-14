#include <SDL.h>
#include <SDL_ttf.h>
#include "sdl.h"
#include "option.h"
#include "player.h"

void init(SDL_Renderer * renderer, ressources_t *textures, world_t * world, player_t * player){
    //get_window_options();
    init_data(world);
    init_ressources(renderer, textures);
    init_player(player);
}

int main( int argc, char* args[] )
{
    SDL_Event* event = malloc(sizeof(SDL_Event));
    world_t* world = malloc(sizeof(world_t));
    ressources_t* ressources = malloc(sizeof(ressources_t));
    player_t* player = malloc(sizeof(player_t));
    
    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) < 0){
        printf("%s",SDL_GetError());
    }
    if(TTF_Init() < 0){
        printf("%s",SDL_GetError());
    }
    if(IMG_Init(IMG_INIT_PNG) == 0){
        printf("%s",SDL_GetError());
    }

    SDL_Window* window = create_window();
    SDL_Renderer *renderer = create_renderer(window);

    init(renderer,ressources,world,player);
    while(!fin(world)){
        if(SDL_RenderClear(renderer) < 0){
            printf("%s",SDL_GetError());
        }
        handle_events(event,world,ressources,player);
        switch (world->gameover)
        {
            case Menu:
                render_main_menu_background(renderer,ressources);
                render_main_menu_text(renderer,ressources);
                break;
            case Alive:

                /*unsigned int time_limit; // a deplacer dans alive plus tard
                unsigned int timer = SDL_GetTicks();    
                unsigned int countdown = (time_limit - timer)/1000; 
                afficher_texte(renderer,"../assets/arial.ttf",countdown,900,600);   //a deplacer dans alive plus tard
                break;*/
            default:
                break;
        }
        SDL_RenderPresent(renderer);
    }
    
    TTF_CloseFont(ressources->font);
    free(event);
    free(world);
    free_ressources(ressources);
    free(ressources);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();

    return 0;
}