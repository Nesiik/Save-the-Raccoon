#include "fonctions_fichiers.h"
#include <stdio.h>

int test_modif_fichiers(void) {
    int nbLigne = 0;
    int nbCol = 0;
    taille_fichier("tabChiffres.txt",&nbLigne,&nbCol);
    printf("taille du fichier : %d x %d \n",nbLigne,nbCol);
    char** tab = lire_fichier("tabChiffres.txt",NULL,NULL);
    afficher_tab_2D(tab,nbLigne,nbCol);
    printf("\n");
    modifier_caractere(tab,nbLigne,nbCol,'1','a');
    ecrire_fichier("tabChiffres.txt",tab,nbLigne,nbCol);

    taille_fichier("tabChiffres.txt",&nbLigne,&nbCol);
    printf("taille du fichier : %d x %d \n",nbLigne,nbCol);
    tab = lire_fichier("tabChiffres.txt",NULL,NULL);
    afficher_tab_2D(tab,nbLigne,nbCol);
    modifier_caractere(tab,nbLigne,nbCol,'a','1');
    ecrire_fichier("tabChiffres.txt",tab,nbLigne,nbCol);
    
    printf("\n");
    taille_fichier("tabChiffres.txt",&nbLigne,&nbCol);
    printf("taille du fichier : %d x %d \n",nbLigne,nbCol);
    tab = lire_fichier("tabChiffres.txt",NULL,NULL);
    afficher_tab_2D(tab,nbLigne,nbCol);
    desallouer_tab_2D(tab);
    return 0;
}
