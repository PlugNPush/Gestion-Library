//
//  membres.h
//  Gestion-Library
//
//  Created by PlugN on 24/05/2020.
//  Copyright © 2020 PlugN. All rights reserved.
//

#ifndef membres_h
#define membres_h

#include "global.h"

typedef struct Date {
    int jour;
    int mois;
    int annee;
} Date;

typedef struct Emprunt {
    char code[8];
    Date dateRetour;
} Emprunt;

typedef struct Emprunts {
    Emprunt* emprunts;
    int taille;
} Emprunts;

typedef struct Membre {
    char nom[50];
    char prenom[50];
    char mail[50];
    char adresse[50];
    char metier[50];
    Emprunts emprunts;
} Membre;

typedef struct Membres {
    Membre* membres;
    int taille;
} Membres;

void db_saveMembres(Membres membres);
void db_loadMembres(Membres* membres);
void trierMembres(Membres* membres);
void afficherEmprunts(Emprunts emprunts);
void afficherMembres(Membres membres);
void ajouterEmprunts(Emprunts* emprunts);
void ajouterMembres(Membres* membres);

#endif /* membres_h */
