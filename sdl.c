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
            ressources->MenuItems.lastselectedItem = ressources->MenuItems.curselectedItem;
            ressources->MenuItems.curselectedItem = i;
            switch (i) {
                case 0: // Jouer
                    //init_data(world);
                    world->gameover = Alive;
                    if(world->cur_level == 0)
                        world->cur_level = 1;
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
                if(event->key.keysym.sym == SDLK_ESCAPE){
                    world->gameover = Menu;
                    world->cur_level = 0;
                }else{
                    deplacement(player, event->key.keysym.sym);
                }
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
        if(i == ressources->MenuItems.curselectedItem){
            if (ressources->MenuItems.curselectedItem != ressources->MenuItems.lastselectedItem){
                SDL_Color color = (SDL_Color){ 180, 180, 180 };
                //Bouton sélectionné
                SDL_DestroyTexture(ressources->MenuItems.ItemList[i].texture);
                ressources->MenuItems.ItemList[i].texture = creer_texte_texture(renderer,ressources->font,&color,ressources->MenuItems.ItemList[i].text,ressources->MenuItems.ItemList[i].rect.x,ressources->MenuItems.ItemList[i].rect.y,&ressources->MenuItems.ItemList[i].rect);
                if(ressources->MenuItems.lastselectedItem != -1){ // si il y a eu une sélection avant cette sélection
                    color = (SDL_Color){ 100, 100, 100 };
                    char lastItem = ressources->MenuItems.lastselectedItem;
                    SDL_DestroyTexture(ressources->MenuItems.ItemList[lastItem].texture);
                    ressources->MenuItems.ItemList[lastItem].texture = creer_texte_texture(renderer,ressources->font,&color,ressources->MenuItems.ItemList[lastItem].text,ressources->MenuItems.ItemList[lastItem].rect.x,ressources->MenuItems.ItemList[lastItem].rect.y,&ressources->MenuItems.ItemList[lastItem].rect);
                    ressources->MenuItems.lastselectedItem = ressources->MenuItems.curselectedItem;
                }
                ressources->MenuItems.lastselectedItem = ressources->MenuItems.curselectedItem;
            }
            
        }
        if(SDL_RenderCopy(renderer, ressources->MenuItems.ItemList[i].texture, NULL, &ressources->MenuItems.ItemList[i].rect) < 0){
            printf("%s",SDL_GetError());
        }
    }
}

void render_worlds(SDL_Renderer* renderer,ressources_t* ressources,world_t* world){
    int level = world->cur_level;
    int spriteW = ressources->background->src.w;
    int spriteH = ressources->background->src.h;
    ressources->background->dest.w = ressources->background->src.w;
    ressources->background->dest.h = ressources->background->src.h;
    for (unsigned int i = 0; i < world->levels[level].nb_ligne_level_tab; i++)
    {
        for (unsigned int j = 0; j < world->levels[level].nb_col_level_tab; j++)
        {
            char cur_char = world->levels[level].level_tab[i][j];
            if (cur_char == 39) //un trou
            {
                continue;
            }
            else if(cur_char > 64 && cur_char < 91){ //dirt
                int tabij = cur_char - 'A'; // conversion ascii -> int
                int dirt_y = tabij/6;
                int dirt_x = tabij - dirt_y*6;
                ressources->background->src.x = dirt_x*spriteW + (dirt_x+1);
                ressources->background->src.y = dirt_y*spriteW + (dirt_y+1);
                ressources->background->dest.x = j*spriteW;
                ressources->background->dest.y = i*spriteH;
                if(SDL_RenderCopy(renderer,ressources->background->text,&ressources->background->src,&ressources->background->dest)<0){
                    SDL_Log("Erreur : %s",SDL_GetError());
                }
            }
        }
    }
}

//Fait pour afficher du texte qui change beaucoup
void afficher_texte(SDL_Renderer* renderer, TTF_Font* police, const char text[], int x, int y ) {
    SDL_Rect dest;
    SDL_Texture* tex = creer_texte_texture(renderer,police,NULL,text,x,y,&dest);
   	SDL_RenderCopy(renderer, tex, NULL, &dest);
	SDL_DestroyTexture(tex);
}

//creer texture texte et rempli un SDL_rect (si non null)
SDL_Texture* creer_texte_texture(SDL_Renderer* renderer, TTF_Font* police,SDL_Color* color, const char text[],int x, int y, SDL_Rect* info){
    SDL_Surface* surf;
    if(color == NULL){
        surf = TTF_RenderText_Solid(police,text, (SDL_Color){ 100, 100, 100 });
    }else{
        surf = TTF_RenderText_Solid(police,text, *color);
    }
    if(surf == NULL){
        printf("%s",SDL_GetError());
        return NULL;
    }
	SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
    if(text == NULL){
        printf("%s",SDL_GetError());
        return NULL;
    }
    if(info != NULL){
        info->x = x;
        info->y = y;
        info->w = surf->w;
        info->h = surf->h;
    }
    SDL_FreeSurface(surf);
    return tex;
}

SDL_Window* create_window(){
    SDL_Window* window = SDL_CreateWindow(
    "Save the Raccoon",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    WINDOW_WIDTH, 
    WINDOW_HEIGHT, 
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
    if(SDL_RenderSetLogicalSize(renderer,WINDOW_WIDTH,WINDOW_HEIGHT) < 0){
        SDL_Log("%s",SDL_GetError());
    }
    return renderer;
}


void init_ressources(SDL_Renderer *renderer, ressources_t* ressources){
    ressources->font = TTF_OpenFont("../assets/arial.ttf",36);
    if(ressources->font == NULL){
        printf("%s",SDL_GetError());
    }

    int x=30,y=300;
    const char* str = "Jouer";
    for (char i = 0; i < MAIN_MENU_ITEM_COUNT; i++)
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
        ressources->MenuItems.ItemList[i].rect = (SDL_Rect){x,y,0,0};
        ressources->MenuItems.ItemList[i].text = SDL_strdup(str);
        ressources->MenuItems.ItemList[i].texture = creer_texte_texture(renderer,ressources->font,NULL,str, ressources->MenuItems.ItemList[i].rect.x, ressources->MenuItems.ItemList[i].rect.y,&ressources->MenuItems.ItemList[i].rect);
        y+=50;
    }
    

    ressources->MenuItems.curselectedItem = -1;
    ressources->MenuItems.lastselectedItem = -1;

    ressources->background = charger_image_png("../assets/dirt_sprite.png",renderer);
    ressources->background->src.w = (ressources->background->src.w/6)-1;
    ressources->background->src.h = (ressources->background->src.h/5)-1;
}

void free_ressources(ressources_t* ressources){
    for (size_t i = 0; i < MAIN_MENU_ITEM_COUNT; i++)
    {
        free(ressources->MenuItems.ItemList[i].text);
        SDL_DestroyTexture(ressources->MenuItems.ItemList[i].texture);
    }
    SDL_DestroyTexture(ressources->background->text);
    free(ressources->background);
}