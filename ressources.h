/**
 * \brief Représentation pour stocker les ressources nécessaires à l'affichage graphique
*/
struct ressources_s{
    SDL_Texture* background; /*!< Texture liée à l'image du fond de l'écran. */
    SDL_Texture* vaisseau; /*!< Texture liée à l'image du vaiseau. */
    SDL_Texture* arrive; /*!< Texture liée à l'image de la ligne d'arrivée */
    SDL_Texture* meteorite; /*!< Texture liée à l'image d'une meteorite. */
    TTF_Font* font; /*!< La police d'écriture pour le jeu. */
    MenuItem MenuItems;
};
/**
 * \brief Type qui correspond aux textures du jeu
*/
typedef struct ressources_s ressources_t;