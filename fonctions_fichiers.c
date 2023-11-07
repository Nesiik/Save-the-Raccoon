#include "fonctions_fichiers.h"
#include "stdlib.h"
#include "stdio.h"

char** allouer_tab_2D(int n,int m){
    
    char** tab = malloc(sizeof(char*)*n); // lignes
    for (int i = 0; i < n; i++)
    {
        tab[i] = malloc(sizeof(char)*m); // colonnes
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            tab[i][j] = ' ';
        }   
    }
    return tab;
}

void desallouer_tab_2D(char** tab, int n){
    for (int i = 0; i < n; i++)
    {
        free(tab[n]);
    }
    free(tab);
}

void afficher_tab_2D(char** tab, int n , int m){
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            printf("%c ",tab[i][j]);  
        }
        printf("\n");
    }
}

void taille_fichier(const char* nomFichier,int* nbLig,int* nbCol){
    FILE* ptrFichier = fopen(nomFichier,"r");
    char strFichier;
    int cCol = 0;
    int lig = 0;
    int col = 0;
    do
    {
        strFichier = fgetc(ptrFichier);
        if (strFichier == EOF)
        {
            break;
        }
        else {
            if(strFichier == '\n' || strFichier == '\r'){
                if (cCol>0){
                    lig +=1;
                }
                if(cCol > col){
                    col = cCol;
                }
                cCol = 0;
            }
            else{
                cCol++;
            }
        }
    } while (strFichier != EOF);
    *nbCol = col;
    *nbLig = lig;
    fclose(ptrFichier);
}

char** lire_fichier(const char* nomFichier){
    int nbLigne,nbCol;
    taille_fichier(nomFichier,&nbLigne,&nbCol);
    char** tab = allouer_tab_2D(nbLigne,nbCol);
    FILE* ptrFichier = fopen(nomFichier,"r");
    char c;
    int cLigne = 0;
    int cCol = 0;
    do
    {
        c = fgetc(ptrFichier);
        if (c == EOF)
        {
            break;
        }
        else {
            if(c == '\n' || c == '\r'){
                if (cCol>0){
                    cLigne +=1;
                }
                cCol = 0;
            }
            else{
                tab[cLigne][cCol] = c;
                cCol++;
            }
        }

    } while (c != EOF);
    fclose(ptrFichier);
    return tab;

}

char** modifier_caractere(char** tab, int n, int m, char ancien, char nouveau){
    for (int i = 0; i < n; i++)
    {
        for(int j = 0; j < m; j++){
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
    fichier = fopen(nomFichier,"w");
    if(fichier != NULL){
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                fputc(tab[i][j],fichier);
            }
            
            fputc('\r',fichier);
        }
        fclose(fichier);
    }
}