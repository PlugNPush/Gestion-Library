//
//  global.c
//  Gestion-Library
//
//  Created by PlugN on 24/05/2020.
//  Copyright © 2020 PlugN. All rights reserved.
//

#include "global.h"

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); 
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}
