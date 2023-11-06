#include "fonctions_fichiers.h"
#include <stdio.h>

int test_fichier(void)
{
    int nbLigne = 0;
    int nbCol = 0;
    taille_fichier("tabChiffres.txt",&nbLigne,&nbCol);
    printf("taille du fichier : %d x %d \n",nbLigne,nbCol);
    char** tab = lire_fichier("tabChiffres.txt");
    afficher_tab_2D(tab,nbLigne,nbCol);
    printf("\n");
    modifier_caractere(tab,nbLigne,nbCol,'1','a');
    ecrire_fichier("tabChiffres.txt",tab,nbLigne,nbCol);
    char** tab2 = lire_fichier("tabChiffres.txt");
    afficher_tab_2D(tab2,nbLigne,nbCol);
    return 0;
}
