#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#ifdef WIN32
#include <windows.h>
#elif _POSIX_C_SOURCE >= 199309L
#include <time.h>   // for nanosleep
#else
#include <unistd.h> // for usleep
#endif
#include "world.h"
#include "option.h"
#include "player.h"


void handle_events(SDL_Event *event,world_t *world , ressources_t* ressources, player_t * player){
    while( SDL_PollEvent( event ) ) {
        switch (event->type){
        case SDL_QUIT:
            world->game_state = Quit;
        break;
        case SDL_KEYDOWN:
            if(world->game_state == Menu){
                if(event->key.keysym.sym == SDLK_ESCAPE){
                    world->game_state = Quit;
                }
            }
            if(world->game_state == Alive){
                if(event->key.keysym.sym == SDLK_ESCAPE){
                    world->game_state = Menu;
                    world->last_level = world->cur_level;
                    world->cur_level = 0;
                }else{
                    if(event->key.keysym.sym == SDLK_q || event->key.keysym.sym == SDLK_d || event->key.keysym.sym == SDLK_s || event->key.keysym.sym == SDLK_z ){
                        /*
                        switch (event->key.keysym.sym)
                        {
                        case SDLK_q:
                            set_state(player,WALK,1);
                            break;
                        case SDLK_d:
                            set_state(player,WALK,0);
                            break;
                        case SDLK_z:
                            set_state(player,FALL,0);
                        default:
                            break;
                        }
                        */
                        deplacement(player, world,event->key.keysym.sym);
                    }
                }
            }
        break;
        case SDL_KEYUP:
        break;
        case SDL_MOUSEBUTTONDOWN:
            switch (world->game_state)
            {
            case Menu:
                if (event->button.button == SDL_BUTTON_LEFT)
                {
                    mouse_menu_events(event->button,world,ressources);
                }
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }
}


int main()
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

    Uint64 last_time = SDL_GetTicks64(),current_time;
    double dt;
    while(!fin(world)){
        // Events
        handle_events(event,world,ressources,player);

        current_time = SDL_GetTicks64();
        dt = (current_time - last_time) / 1000.0;
        /**
        if( dt < 16){
            #ifdef WIN32
                Sleep(1);
            #elif _POSIX_C_SOURCE >= 199309L
                struct timespec ts;
                ts.tv_sec = 1 / 1000;
                ts.tv_nsec = (1 % 1000) * 1000000;
                nanosleep(&ts, NULL);
            #else
                usleep((1 % 1000) * 1000);
            #endif
            continue;
        }
        */

        last_time = current_time;

        // Rendering
        if(SDL_RenderClear(renderer) < 0){
            printf("%s",SDL_GetError());
        }
        render_sky(renderer,ressources);
        render_worlds(renderer,ressources,world);
        switch (world->game_state)
        {
            case Menu:
                render_main_menu_text(renderer,ressources);
                break;
            case Alive:
                render_player(renderer,player);
                printf("%f \n",dt);
                /*unsigned int time_limit; // a deplacer dans alive plus tard
                unsigned int timer = SDL_GetTicks();    
                unsigned int countdown = (time_limit - timer)/1000; 
                afficher_texte(renderer,"../assets/arial.ttf",countdown,900,600);   //a deplacer dans alive plus tard
                */
                break;
            default:
                break;
        }
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