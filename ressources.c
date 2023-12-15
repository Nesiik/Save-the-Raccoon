#include "ressources.h"
#include "sdl.h"

ressources_t* init_ressources(SDL_Renderer *renderer){
    ressources_t* ressources = malloc(sizeof(ressources_t));
    ressources->font = TTF_OpenFont("../assets/arial.ttf",42*2);
    if(ressources->font == NULL){
        printf("Error opening font : %s",TTF_GetError());
    }

    ressources->MenuItems = malloc(sizeof(MenuItem_t));

    int x=550,y=280;
    const char* str = "Jouer";
    for (unsigned char i = 0; i < MAIN_MENU_ITEM_COUNT; i++)
    {
        switch (i)
        {
            case 1:
                str = "Options";
                break;
            case 2:
                str = "Quitter";
                break;
            default:
                break;
        }
        ressources->MenuItems->ItemList[i].rect = (SDL_Rect){x,y,0,0};
        ressources->MenuItems->ItemList[i].text = SDL_strdup(str);
        ressources->MenuItems->ItemList[i].texture = creer_texte_texture(renderer,ressources->font,2,NULL,str, ressources->MenuItems->ItemList[i].rect.x, ressources->MenuItems->ItemList[i].rect.y,&ressources->MenuItems->ItemList[i].rect);
        y+=50;
    }
    

    ressources->MenuItems->curselectedItem = -1;
    ressources->MenuItems->lastselectedItem = -1;

    ressources->dirt = charger_image_png("../assets/dirt_sprite.png",renderer);
    ressources->sky = charger_image_png("../assets/clouds2.1Large(1).png",renderer);
    ressources->spike = charger_image_png("../assets/spikeV4.png",renderer);
    ressources->tree = charger_image_png("../assets/treeV2.png", renderer);
    ressources->flag = charger_image_png("../assets/flagV4.png", renderer);
    ressources->coin = charger_image_png("../assets/coinV2.png", renderer);
    ressources->player = charger_image_png("../assets/raccoon/staticv2.png", renderer);
    ressources->player->sprite_w = PLAYER_SPRITE_SIZE;
    ressources->player->sprite_h = PLAYER_SPRITE_SIZE;

    return ressources;
}

void free_ressources(ressources_t* ressources){
    for (size_t i = 0; i < MAIN_MENU_ITEM_COUNT; i++)
    {
        free(ressources->MenuItems->ItemList[i].text);
        SDL_DestroyTexture(ressources->MenuItems->ItemList[i].texture);
    }
    free(ressources->MenuItems);

    SDL_DestroyTexture(ressources->dirt->text);
    free(ressources->dirt);

    SDL_DestroyTexture(ressources->spike->text);
    free(ressources->spike);

    SDL_DestroyTexture(ressources->sky->text);
    free(ressources->sky);

    SDL_DestroyTexture(ressources->tree->text);
    free(ressources->tree);

    SDL_DestroyTexture(ressources->flag->text);
    free(ressources->flag);

    SDL_DestroyTexture(ressources->coin->text);
    free(ressources->coin);

    SDL_DestroyTexture(ressources->player->text);
    free(ressources->player);

    TTF_CloseFont(ressources->font);
}