#include "sdl.h"
#include "sprite.h"
#include "fonctions_fichiers.h"


void mouse_menu_events(SDL_MouseButtonEvent button,world_t* world, ressources_t* ressources){
    int mouseX = button.x;
    int mouseY = button.y;

    for (unsigned char i = 0; i < MAIN_MENU_ITEM_COUNT; i++) {
        Item menuItem = ressources->MenuItems->ItemList[i];

        if (mouseX >= menuItem.rect.x && mouseX <= menuItem.rect.x + menuItem.rect.w &&
            mouseY >= menuItem.rect.y && mouseY <= menuItem.rect.y + menuItem.rect.h)
        {
            ressources->MenuItems->lastselectedItem = ressources->MenuItems->curselectedItem;
            ressources->MenuItems->curselectedItem = i;
            switch (i) {
                case 0: // Jouer
                    //init_data(world);
                    world->game_state = Alive;
                    if(world->cur_level == 0)
                        world->cur_level = 1;
                    //world->depart = SDL_GetTicks();
                    break;
                case 1: // Option
                    break;
                case 2: // Quitter
                    world->game_state = Quit;
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
            world->game_state = Quit;
        break;
        case SDL_KEYDOWN:
            if(world->game_state == Menu){
                if(event->key.keysym.sym == SDLK_ESCAPE){
                    world->game_state = Quit;
                }
            }
            if(world->game_state == Alive){
                if(event->key.keysym.sym == SDLK_ESCAPE){
                    world->game_state = Menu;
                    world->cur_level = 0;
                }else{
                    if(event->key.keysym.sym == SDLK_q || event->key.keysym.sym == SDLK_d || event->key.keysym.sym == SDLK_s || event->key.keysym.sym == SDLK_z )
                        deplacement(player, world,event->key.keysym.sym);
                }
            }
        break;
        case SDL_MOUSEBUTTONDOWN:
            switch (world->game_state)
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
        if(i == ressources->MenuItems->curselectedItem){
            if (ressources->MenuItems->curselectedItem != ressources->MenuItems->lastselectedItem){
                SDL_Color color = (SDL_Color){ 180, 180, 180 };
                //Bouton sélectionné
                SDL_DestroyTexture(ressources->MenuItems->ItemList[i].texture);
                ressources->MenuItems->ItemList[i].texture = creer_texte_texture(renderer,ressources->font,2,&color,ressources->MenuItems->ItemList[i].text,ressources->MenuItems->ItemList[i].rect.x,ressources->MenuItems->ItemList[i].rect.y,&ressources->MenuItems->ItemList[i].rect);
                if(ressources->MenuItems->lastselectedItem != -1){ // si il y a eu une sélection avant cette sélection
                    color = (SDL_Color){ 100, 100, 100 };
                    char lastItem = ressources->MenuItems->lastselectedItem;
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

void render_sky(SDL_Renderer* renderer, ressources_t* ressources){
    SDL_Rect rect = {0,0,WINDOW_WIDTH,WINDOW_HEIGHT};
    if(SDL_RenderCopy(renderer, ressources->sky->text, NULL, &rect) < 0){
        printf("%s",SDL_GetError());
    }
}

void render_worlds(SDL_Renderer* renderer,ressources_t* ressources,world_t* world){
    int level = world->cur_level;
    char cur_char;
    int tabij,dirt_y,dirt_x,one_sprite_w,one_sprite_h;
    for (unsigned int i = 0; i < world->levels[level]->nb_ligne_level_tab; i++)
    {
        for (unsigned int j = 0; j < world->levels[level]->nb_col_level_tab; j++)
        {
            cur_char = world->levels[level]->level_tab[i][j]; // ' / 39 = trou
            if(cur_char > 64 && cur_char < 91){ //dirt
                tabij = cur_char - 'A'; // conversion ascii -> int
                dirt_y = tabij/6;
                dirt_x = tabij - dirt_y*6;
                one_sprite_w = (ressources->dirt->sprite_w/N_WIDTH_DIRT_SPRITE)-1;
                one_sprite_h = (ressources->dirt->sprite_h/N_HEIGHT_DIRT_SPRITE)-1;
                SDL_Rect src = {dirt_x*one_sprite_w + (dirt_x+1),dirt_y*one_sprite_w + (dirt_y+1),one_sprite_w,one_sprite_h};
                SDL_Rect dest = {j*one_sprite_w,i*one_sprite_h , one_sprite_w , one_sprite_h};
                if(SDL_RenderCopy(renderer,ressources->dirt->text,&src,&dest)<0){
                    SDL_Log("Erreur : %s",SDL_GetError());
                }
            }
            else if(cur_char > 59 && cur_char < 64){
                tabij = cur_char - '<'; // conversion ascii -> int
                SDL_Rect dest = {j*ressources->spike->sprite_w,i*ressources->spike->sprite_h , ressources->spike->sprite_w , ressources->spike->sprite_h};
                if(SDL_RenderCopyEx(renderer, ressources->spike->text, NULL, &dest, tabij*90, NULL, SDL_FLIP_NONE )<0){
                    SDL_Log("Erreur : %s",SDL_GetError());
                }
            }          
        }
    }
}

//Fait pour afficher du texte qui change beaucoup
void afficher_texte(SDL_Renderer* renderer, TTF_Font* police, const char text[], int x, int y ) {
    SDL_Rect dest;
    SDL_Texture* tex = creer_texte_texture(renderer,police,0,NULL,text,x,y,&dest);
   	SDL_RenderCopy(renderer, tex, NULL, &dest);
	SDL_DestroyTexture(tex);
}

//renvoie la surface pour du texte
SDL_Surface* texte_surface(SDL_Renderer* renderer, TTF_Font* police,const char text[],char type,SDL_Color* color){
    SDL_Surface* surf;
    if(color == NULL){
        color = &(SDL_Color){ 100, 100, 100 };
    }
    switch (type) //https://www.libsdl.org/projects/old/SDL_ttf/docs/SDL_ttf.html#SEC42
    {
    case 0: //rapide
        surf = TTF_RenderText_Solid(police,text,*color);
        break;
    case 2: //lent +beau 
        surf = TTF_RenderText_Blended(police,text,*color);
        break;
    default:
        break;
    }
    if (!surf)
    {
        printf("%s \n",TTF_GetError());
    }
    return surf;
}

//creer texture texte et rempli un SDL_rect (si non null)
SDL_Texture* creer_texte_texture(SDL_Renderer* renderer, TTF_Font* police,char type,SDL_Color* color, const char text[],int x, int y, SDL_Rect* info){
    SDL_Surface* surf = texte_surface(renderer,police,text,type,color);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
    if(text == NULL){
        printf("%s \n",SDL_GetError());
        return NULL;
    }
    if(info != NULL){
        info->x = x;
        info->y = y;
        info->w = surf->w/2;
        info->h = surf->h/2;
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
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    if(renderer == NULL)
    {
        SDL_Log("Erreur lors de la creation du renderer : %s", SDL_GetError());
    }
    if(SDL_RenderSetLogicalSize(renderer,WINDOW_WIDTH,WINDOW_HEIGHT) < 0){
        SDL_Log("%s",SDL_GetError());
    }
    return renderer;
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
        ressources->MenuItems->ItemList[i].rect = (SDL_Rect){x,y,0,0};
        ressources->MenuItems->ItemList[i].text = SDL_strdup(str);
        ressources->MenuItems->ItemList[i].texture = creer_texte_texture(renderer,ressources->font,2,NULL,str, ressources->MenuItems->ItemList[i].rect.x, ressources->MenuItems->ItemList[i].rect.y,&ressources->MenuItems->ItemList[i].rect);
        y+=50;
    }
    

    ressources->MenuItems->curselectedItem = -1;
    ressources->MenuItems->lastselectedItem = -1;

    ressources->dirt = charger_image_png("../assets/dirt_sprite.png",renderer);

    ressources->spike = charger_image_png("../assets/spikeV4.png",renderer);
    ressources->sky = charger_image_png("../assets/clouds2.1Large(1).png",renderer);

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

    TTF_CloseFont(ressources->font);
}