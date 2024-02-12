#include "fonctions_fichiers.h"
#include <stdlib.h>
#include <stdio.h>

char** allouer_tab_2D(int n,int m){
    //https://www.geeksforgeeks.org/dynamically-allocate-2d-array-c/
    int len = sizeof(char *) * n + sizeof(char) * m * n;
    char** tab = (char **)malloc(len);
 
    // ptr is now pointing to the first element in of 2D array
    char* ptr = (char *)(tab + n);
    // for loop to point rows pointer to appropriate location in 2D array
    for(int i = 0; i < n; i++){
        tab[i] = (ptr + m * i);
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            tab[i][j] = ' ';
        }   
    }
    return tab;
}

void desallouer_tab_2D(char** tab /*, int n*/ ){
    free(tab);
}

void afficher_tab_2D(char** tab, int n , int m){
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++)
        {
            printf("%c ",tab[i][j]);  
        }
        printf("\n");
    }
}

void taille_fichier(const char* nomFichier,int* nbLig,int* nbCol){
    FILE* ptrFichier = fopen(nomFichier,"r");
    if(ptrFichier == NULL){
        printf("Erreur ouverture fichier");
        return;
    }
    int col = 0;
    *nbCol = 0;
    *nbLig = 0;
    char strFichier = fgetc(ptrFichier);
    while (strFichier != EOF){
        if(strFichier == '\n' || strFichier == '\r'){
            if (col>0){
                *nbLig += 1;
            }
            if(col > *nbCol){
                *nbCol = col;
            }
            col = 0;
        }
        else{
            col++;
        }
        strFichier = fgetc(ptrFichier);
    }
    fclose(ptrFichier);
}

char** lire_fichier(const char* nomFichier, int* nbLig, int* nbCol){
    FILE* ptrFichier = fopen(nomFichier,"r");
    if(ptrFichier == NULL){
        printf("Erreur ouverture fichier");
        return NULL;
    }
    char** tab;
    if (nbLig == NULL || nbCol == NULL) {
        int nbLigne,nbCcol;
        taille_fichier(nomFichier,&nbLigne,&nbCcol);
        tab = allouer_tab_2D(nbLigne,nbCcol);
    }
    else{
        taille_fichier(nomFichier,nbLig,nbCol);
        tab = allouer_tab_2D(*nbLig,*nbCol);
    }
    int cLigne = 0;
    int cCol = 0;
    char strFichier = fgetc(ptrFichier);
    while (strFichier != EOF) {
        if(strFichier == '\n' || strFichier == '\r'){
            if (cCol>0){
                cLigne +=1;
            }
            cCol = 0;
        }
        else{
            tab[cLigne][cCol] = strFichier;
            cCol++;
        }
        strFichier = fgetc(ptrFichier);
    } 
    fclose(ptrFichier);
    return tab;

}

char** modifier_caractere(char** tab, int n, int m, char ancien, char nouveau){
    for (int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            if (tab[i][j] == ancien) {
                tab[i][j] = nouveau;
            } 
        }
    }
    return tab;
}

void ecrire_fichier(const char* nomFichier, char** tab, int n, int m){
    FILE* fichier = fopen(nomFichier,"w");
    if(fichier != NULL){
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                fputc(tab[i][j],fichier);
            }
            fputc('\n',fichier);
        }
        fclose(fichier);
    }
}