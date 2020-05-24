//
//  main.c
//  Gestion-Library
//
//  Created by PlugN on 05/05/2020.
//  Copyright © 2020 PlugN. All rights reserved.
//

#include "livres.h"
#include "membres.h"


int main(int argc, const char * argv[])
{
    
    Livres livres;
    db_loadLivres(&livres);
    
    //Membres membres;
    //db_loadMembres(&membres);
    
    //afficherMembres(membres);
    
    printf("\n/////////\n\n");
    
    //trierMembres(&membres);
    
    //afficherMembres(membres);
    
    //ajouterMembres(&membres);
    
    //afficherMembres(membres);
    
    //db_saveMembres(membres);
    
    afficherLivres(livres);
    
    ajouterLivres(&livres);
    
    afficherLivres(livres);

    db_saveLives(livres);

    return 0;
}
