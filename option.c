#include "option.h"

static int gcd (int a, int b) {
    return (b == 0) ? a : gcd (b, a%b);
}

SDL_bool val_in_array(int* arr,int val){
    for (size_t i = 0; i < sizeof(arr) / sizeof(int); i++)
    {
        if(val == arr[i]){
            return SDL_TRUE;
        }
    }
    return SDL_FALSE;
}

void get_window_options(){
    static int display_in_use = 0; /* Only using first display */
    static int validW[2] = {16,4}; 
    static int validH[2] = {3,9}; 

    int i, display_mode_count,r,res_count;
    SDL_DisplayMode mode;
    Uint32 f;
    display_mode_count = SDL_GetNumDisplayModes(display_in_use);

    if (display_mode_count < 1) {
        SDL_Log("SDL_GetNumDisplayModes failed: %s", SDL_GetError());
        //return 1;
    }

    res_count = 0;
    SDL_Log("SDL_GetNumDisplayModes: %i", display_mode_count);

    for (i = 0; i < display_mode_count; ++i) {
        if (SDL_GetDisplayMode(display_in_use, i, &mode) != 0) {
            SDL_Log("SDL_GetDisplayMode failed: %s", SDL_GetError());
            //return 1;
        }
        f = mode.format;
        r = gcd(mode.w,mode.h);
        if(!val_in_array(validW,mode.w/r) || !val_in_array(validH,mode.h/r)){
            continue;
        }
        res_count++;
        SDL_Log("Res %i\tbpp %i\t%s\t%i x %i\t%iHz\t%i:%i",
                res_count, SDL_BITSPERPIXEL(f),
                SDL_GetPixelFormatName(f),
                mode.w, mode.h,mode.refresh_rate,
                mode.w/r,mode.h/r);
    }
}

void set_vsync(SDL_Renderer* renderer, SDL_bool state){
    if(SDL_RenderSetVSync(renderer,state) != 0){
        SDL_Log("SDL_RenderSetVSync failed: %s", SDL_GetError());
        //return 1;
    }
}