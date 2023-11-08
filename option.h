#ifndef OPTION_H
#define OPTION_H

#include <SDL.h>

enum FullScreenState {
    None = 0,
    RealFS = SDL_WINDOW_FULLSCREEN,
    FakeFS = SDL_WINDOW_FULLSCREEN_DESKTOP
};

struct option
{
    /* data */
};

void get_window_options();
void set_vsync(SDL_Renderer* renderer, SDL_bool state);
void set_full_screen(SDL_Window* window, Uint32 flags);

#endif