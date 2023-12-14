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
#include "sdl.h"
#include "ressources.h"
#include "world.h"
#include "option.h"
#include "player.h"


void compatible_sleep(unsigned long millisecond){
    #ifdef WIN32
        Sleep(millisecond); //millisecond
    #elif _POSIX_C_SOURCE >= 199309L
        struct timespec ts;
        ts.tv_sec = (int)(millisecond / 1000);
        ts.tv_nsec = (millisecond % 1000) * 1000000;
        nanosleep(&ts, NULL);
    #else
        usleep(millisecond * 1000); //microseconds
    #endif
}

void handle_events(SDL_Event *event,world_t *world , ressources_t* ressources){
    while( SDL_PollEvent( event ) ) {
        if (event->type == SDL_QUIT || event->type == SDL_KEYDOWN || event->type == SDL_MOUSEBUTTONDOWN) {
            world_event(event,world,ressources);
        }
    }
}


int main() {    
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS ) < 0){
        printf("SDL_Init error : %s \n",SDL_GetError());
    }
    if(TTF_Init() < 0){
        printf("TTF_Init error : %s \n",TTF_GetError());
    }
    if(IMG_Init(IMG_INIT_PNG) == 0){
        printf("IMG_Init error : %s \n",SDL_GetError());
    }

    SDL_Window* window = create_window();
    SDL_Renderer *renderer = create_renderer(window);
    SDL_Event* event = malloc(sizeof(SDL_Event));

    world_t* world = init_world();
    ressources_t* ressources = init_ressources(renderer);
    player_t* player = init_player();

    set_full_screen(window,FakeFS);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

    Uint64 last_time = SDL_GetTicks(),current_time;
    double dt;
    while(!fin(world)){
        // Events
        handle_events(event,world,ressources);
        handle_keyboard_player(player);

        // Time / Physics
        current_time = SDL_GetTicks();
        dt = (current_time - last_time) / 1000.0;
        /* block fps (not needed currently because we're using vsync)
        if( dt < 16){
             compatible_sleep(1);
            continue;
        }
        */

        last_time = current_time;
        if(world->game_state == Alive){
            move_player(player,world,dt);
        }

        // Rendering
        if(SDL_RenderClear(renderer) < 0){
            printf("%s",SDL_GetError());
        }
        render_sky(renderer,ressources);
        render_worlds(renderer,ressources,world);
        switch (world->game_state) {
            case Menu:
                render_main_menu_text(renderer,ressources);
                break;
            case Alive:
                world->end_level_time = SDL_GetTicks();
                render_player(renderer,player,ressources);
                break;
            case Dead:
                death(renderer,world,ressources);
                break;
            default:
                break;
        }
        SDL_RenderPresent(renderer);
        if (world->game_state == Dead) {
            compatible_sleep(3000);
            world->game_state = Menu; 
        }
    }

    free(event);
    free_levels(world);
    free(world);
    free_ressources(ressources);
    free(ressources);
    free(player);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();

    return EXIT_SUCCESS;
}