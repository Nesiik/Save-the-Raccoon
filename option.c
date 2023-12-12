#include "option.h"

static Uint16 gcd (Uint16 a, Uint16 b) {
    //return (b == 0) ? a : gcd (b, a%b);
    while (a != 0 && b != 0)
    {
        if (a > b)
            a %= b;
        else
            b %= a;
    }
    return a | b;
}

char val_in_array(int* arr,int arr_size,int val){
    for (int i = 0; i < arr_size; i++)
    {
        if(val == arr[i]){
            return 1;
        }
    }
    return 0;
}

void get_window_options(){
    static int display_in_use = 0; /* Only using first display */
    static int validW[] = {16}; 
    static int validH[] = {9}; 

    int i, display_mode_count,r,res_count;
    SDL_DisplayMode mode;
    Uint32 f;
    display_mode_count = SDL_GetNumDisplayModes(display_in_use);

    if (display_mode_count < 1) {
        SDL_Log("SDL_GetNumDisplayModes failed: %s", SDL_GetError());
    }

    res_count = 0;
    SDL_Log("SDL_GetNumDisplayModes: %i", display_mode_count);

    for (i = 0; i < display_mode_count; ++i) {
        if (SDL_GetDisplayMode(display_in_use, i, &mode) != 0) {
            SDL_Log("SDL_GetDisplayMode failed: %s", SDL_GetError());
        }
        f = mode.format;
        r = gcd(mode.w,mode.h);
        if(!val_in_array(validW,1,mode.w/r) || !val_in_array(validH,1,mode.h/r)){
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
    /*
    if(SDL_RenderSetVSync(renderer,state) != 0){
        SDL_Log("SDL_RenderSetVSync failed: %s", SDL_GetError());
    }
    */
}

void set_full_screen(SDL_Window* window, Uint32 flags){ //FullScreenFlags
    if(SDL_SetWindowFullscreen(window,flags)){
        SDL_Log("SDL_SetWindowFullscreen failed: %s",SDL_GetError());
    }
}

/*
void set_resolution(screen_state* screen,int w, int h){
    if(screen->fullscreen > 0){ //fullscreen
        //SDL_SetWindowDisplayMode();
    }
    else{
        //SDL_SetWindowSize();
    }
    
}
*/