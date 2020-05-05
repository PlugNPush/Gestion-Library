//
//  main.c
//  Gestion-Library
//
//  Created by PlugN on 05/05/2020.
//  Copyright © 2020 PlugN. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>


typedef struct Livre {
    char* nom;
    char* auteur;
} Livre;

int main()
{
    int nb_livre;
    Livre* tab_livre;

    printf("Nombre de livres a renseigne: ");
    scanf("%d", &nb_livre);

    system("cls");

    int i;
    // allocation mémoire pour tab_livre
    tab_livre = malloc(nb_livre * sizeof(Livre));

    // remplissage tableau
    for(i = 0; i < nb_livre; i++) {
        printf("Livre %d\n", i+1);
        printf("\tNom   ?: ");

        scanf("%49s", tab_livre[i].nom);
        printf("\tAuteur?: ");
        scanf("%49s", tab_livre[i].auteur);

        printf("\n---------------\n");
    }


    // affichage tab
    system("cls");
        for(i = 0; i < nb_livre; i++) {
        printf("Livre %d\n", i+1);
        printf("\tNom   : ");
        printf("%s", tab_livre[i].nom);
        printf("\tAuteur: ");
        printf("%s", tab_livre[i].auteur);

        printf("\n---------------\n");
    }

    // liberation tab
    free(tab_livre);

    return 0;
}