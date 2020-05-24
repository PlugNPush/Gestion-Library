//
//  livres.h
//  Gestion-Library
//
//  Created by PlugN on 24/05/2020.
//  Copyright © 2020 PlugN. All rights reserved.
//

#ifndef livres_h
#define livres_h

#include "global.h"

typedef struct Livre {
    char nom[50];
    char auteur[50];
    char code[8];
    int exemplaires;
    int disponibles;
} Livre;

typedef struct Livres {
    Livre* livres;
    int taille;
} Livres;

void db_saveLives(Livres livres);
void db_loadLivres(Livres* livres);
void afficherLivres(Livres livres);
void ajouterLivres(Livres* livres);

#endif /* livres_h */
