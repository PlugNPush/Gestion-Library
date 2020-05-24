//
//  membres.c
//  Gestion-Library
//
//  Created by PlugN on 24/05/2020.
//  Copyright © 2020 PlugN. All rights reserved.
//

#include "membres.h"

void db_saveMembres(Membres membres) {
    //char* location = concat("/Gestion-Library/databases", "/db-membres.data");
    char* location = "db-membres.data";
    printf("Now saving to location: %s", location);
    FILE *membres_db = fopen(location, "w");
    
    if (membres_db == NULL) {
        perror("Critical error, your data was not saved!");
        fclose(membres_db);
        exit(EXIT_FAILURE);
    }
    
    fprintf(membres_db, "DATA_INTEGRITY_CHECK:%s\n", location);
    fprintf(membres_db, "DATA_SIZE:%d\n", membres.taille);
    
    int totalsize = membres.taille;
    int i;
    int r;
    for (i = 0; i < membres.taille; i++) {
        totalsize = totalsize + membres.membres[i].emprunts.taille;
    }
    fprintf(membres_db, "TOTAL_DATA_SIZE:%d\n", totalsize);
    
    Membre membre;
    
    for (i = 0; i < membres.taille; i++) {
        membre = membres.membres[i];
        fprintf(membres_db, "%s:%s:%s:%s:%s:%d/\n", membre.nom, membre.prenom, membre.adresse, membre.mail, membre.metier, membres.membres[i].emprunts.taille);
        for (r = 0; r < membres.membres[i].emprunts.taille; r++) {
            fprintf(membres_db, "%s:%d:%d:%d/\n", membre.emprunts.emprunts[r].code, membre.emprunts.emprunts[r].dateRetour.jour, membre.emprunts.emprunts[r].dateRetour.mois, membre.emprunts.emprunts[r].dateRetour.annee);
        }
    }
    fclose(membres_db);
}



void db_loadMembres(Membres* membres) {
    char* location = "db-membres.data";
    printf("Now reading for location: %s\n", location);
    FILE *membres_db = fopen(location, "r");
    
    membres->taille = 0;
    membres->membres = (Membre*) malloc(sizeof(Membre));
    
    if (membres_db == NULL) {
        perror("Impossible de charger la base de donnees!\n");
        fclose(membres_db);
        return;
    }
    
    char integrityCheck[150];
    fscanf(membres_db, "DATA_INTEGRITY_CHECK:%s", integrityCheck);
    
    printf("Integrity check: %s\n", integrityCheck);
    
    if (strcmp(integrityCheck, location) != 0) {
        printf("Integrity check failed, not loading data!\n");
        fclose(membres_db);
        return;
    }
    
    int i = 0;
    int r = 0;
    int size = 0;
    int totalsize = 0;
    
    char verification[150];
    char verificationtotal[150];
    
    //fscanf(livres_db, "DATA_SIZE: %d", &size);
    fscanf(membres_db, " %19[^:]:%d\n", verification, &size);
    fscanf(membres_db, " %19[^:]:%d\n", verificationtotal, &totalsize);
    
    
    printf("Verification : %s\n", verification);
    printf("Verificationtotal : %s\n", verificationtotal);
    
    printf("READ DATA_SIZE: %d\n", size);
    printf("READ TOTAL_DATA_SIZE: %d\n", totalsize);

    
    FILE *integrity_module = fopen(location, "r");
    
    if (integrity_module == NULL) {
        perror("Impossible de charger la base de donnees!\n");
        fclose(membres_db);
        return;
    }
    
    char c;
    int integrity = 0;
    for (c = getc(integrity_module); c != EOF; c = getc(integrity_module)) {
        if (c == '\n') { 
            integrity = integrity + 1;
        }
    }
    integrity = integrity - 3;
    fclose(integrity_module);
    
    if (integrity != totalsize) {
        printf("\nModule d’integrite calcule %d, mais %d a ete elimine.\n", integrity, totalsize);
        printf("Echec du test d’integrite : le fichier a ete modifie illegalement ! Ne pas charger les donnees.\n\n");
        fclose(membres_db);
        return;
    }
    
    
    membres->taille = size;
    
    membres->membres = (Membre*) malloc(size * sizeof(Membre));
    
    for (i = 0; i < size; i++) {
        fscanf(membres_db, " %100[^:]:%100[^:]:%100[^:]:%100[^:]:%100[^:]:%d/", membres->membres[i].nom, membres->membres[i].prenom, membres->membres[i].adresse, membres->membres[i].mail, membres->membres[i].metier, &membres->membres[i].emprunts.taille);
        membres->membres[i].emprunts.emprunts = (Emprunt*) malloc(membres->membres[i].emprunts.taille * sizeof(Emprunt));
        for (r = 0; r < membres->membres[i].emprunts.taille; r++) {
            fscanf(membres_db, " %100[^:]:%d:%d:%d/", membres->membres[i].emprunts.emprunts[r].code, &membres->membres[i].emprunts.emprunts[r].dateRetour.jour, &membres->membres[i].emprunts.emprunts[r].dateRetour.mois, &membres->membres[i].emprunts.emprunts[r].dateRetour.annee);
        }
    }
    
    fclose(membres_db);
}

void trierMembres(Membres* membres)
{
    int i, j;
    Membre tmp;
    
    for(i=0;i<membres->taille;i++) {
        for(j=i+1;j<membres->taille;j++){
            if(strcmp(membres->membres[i].nom,membres->membres[j].nom)>0){
                tmp = membres->membres[i];
                membres->membres[i] = membres->membres[j];
                membres->membres[j] = tmp;
           }
        }
    }
    
}

void afficherEmprunts(Emprunts emprunts)
{
    int r;
    for (r=0; r < emprunts.taille; r++) {
        printf("Emprunt %d\n", r+1);
        printf("\tCode  : ");
        printf("%s", emprunts.emprunts[r].code);
        printf("\tDate retour  : ");
        printf("%d/%d/%d\n", emprunts.emprunts[r].dateRetour.jour, emprunts.emprunts[r].dateRetour.mois, emprunts.emprunts[r].dateRetour.annee);
    }
}

void afficherMembres(Membres membres)
{
    int r;
    for(r = 0; r < membres.taille; r++) {
        printf("Membre %d\n", r+1);
        printf("\tNom   : ");
        printf("%s", membres.membres[r].nom);
        printf("\tPrenom: ");
        printf("%s", membres.membres[r].prenom);
        printf("\tAdresse: ");
        printf("%s", membres.membres[r].adresse);
        printf("\tMail: ");
        printf("%s", membres.membres[r].mail);
        printf("\tMetier: ");
        printf("%s\n", membres.membres[r].metier);
        afficherEmprunts(membres.membres[r].emprunts);
        printf("\n---------------\n");
    }
}

void ajouterEmprunts(Emprunts* emprunts, Livres* livres)
{
    int nb_emprunts;
    Emprunt* tab_emprunts;
    
    printf("Nombre d'emprunts a renseigner: ");
    scanf("%d", &nb_emprunts);
    fflush(stdin);
    
    int i;
    // allocation mémoire pour tab_livre
    tab_emprunts = (Emprunt*) malloc((emprunts->taille + nb_emprunts) * sizeof(Emprunt));
    tab_emprunts = emprunts->emprunts;
    
    for(i = emprunts->taille; i < (emprunts->taille + nb_emprunts); i++) {
        printf("Emprunt %d\n", i+1);
        char code[8];
        printf("\tCode   ?: ");
        scanf("%8[^\n]", code);
        fflush(stdin);
        
        int index = localiserCodeAutomatique(*livres, code);
        
        if (index >= 0) {
            int transaction = transactionLivre(livres, index, 1);
            if (transaction > 0) {
                strcpy(tab_emprunts[i].code, code);
                fflush(stdin);
                printf("\tDate retour (jj/mm/aaaa)?: ");
                scanf("%d/%d/%d", &tab_emprunts[i].dateRetour.jour, &tab_emprunts[i].dateRetour.mois, &tab_emprunts[i].dateRetour.annee);
                fflush(stdin);
                printf("\n");
            }
            
        }
    }
    
    emprunts->emprunts = tab_emprunts;
    emprunts->taille = emprunts->taille + nb_emprunts;
    
}

void ajouterMembres(Membres* membres, Livres* livres)
{
    int nb_membres;
    Membre* tab_membres;

    printf("Nombre de membres a renseigner: ");
    scanf("%d", &nb_membres);
    fflush(stdin);
    printf("\n");

    int i;
    // allocation mémoire pour tab_livre
    tab_membres = (Membre*) malloc((membres->taille + nb_membres) * sizeof(Membre));
    tab_membres = membres->membres;

    // remplissage tableau
    for(i = membres->taille; i < (membres->taille + nb_membres); i++) {
        printf("Membre %d\n", i+1);
        
        printf("\tNom   ?: ");
        scanf("%100[^\n]", tab_membres[i].nom);
        fflush(stdin);
        printf("\tPrenom?: ");
        scanf("%100[^\n]", tab_membres[i].prenom);
        fflush(stdin);
        printf("\tAdresse?: ");
        scanf("%100[^\n]", tab_membres[i].adresse);
        fflush(stdin);
        printf("\tMail?: ");
        scanf("%100[^\n]", tab_membres[i].mail);
        fflush(stdin);
        printf("\tMetier?: ");
        scanf("%100[^\n]", tab_membres[i].metier);
        fflush(stdin);
        tab_membres[i].emprunts.emprunts = (Emprunt*) malloc(sizeof(Emprunt));
        tab_membres[i].emprunts.taille = 0;
        ajouterEmprunts(&tab_membres[i].emprunts, livres);

        printf("\n---------------\n");
    }


    // affichage tab
    printf("\n");
    
    membres->membres = tab_membres;
    membres->taille = membres->taille + nb_membres;
    
}

int compareDates(Date d1, Date d2){
    if((d1.annee == d2.annee) && (d1.mois == d2.mois) && (d1.jour == d2.jour)){
        return 0;
    }
    else if ((d1.annee > d2.annee) || ((d1.mois  > d2.mois ) && (d1.annee == d2.annee)) || ((d1.jour  > d2.jour ) && (d1.mois  == d2.mois ) && (d1.annee == d2.annee))){
        return 1;
    }
    else{
    return -1;
    }

    return 0;
}

void supprimerMembre(int ligne, Membres* membres)
{
    int i;
    if (ligne >= 0) {
        for (i = ligne; i < membres->taille; i++)
        {
            membres->membres[i] = membres->membres[i+1];
        }
        membres->taille = membres->taille - 1;
    }
    if (ligne == -1) {
        printf("Membre introuvable.\n");
    }
}

int localiserMembre(Membres membres)
{
    Membre m;
    printf("Membre à localiser ?\n");
    printf("Nom :\n> ");
    scanf("%100[^\n]", m.nom);
    fflush(stdin);
    printf("\n");

    printf("Prenom :\n> ");
    scanf("%100[^\n]", m.prenom);
    fflush(stdin);
    printf("\n");


    for (int i = 0; i < membres.taille; i++)
    {
        if (strcmp(m.nom, membres.membres[i].nom) == 0)
        {
            if (strcmp(m.prenom, membres.membres[i].prenom) == 0)
            {
                return i;
            }
        }
    }
    return -1;
}

int localiserCodeAutomatique(Livres livres, char* code)
{
    for (int i = 0; i < livres.taille; i++)
    {
        if (strcmp(code, livres.livres[i].code) == 0)
        {
            return i;
        }
    }
    return -1;
}

void supprimerEmpunt(int ligne, Emprunts* emprunts)
{
    int i;
    if (ligne >= 0) {
        for (i = ligne; i < emprunts->taille; i++)
        {
            emprunts->emprunts[i] = emprunts->emprunts[i+1];
        }
        emprunts->taille = emprunts->taille - 1;
    }
    if (ligne == -1) {
        printf("Membre introuvable.\n");
    }
}

int localiserEmprunt(Emprunts emprunts, char* code)
{
    for (int i = 0; i < emprunts.taille; i++)
    {
        if (strcmp(code, emprunts.emprunts[i].code) == 0)
        {
            return i;
        }
    }
    return -1;
}

void supprimerEmpruntA(Membres* membres, Livres* livres, int index)
{
    time_t my_time;
    struct tm * timeinfo;
    time (&my_time);
    timeinfo = localtime (&my_time);
    Date d2;
    d2.jour = timeinfo->tm_mday;
    d2.mois = timeinfo->tm_mon+1;
    d2.annee = timeinfo->tm_year+1900;
    char code[8];
    printf("Saisir le code:\n> ");
    scanf("%8[^\n]", code);
    fflush(stdin);
    int index2 = localiserEmprunt(membres->membres[index].emprunts, code);
    int index3 = localiserCodeAutomatique(*livres, code);
    if (index2 >= 0 && index3 >= 0) {
        if (compareDates(membres->membres[index].emprunts.emprunts[index2].dateRetour, d2) == -1) {
            printf("Vous avez rendu votre emprunt en retard.");
        }
        supprimerEmpunt(index2, &membres->membres[index].emprunts);
        transactionLivre(livres, index3, -1);
    }
}
