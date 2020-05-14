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
    Emprunt* emprunt;
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

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

void db_saveLives(Livres livres) {
    //char* location = concat("/Gestion-Library/databases", "/db-livres.data");
    char* location = "db-livres.data";
    printf("Now saving to location: %s\n", location);
    FILE *livres_db = fopen(location, "w");
    
    if (livres_db == NULL) {
        perror("Critical error, your data was not saved");
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
            fprintf(membres_db, "%s:%d:%d:%d/\n", membre.emprunts.emprunt[r].code, membre.emprunts.emprunt[r].dateRetour.jour, membre.emprunts.emprunt[r].dateRetour.mois, membre.emprunts.emprunt[r].dateRetour.annee);
        }
    }
    fclose(membres_db);
}

void db_loadLivres(Livres* livres) {
    char* location = "db-livres.data";
    printf("Now reading for location: %s\n", location);
    FILE *livres_db = fopen(location, "r");
    
    if (livres_db == NULL) {
        perror("Unable to load the database!\n");
        fclose(livres_db);
        return;
    }
    
    char integrityCheck[150];
    fscanf(livres_db, "DATA_INTEGRITY_CHECK:%s", integrityCheck);
    
    printf("Integrity check: %s\n", integrityCheck);
    
    if (strcmp(integrityCheck, location) != 0) {
        printf("Integrity check failed, not loading data!\n");
        fclose(livres_db);
        return;
    }
    
    int i = 0;
    int size = 0;
    
    char verification[150];
    
    //fscanf(livres_db, "DATA_SIZE: %d", &size);
    fscanf(livres_db, " %19[^:]:%d\n", verification, &size);
    
    
    printf("Verification : %s\n", verification);
    
    printf("READ DATA_SIZE: %d\n", size);
    
    FILE *integrity_module = fopen(location, "r");
    
    if (integrity_module == NULL) {
        perror("Unable to load the database!\n");
        fclose(livres_db);
        return;
    }
    
    char c;
    int integrity = 0;
    for (c = getc(integrity_module); c != EOF; c = getc(integrity_module)) {
        if (c == '\n') { // Increment count if this character is newline
            integrity = integrity + 1;
        }
    }
    integrity = integrity - 2;
    fclose(integrity_module);
    
    if (integrity != size) {
        printf("\nIntegrity Module calculated %d, but %d was exptected.\n", integrity, size);
        printf("Integrity test failed: the file was illegally modified! Not loading data.\n\n");
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

void db_loadMembres(Membres* membres) {
    char* location = "db-membres.data";
    printf("Now reading for location: %s\n", location);
    FILE *membres_db = fopen(location, "r");
    
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
        
        for (r = 0; r < membres->membres[i].emprunts.taille; r++) {
            fscanf(membres_db, " %100[^:]:%d:%d:%d/", membres->membres[i].emprunts.emprunt[r].code, &membres->membres[i].emprunts.emprunt[r].dateRetour.jour, &membres->membres[i].emprunts.emprunt[r].dateRetour.mois, &membres->membres[i].emprunts.emprunt[r].dateRetour.annee);
        }
    }
    
    fclose(membres_db);
}


int main(int argc, const char * argv[])
{
    
    Livres livresimport;
    db_loadLivres(&livresimport);
    
    int r;
    for(r = 0; r < livresimport.taille; r++) {
        printf("Livre %d\n", r+1);
        printf("\tNom   : ");
        printf("%s", livresimport.livres[r].nom);
        printf("\tAuteur: ");
        printf("%s", livresimport.livres[r].auteur);

        printf("\n---------------\n");
    }
    
    exit(EXIT_SUCCESS);
    
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
        fflush(stdin);
        printf("\tAuteur?: ");
        scanf("%49s", tab_livre[i].auteur);
        fflush(stdin);
        
        strcpy(tab_livre[i].code, "XXX-YYY");
        tab_livre[i].disponibles = 5;
        tab_livre[i].exemplaires = 31;

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
    
    Livres livres;
    livres.livres = tab_livre;
    livres.taille = nb_livre;
    
    db_saveLives(livres);

    // liberation tab
    free(tab_livre);

    return 0;
}
