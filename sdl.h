#ifndef SDL_H
#define SDL_H

#include <SDL.h>
#include <SDL_ttf.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

enum Text_type{
    FAST_TEXT = 0,
    SLOW_TEXT = 2
};

SDL_Window* create_window();
SDL_Renderer* create_renderer(SDL_Window* window);
void afficher_texte(SDL_Renderer* renderer, TTF_Font* police,char type ,const char text[], int x, int y );
SDL_Surface* texte_surface(TTF_Font* police,const char text[],char type,SDL_Color* color);
SDL_Texture* creer_texte_texture(SDL_Renderer* renderer, TTF_Font* police,char type,SDL_Color* color, const char text[],int x, int y, SDL_Rect* info);

#endif