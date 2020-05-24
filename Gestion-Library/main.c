//
//  main.c
//  Gestion-Library
//
//  Created by PlugN on 05/05/2020.
//  Copyright © 2020 PlugN. All rights reserved.
//

#include "livres.h"
#include "membres.h"

void save(Livres* livres, Membres* membres)
{
    db_saveMembres(*membres);
    db_saveLives(*livres);
}

void start(Livres* livres, Membres* membres)
{
    char choix[50];
    printf("\n\n\nQue souhaitez-vous faire?\nafficher membres - afficher livres - ajouter membres - ajouter livres - rechercher un livre - lister les retards - demander un emprunt - rendre un emprunt - trier membres - trier livres - supprimer membre - supprimer livre - sauvegarder - sortir - sortir sans sauvegarder\n> ");
    scanf("%50[^\n]", choix);
    fflush(stdin);
    printf("Commande: %s\n", choix);
    if (strcmp(choix, "afficher membres") == 0) {
        afficherMembres(*membres);
    } else if (strcmp(choix, "afficher livres") == 0) {
        afficherLivres(*livres);
    } else if (strcmp(choix, "ajouter membres") == 0) {
        ajouterMembres(membres, livres);
    } else if (strcmp(choix, "ajouter livres") == 0) {
        ajouterLivres(livres);
    } else if (strcmp(choix, "rechercher un livre") == 0) {
        rechercherLivre(*livres);
    } else if (strcmp(choix, "lister les retards") == 0){
        listerRetards(membres, livres);
    } else if (strcmp(choix, "demander un emprunt") == 0){
        int index = localiserMembre(*membres);
        if (index >= 0) {
            if (membres->membres[index].emprunts.taille >= 3) {
                printf("Vous avez trop d'emprunts !");
            } else {
                ajouterEmprunts(&membres->membres[index].emprunts, livres);
            }
        }
    } else if (strcmp(choix, "rendre un emprunt") == 0) {
        int index = localiserMembre(*membres);
        if (index >= 0) {
            supprimerEmpruntA(membres, livres, index);
        }
    } else if (strcmp(choix, "trier membres") == 0) {
        trierMembres(membres);
    } else if (strcmp(choix, "trier livres") == 0) {
        char choixtri[50];
        printf("Comment voulez-vous les trier?\ntitre - auteur - code\n> ");
        scanf("%50[^\n]", choixtri);
        fflush(stdin);
        if (strcmp(choixtri, "titre") == 0) {
            trierLivres(livres);
        } else if (strcmp(choixtri, "auteur") == 0) {
            trierLivresAuteur(livres);
        } else if (strcmp(choixtri, "code") == 0) {
            trierLivresCode(livres);
        } else {
            printf("Argument non reconnu.\n");
        }
        
    } else if (strcmp(choix, "supprimer membre") == 0) {
        int index = localiserMembre(*membres);
        supprimerMembre(index, membres);
    } else if (strcmp(choix, "supprimer livre") == 0) {
        int index = localiserLivre(*livres);
        supprimerLivre(index, livres);
    } else if (strcmp(choix, "sauvegarder") == 0) {
        save(livres, membres);
    } else if (strcmp(choix, "sortir") == 0) {
        return;
    } else if (strcmp(choix, "sortir sans sauvegarder") == 0) {
        exit(EXIT_SUCCESS);
    }
    else {
        printf("Commande non reconnue.\n");
    }
    start(livres, membres);
}

void init(Livres* livres, Membres* membres)
{
    
    db_loadMembres(membres);
    db_loadLivres(livres);
    
    start(livres, membres);
}

int main(int argc, const char * argv[])
{
    
    Livres livres;
    Membres membres;
    
    init(&livres, &membres);
    
    save(&livres, &membres);

    return 0;
}
