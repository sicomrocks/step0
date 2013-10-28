#include <string.h>
#include "notify.h"
#include <stdio.h>
#include <stdlib.h>  // exit

#ifndef _FONCTIONS_H_
#define _FONCTIONS_H_

/*fonction pour regarder si le paramètre est bien un nom de registre.
 * Si oui, elle renvoie le numéro du registre (PC=32 SR=33 HI=34 LO=35, si non elle renvoie -1*/
int isregister(char* param);

int automate(char* nombre );
int isadress(char* param);
int adressType(char* param);

/*fonction qui vide le dictionnaire*/
void free_memory();


#endif
