#ifndef OPTION_H
#define OPTION_H

#include <SDL.h>

void get_window_options();
void set_vsync(SDL_Renderer* renderer, SDL_bool state);

struct option
{
    /* data */
};

#endif