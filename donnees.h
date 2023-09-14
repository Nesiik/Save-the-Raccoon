/**
 * \brief Représentation d'un sprite dans le jeu
*/
struct sprite_s{
    int x; /*!< Abscisse du sprite */
    int y; /*!< Ordonnée du sprite */
    int h; /*!< Hauteur du sprite */
    int w; /*!< Largeur du sprite */
};
/**
 * \brief Type qui correspond aux sprites du jeu
*/
typedef struct sprite_s sprite_t;

/**
 * \brief Représentation du monde du jeu
*/
struct world_s{
    sprite_t vaisseau; /*!< sprite vaisseau */
    sprite_t arrive; /*!< sprite ligne d'arrivée */
    int vy; /*!< vitesse déplacement */
    char gameover; /*!< Champ indiquant si l'on est à la fin du jeu,  1 : mort par mur, 2 : victoire , 3 : sorti jeu */
};

/**
 * \brief Type qui correspond aux données du monde
 */
typedef struct world_s world_t;