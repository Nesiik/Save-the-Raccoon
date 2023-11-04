#include "option.h"

void get_window_options(){
    static int display_in_use = 0; /* Only using first display */

    int i, display_mode_count;
    SDL_DisplayMode mode;
    Uint32 f;
    display_mode_count = SDL_GetNumDisplayModes(display_in_use);
    if (display_mode_count < 1) {
        SDL_Log("SDL_GetNumDisplayModes failed: %s", SDL_GetError());
        //return 1;
    }

    SDL_Log("SDL_GetNumDisplayModes: %i", display_mode_count);

    for (i = 0; i < display_mode_count; ++i) {
        if (SDL_GetDisplayMode(display_in_use, i, &mode) != 0) {
            SDL_Log("SDL_GetDisplayMode failed: %s", SDL_GetError());
            //return 1;
        }
        f = mode.format;

        SDL_Log("Mode %i\tbpp %i\t%s\t%i x %i\t%iHz",
                i, SDL_BITSPERPIXEL(f),
                SDL_GetPixelFormatName(f),
                mode.w, mode.h,mode.refresh_rate);
    }
}

void set_vsync(SDL_Renderer* renderer, SDL_bool state){
    if(SDL_RenderSetVSync(renderer,state) != 0){
        SDL_Log("SDL_RenderSetVSync failed: %s", SDL_GetError());
        //return 1;
    }
}