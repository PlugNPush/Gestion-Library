//
//  livres.c
//  Gestion-Library
//
//  Created by PlugN on 24/05/2020.
//  Copyright © 2020 PlugN. All rights reserved.
//

#include "livres.h"

void db_saveLives(Livres livres) {
    
    char* location = "db-livres.data";
    printf("Enregistrement a l'emplacement: %s\n", location);
    FILE *livres_db = fopen(location, "w");
    
    if (livres_db == NULL) {
        perror("Erreur critique, vos donnees n’ont pas ete enregistrees");
        fclose(livres_db);
        exit(EXIT_FAILURE);
    }
    
    fprintf(livres_db, "DATA_INTEGRITY_CHECK:%s\n", location);
    fprintf(livres_db, "DATA_SIZE:%d\n", livres.taille);
    Livre livre;
    int i;
    for (i = 0; i < livres.taille; i++) {
        livre = livres.livres[i];
        fprintf(livres_db, "%s:%s:%d:%d:%s;\n", livre.auteur, livre.code, livre.disponibles, livre.exemplaires, livre.nom);
    }
    fclose(livres_db);
}

void db_loadLivres(Livres* livres) {
    char* location = "db-livres.data";
    printf("Chargement depuis l'emplacement: %s\n", location);
    FILE *livres_db = fopen(location, "r");
    
    livres->taille = 0;
    livres->livres = (Livre*) malloc(sizeof(Livre));
    
    if (livres_db == NULL) {
        perror("Impossible de charger la base de donnees!\n");
        fclose(livres_db);
        return;
    }
    
    char integrityCheck[150];
    fscanf(livres_db, "DATA_INTEGRITY_CHECK:%s", integrityCheck);
    
    printf("Controle d'integrite: %s\n", integrityCheck);
    
    if (strcmp(integrityCheck, location) != 0) {
        printf("Echec de la verification de l’integrite, pas de chargement des donnees!\n");
        fclose(livres_db);
        return;
    }
    
    int i = 0;
    int size = 0;
    
    char verification[150];
    
    
    fscanf(livres_db, " %19[^:]:%d\n", verification, &size);
    
    
    printf("Verification : %s\n", verification);
    
    printf("READ DATA_SIZE: %d\n", size);
    
    FILE *integrity_module = fopen(location, "r");
    
    if (integrity_module == NULL) {
        perror("Impossible de charger la base de donnees!\n");
        fclose(livres_db);
        return;
    }
    
    char c;
    int integrity = 0;
    for (c = getc(integrity_module); c != EOF; c = getc(integrity_module)) {
        if (c == '\n') { 
            integrity = integrity + 1;
        }
    }
    integrity = integrity - 2;
    fclose(integrity_module);
    
    if (integrity != size) {
        printf("\nLe module d’integrite a calcule %d, mais %d etait attendu.\n", integrity, size);
        printf("Echec du test d’integrite : le fichier a ete modifie illegalement ! Les donnees ne seront pas chargees.\n\n");
        fclose(livres_db);
        return;
    }
    
    
    livres->taille = size;
    
    livres->livres = (Livre*) malloc(size * sizeof(Livre));
    
    for (i = 0; i < size; i++) {
        fscanf(livres_db, " %100[^:]:%100[^:]:%d:%d:%100[^;];", livres->livres[i].auteur, livres->livres[i].code, &livres->livres[i].disponibles, &livres->livres[i].exemplaires, livres->livres[i].nom);
    }
    
    fclose(livres_db);
}

void afficherLivres(Livres livres)
{
    int r;
    for(r = 0; r < livres.taille; r++) {
        printf("Livre %d\n", r+1);
        printf("\tTitre   : ");
        printf("%s", livres.livres[r].nom);
        printf("\tAuteur: ");
        printf("%s", livres.livres[r].auteur);
        printf("\tCode: ");
        printf("%s", livres.livres[r].code);
        printf("\tExemplaires: ");
        printf("%d", livres.livres[r].exemplaires);
        printf("\tDispobibles: ");
        printf("%d", livres.livres[r].disponibles);

        printf("\n---------------\n");
    }
}

void ajouterLivres(Livres* livres)
{
    int nb_livre;
    Livre* tab_livre;

    printf("Nombre de livres a renseigner: ");
    scanf("%d", &nb_livre);
    fflush(stdin);
    printf("\n");

    int i;
    // allocation mémoire pour tab_livre
    tab_livre = (Livre*) malloc((livres->taille + nb_livre) * sizeof(Livre));
    tab_livre = livres->livres;

    // remplissage tableau
    for(i = livres->taille; i < (livres->taille + nb_livre); i++) {
        printf("Livre %d\n", i+1);
        
        printf("\tTitre   ?: ");
        scanf("%100[^\n]", tab_livre[i].nom);
        fflush(stdin);
        printf("\tAuteur?: ");
        scanf("%100[^\n]", tab_livre[i].auteur);
        fflush(stdin);
        printf("\tExemplaires?: ");
        scanf("%d", &tab_livre[i].exemplaires);
        fflush(stdin);
        printf("\tDisponibles?: ");
        scanf("%d", &tab_livre[i].disponibles);
        fflush(stdin);
        
        printf("\tCode?: ");
        scanf("%8[^\n]", tab_livre[i].code);
        fflush(stdin);
        
        

        printf("\n---------------\n");
    }


    // affichage tab
    printf("\n");
    
    livres->livres = tab_livre;
    livres->taille = livres->taille + nb_livre;
    
}

void trierLivresCode(Livres* livres)
{
    int i, j;
    Livre tmp;
    
    for(i=0;i<livres->taille;i++) {
        for(j=i+1;j<livres->taille;j++){
            if(strcmp(livres->livres[i].code,livres->livres[j].code)>0){
                tmp = livres->livres[i];
                livres->livres[i] = livres->livres[j];
                livres->livres[j] = tmp;
           }
        }
    }
    
}

void trierLivresAuteur(Livres* livres)
{
    int i, j;
    Livre tmp;
    
    for(i=0;i<livres->taille;i++) {
        for(j=i+1;j<livres->taille;j++){
            if(strcmp(livres->livres[i].auteur,livres->livres[j].auteur)>0){
                tmp = livres->livres[i];
                livres->livres[i] = livres->livres[j];
                livres->livres[j] = tmp;
           }
        }
    }
    
}

void trierLivres(Livres* livres)
{
    int i, j;
    Livre tmp;
    
    for(i=0;i<livres->taille;i++) {
        for(j=i+1;j<livres->taille;j++){
            if(strcmp(livres->livres[i].nom,livres->livres[j].nom)>0){
                tmp = livres->livres[i];
                livres->livres[i] = livres->livres[j];
                livres->livres[j] = tmp;
           }
        }
    }
    
}

void supprimerLivre(int ligne, Livres* livres)
{
    int i;
    if (ligne >= 0) {
        for (i = ligne; i < livres->taille; i++)
        {
            livres->livres[i] = livres->livres[i+1];
        }
        livres->taille = livres->taille - 1;
    }
    if (ligne == -1) {
        printf("Livre introuvable.\n");
    }
}

int localiserLivre(Livres livres)
{
    Livre l;
    printf("Livre a localiser ?\n");
    printf("Titre :\n> ");
    scanf("%100[^\n]", l.nom);
    fflush(stdin);
    printf("\n");

    printf("Auteur :\n> ");
    scanf("%100[^\n]", l.auteur);
    fflush(stdin);
    printf("\n");


    for (int i = 0; i < livres.taille; i++)
    {
        if (strcmp(l.nom, livres.livres[i].nom) == 0)
        {
            if (strcmp(l.auteur, livres.livres[i].auteur) == 0)
            {
                return i;
            }
        }
    }
    return -1;
}

int localiserCode(Livres livres)
{
    Livre l;
    printf("Livre a localiser ?\n");
    printf("Code:\n> ");
    scanf("%8[^\n]", l.code);
    fflush(stdin);
    printf("\n");


    for (int i = 0; i < livres.taille; i++)
    {
        if (strcmp(l.code, livres.livres[i].code) == 0)
        {
            return i;
        }
    }
    return -1;
}

int transactionLivre(Livres* livres, int index, int sens)
{
    // Sens positif = emprunter un livre.
    // Sens négatif = rendre un livre.
    if (sens > 0) {
        if (livres->livres[index].disponibles > 0) {
            livres->livres[index].disponibles = livres->livres[index].disponibles - 1;
            return 1;
        }
    } else {
        livres->livres[index].disponibles = livres->livres[index].disponibles + 1;
        return 1;
    }
    return 0;
}

void afficherLivre(Livre livre)
{
    printf("\tTitre   : ");
    printf("%s", livre.nom);
    printf("\tAuteur: ");
    printf("%s", livre.auteur);
    printf("\tCode: ");
    printf("%s", livre.code);
    printf("\tExemplaires: ");
    printf("%d", livre.exemplaires);
    printf("\tDispobibles: ");
    printf("%d", livre.disponibles);

    printf("\n---------------\n");
}

void rechercherLivre(Livres livres)
{
    char recherche[100];
    printf("Effectuez votre recherche.\n");
    printf("Recherche groupee (code, titre, auteur...):\n> ");
    scanf("%100[^\n]", recherche);
    fflush(stdin);
    
    for (int i = 0; i < livres.taille; i++)
    {
        if ((strcmp(recherche, livres.livres[i].code) == 0) || (strcmp(recherche, livres.livres[i].nom) == 0) || (strcmp(recherche, livres.livres[i].auteur) == 0) || (strncmp(recherche, livres.livres[i].code, 3) == 0 && strlen(recherche) == 3))
        {
            afficherLivre(livres.livres[i]);
        }
    }
    
}
