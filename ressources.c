#include "ressources.h"

void render_main_menu_text(SDL_Renderer *renderer,ressources_t *ressources){
    for (unsigned char i = 0; i < MAIN_MENU_ITEM_COUNT; i++) {
        if(i == ressources->MenuItems->curselectedItem){
            if (ressources->MenuItems->curselectedItem != ressources->MenuItems->lastselectedItem){
                SDL_Color color = (SDL_Color){ 180, 180, 180, 255 };
                //Bouton sélectionné
                SDL_DestroyTexture(ressources->MenuItems->ItemList[i].texture);
                ressources->MenuItems->ItemList[i].texture = creer_texte_texture(renderer,ressources->font,2,&color,ressources->MenuItems->ItemList[i].text,ressources->MenuItems->ItemList[i].rect.x,ressources->MenuItems->ItemList[i].rect.y,&ressources->MenuItems->ItemList[i].rect);
                if(ressources->MenuItems->lastselectedItem != -1){ // si il y a eu une sélection avant cette sélection
                    color = (SDL_Color){ 100, 100, 100, 255 };
                    signed char lastItem = ressources->MenuItems->lastselectedItem;
                    SDL_DestroyTexture(ressources->MenuItems->ItemList[lastItem].texture);
                    ressources->MenuItems->ItemList[lastItem].texture = creer_texte_texture(renderer,ressources->font,2,&color,ressources->MenuItems->ItemList[lastItem].text,ressources->MenuItems->ItemList[lastItem].rect.x,ressources->MenuItems->ItemList[lastItem].rect.y,&ressources->MenuItems->ItemList[lastItem].rect);
                    ressources->MenuItems->lastselectedItem = ressources->MenuItems->curselectedItem;
                }
                ressources->MenuItems->lastselectedItem = ressources->MenuItems->curselectedItem;
            }
            
        }
        if(SDL_RenderCopy(renderer, ressources->MenuItems->ItemList[i].texture, NULL, &ressources->MenuItems->ItemList[i].rect) < 0){
            printf("%s",SDL_GetError());
        }
    }
}

ressources_t* init_ressources(SDL_Renderer *renderer){
    ressources_t* ressources = malloc(sizeof(ressources_t));
    ressources->font = TTF_OpenFont("../assets/arial.ttf",42*2);
    if(ressources->font == NULL){
        printf("%s",SDL_GetError());
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

    TTF_CloseFont(ressources->font);
}