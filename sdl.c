#include "sdl.h"
#include "sprite.h"
#include "fonctions_fichiers.h"


void mouse_menu_events(SDL_MouseButtonEvent button,world_t* world, ressources_t* ressources){
    int mouseX = button.x;
    int mouseY = button.y;

    for (unsigned char i = 0; i < MAIN_MENU_ITEM_COUNT; i++) {
        Item menuItem = ressources->MenuItems.ItemList[i];

        if (mouseX >= menuItem.rect.x && mouseX <= menuItem.rect.x + menuItem.rect.w &&
            mouseY >= menuItem.rect.y && mouseY <= menuItem.rect.y + menuItem.rect.h)
        {
            ressources->MenuItems.selectedItem = i;
            switch (i) {
                case 0: // Jouer
                    //init_data(world);
                    //world->gameover = Alive;
                    //world->depart = SDL_GetTicks();
                    break;
                case 1: // Option
                    break;
                case 2: // Quitter
                    world->gameover = Quit;
                    break;
                
                default:
                    break;
            }
        }
    }
}

void handle_events(SDL_Event *event,world_t *world , ressources_t* ressources, player_t * player){
    while( SDL_PollEvent( event ) ) {
        switch (event->type){
        case SDL_QUIT:
            world->gameover = Quit;
        break;
        case SDL_KEYDOWN:
            if(world->gameover == Menu){
                if(event->key.keysym.sym == SDLK_ESCAPE){
                    world->gameover = Quit;
                }
            }
            if(world->gameover == Alive){
                deplacement(player, event->key.keysym.sym);
            }
        break;
        case SDL_MOUSEBUTTONDOWN:
            switch (world->gameover)
            {
            case Menu:
                if (event->button.button == SDL_BUTTON_LEFT)
                {
                    mouse_menu_events(event->button,world,ressources);
                }
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }
}


void render_main_menu_text(SDL_Renderer *renderer,ressources_t *ressources){
    for (int i = 0; i < MAIN_MENU_ITEM_COUNT; i++) {
        SDL_Color color;

        if(i == ressources->MenuItems.selectedItem){
            color = (SDL_Color){ 180, 180, 180 };
        }
        else{
            color = (SDL_Color){ 100, 100, 100 };
        }

        SDL_Surface* surface = TTF_RenderText_Solid(ressources->font, ressources->MenuItems.ItemList[i].text,  color);
        if(surface == NULL){
            printf("%s",SDL_GetError());
        }
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        if(texture == NULL){
            printf("%s",SDL_GetError());
        }

        if(ressources->MenuItems.ItemList[i].rect.w == 0 || ressources->MenuItems.ItemList[i].rect.h == 0){
            ressources->MenuItems.ItemList[i].rect.w = surface->w;
            ressources->MenuItems.ItemList[i].rect.h = surface->h;
        }

        if(SDL_RenderCopy(renderer, texture, NULL, &ressources->MenuItems.ItemList[i].rect) < 0){
            printf("%s",SDL_GetError());
        }

        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }
}

void render_main_menu_background(SDL_Renderer* renderer,ressources_t* ressources){
    if(ressources->back_info == NULL){
        ressources->back_info = lire_fichier("../menu_background.txt");
        if(ressources->back_info == NULL){
            SDL_Log("Erreur lecture fichier \n");
            return;
        }
        taille_fichier("../menu_background.txt",&ressources->back_ligne,&ressources->back_col);
    }
    int spriteW = ressources->background->src.w;
    int spriteH = ressources->background->src.h;
    ressources->background->dest.w = ressources->background->src.w;
    ressources->background->dest.h = ressources->background->src.h;
    for (unsigned int i = 0; i < ressources->back_ligne; i++)
    {
        for (unsigned int j = 0; j < ressources->back_col; j++)
        {
            int tabij = ressources->back_info[i][j] - '0'; // conversion ascii -> int
            ressources->background->src.x = tabij*spriteW + (tabij+1);
            ressources->background->src.y = 0;
            ressources->background->dest.x = j*spriteW;
            ressources->background->dest.y = i*spriteH;
            if(SDL_RenderCopy(renderer,ressources->background->text,&ressources->background->src,&ressources->background->dest)<0){
                SDL_Log("Erreur : %s",SDL_GetError());
            }
        }
    }
}

/*void render_player(SDL_Renderer* renderer, ressources_t * ressources, player_t* player){

}*/

void afficher_texte(SDL_Renderer* renderer, TTF_Font* police, const char text[], int x, int y ) {
	SDL_Color fg = { 255, 255, 255 };
	SDL_Surface* surf = TTF_RenderText_Solid(police,text, fg);
        if(surf == NULL){
        printf("%s",SDL_GetError());
    }
    SDL_Rect dest = {x,y,surf->w,surf->h};
	SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
    if(text == NULL){
        printf("%s",SDL_GetError());
    }

	SDL_RenderCopy(renderer, tex, NULL, &dest);
	SDL_DestroyTexture(tex);
	SDL_FreeSurface(surf);
}

SDL_Window* create_window(){
    SDL_Window* window = SDL_CreateWindow(
    "Save the Raccoon",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    1280, 
    720, 
    SDL_WINDOW_SHOWN);
    if(window == NULL)
    {
        SDL_Log("Erreur lors de la creation de l'image : %s", SDL_GetError());
    }
    return window;
}

SDL_Renderer* create_renderer(SDL_Window* window){
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if(renderer == NULL)
    {
        SDL_Log("Erreur lors de la creation du renderer : %s", SDL_GetError());
    }
    SDL_RenderSetLogicalSize(renderer,1280,720);
    return renderer;
}


void init_ressources(SDL_Renderer *renderer, ressources_t* ressources){
    ressources->font = TTF_OpenFont("../assets/arial.ttf",28);
    if(ressources->font == NULL){
        printf("%s",SDL_GetError());
    }

    ressources->MenuItems.ItemList[0].rect = (SDL_Rect){100,200,0,0};
    const char* str = "Jouer";
    ressources->MenuItems.ItemList[0].text = SDL_strdup(str);
    ressources->MenuItems.ItemList[1].rect = (SDL_Rect){100,250,0,0};
    str = "Options";
    ressources->MenuItems.ItemList[1].text = SDL_strdup(str);
    ressources->MenuItems.ItemList[2].rect = (SDL_Rect){100,300,0,0};
    str = "Quitter";
    ressources->MenuItems.ItemList[2].text = SDL_strdup(str);

    ressources->MenuItems.selectedItem = -1;

    ressources->background = charger_image_png("../assets/dirt_sprite.png",renderer);
    ressources->background->src.w = (ressources->background->src.w/6)-1;
    ressources->background->src.h = (ressources->background->src.h/5)-1;
    ressources->back_info = NULL;
}

void free_ressources(ressources_t* ressources){
    for (size_t i = 0; i < MAIN_MENU_ITEM_COUNT; i++)
    {
        free(ressources->MenuItems.ItemList[i].text);
    }
    SDL_DestroyTexture(ressources->background->text);
    free(ressources->background);
    desallouer_tab_2D(ressources->back_info,ressources->back_ligne);
}