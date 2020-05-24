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
        perror("Unable to load the database!\n");
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
        perror("Unable to load the database!\n");
        fclose(membres_db);
        return;
    }
    
    char c;
    int integrity = 0;
    for (c = getc(integrity_module); c != EOF; c = getc(integrity_module)) {
        if (c == '\n') { // Increment count if this character is newline
            integrity = integrity + 1;
        }
    }
    integrity = integrity - 3;
    fclose(integrity_module);
    
    if (integrity != totalsize) {
        printf("\nIntegrity Module calculated %d, but %d was exptected.\n", integrity, size);
        printf("Integrity test failed: the file was illegally modified! Not loading data.\n\n");
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

Membres getSortedList(Membres membres)
{
    int i, j;
    Membre tmp;
    
    for(i=0;i<=membres.taille;i++) {
        for(j=i+1;j<=membres.taille;j++){
            if(strcmp(membres.membres[i].nom,membres.membres[j].nom)>0){
                tmp = membres.membres[i];
                membres.membres[i] = membres.membres[j];
                membres.membres[j] = tmp;
           }
        }
    }
    
    return membres;
    
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
        printf("\tPrénom: ");
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

void ajouterEmprunts(Emprunts* emprunts)
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
        
        printf("\tCode   ?: ");
        scanf("%8[^\n]", tab_emprunts[i].code);
        fflush(stdin);
        printf("\tDate retour (jj/mm/aaaa)?: ");
        scanf("%d/%d/%d", &tab_emprunts[i].dateRetour.jour, &tab_emprunts[i].dateRetour.mois, &tab_emprunts[i].dateRetour.annee);
        fflush(stdin);
        printf("\n");
    }
    
    emprunts->emprunts = tab_emprunts;
    emprunts->taille = emprunts->taille + nb_emprunts;
    
}

void ajouterMembres(Membres* membres)
{
    int nb_membres;
    Membre* tab_membres;

    printf("Nombre de membres a renseigner: ");
    scanf("%d", &nb_membres);
    fflush(stdin);
    system("cls");

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
        ajouterEmprunts(&tab_membres[i].emprunts);

        printf("\n---------------\n");
    }


    // affichage tab
    system("cls");
    
    membres->membres = tab_membres;
    membres->taille = membres->taille + nb_membres;
    
}
