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


void compatible_sleep(unsigned long millisecond){ //be able to sleep on linux/windows
    #ifdef WIN32
        Sleep(millisecond); //millisecond
    #elif _POSIX_C_SOURCE >= 199309L
        struct timespec ts;
        ts.tv_sec = (int)(millisecond / 1000); // put everything above 1s
        ts.tv_nsec = (millisecond % 1000) * 1000000; //put the rest in nanosec
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


int main(int argc, char **argv) {    
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

    Uint64 last_time = SDL_GetPerformanceCounter(),current_time;
    double dt;
    while(!fin(world)){
        // Events
        handle_events(event,world,ressources);
        handle_keyboard_player(player);

        // Time / Physics
        current_time = SDL_GetPerformanceCounter();
        dt = (current_time - last_time) / (double)SDL_GetPerformanceFrequency();
        //printf("dt : %0.4f \n",dt);
        /* block fps (not needed currently because we're using vsync)
        if( dt < 16){
             compatible_sleep(1);
            continue;
        }
        */
        last_time = current_time;
        if(world->need_player_pos_update == 1){
            set_spawn(world,player);
            dt = 0.;
            world->start_level_time = SDL_GetPerformanceCounter();
        }
        if(world->game_state == Alive){
            move_player(player,world,ressources,dt);
        }

        // Rendering
        if(SDL_RenderClear(renderer) < 0){
            printf("Error clearing screen : %s \n",SDL_GetError());
        }
        render_sky(renderer,ressources);
        render_worlds(renderer,ressources,world);
        switch (world->game_state) {
            case Menu:
                render_main_menu_text(renderer,ressources);
                break;
            case Alive:
                //print_state(player); //debug
                world->end_level_time = SDL_GetTicks();
                render_player(renderer,player,ressources);
                break;
            case Dead:
                death(renderer,world,ressources);
                break;
            case Win:
                won(renderer,world,ressources);
                break;
            default:
                break;
        }
        SDL_RenderPresent(renderer);
        if (world->game_state == Dead) {
            compatible_sleep(2000);
            world->game_state = Menu; 
        }
        else if (world->game_state == Win || world->game_state == Finished) {
            compatible_sleep(2000);
            if(world->game_state == Win)
                world->game_state = Alive;
            else
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