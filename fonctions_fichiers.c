#include "fonctions_fichiers.h"
#include "stdlib.h"
#include "stdio.h"

char** allouer_tab_2D(int n,int m){
    char** tab = malloc(sizeof(char*)*n); // lignes
    for (int i = 0; i <= n; i++)
    {
        tab[i] = malloc(sizeof(char)*m); // colonnes
    }
    for (int i = 0; i <= n; i++)
    {
        for (int j = 0; j <= m; j++)
        {
            tab[i][j] = ' ';
        }   
    }
    return tab;
}

void desallouer_tab_2D(char** tab, int n){
    for (int i = 0; i <= n; i++)
    {
        free(tab[n]);
    }
    free(tab);
}

void afficher_tab_2D(char** tab, int n , int m){
    for (int i = 0; i <= n; i++)
    {
        for (int j = 0; j <= m; j++)
        {
            printf("%c ",tab[i][j]);  
        }
        printf("\n");
    }
}

void taille_fichier(const char* nomFichier,int* nbLig,int* nbCol){
    FILE* ptrFichier = fopen(nomFichier,"r");
    int strFichier;
    int cCol = -1;
    *nbLig = 0;
    *nbCol = 0;
    do
    {
        strFichier = fgetc(ptrFichier);
        if (strFichier == -1)
        {
            continue;
        }
        cCol++;
        if(strFichier == '\n' || strFichier == '\r'){
            if (fgetc(ptrFichier) == '\r')
            {
                continue;
            }
            *nbLig = *nbLig + 1;
            if(cCol > *nbCol){
                *nbCol = cCol;
            }
            cCol = -1;
        }
    } while (strFichier != EOF);

    fclose(ptrFichier);
}

char** lire_fichier(const char* nomFichier){
    int nbLigne,nbCol;
    taille_fichier(nomFichier,&nbLigne,&nbCol);
    char** tab = allouer_tab_2D(nbLigne,nbCol);
    FILE* ptrFichier = fopen(nomFichier,"r");
    int c;
    int cLigne = 0;
    int cCol = 0;
    do
    {
        c = fgetc(ptrFichier);
        if (c == -1)
        {
            continue;
        }
        //printf("c:%d , cligne: %d , cCol: %d \n",c,cLigne,cCol);
        if(c == '\n' || c == '\r'){
            if (c == '\r')
            {
                fgetc(ptrFichier);
            }
            cLigne++;
            cCol = 0;
            continue;
        }
        tab[cLigne][cCol] = c;
        cCol++;
    } while (c != EOF);

    return tab;

    fclose(ptrFichier);
}

char** modifier_caractere(char** tab, int n, int m, char ancien, char nouveau){
    for (int i = 0; i <= n; i++)
    {
        for(int j = 0; j <= m; j++){
            if (tab[i][j] == ancien)
            {
                tab[i][j] = nouveau;
            }
            
        }
    }
    return tab;
}

void ecrire_fichier(const char* nomFichier, char** tab, int n, int m){
    FILE* fichier = NULL;
    fichier = fopen(nomFichier,"w+");

    if(fichier != NULL){
        for (int i = 0; i <= n; i++)
        {
            for (int j = 0; j <= m; j++)
            {
                if(j != m){
                    fputc(tab[i][j],fichier);
                }
            }
            if(i != n){
                fputc('\n',fichier);
            }
        }
        fclose(fichier);
    }
}