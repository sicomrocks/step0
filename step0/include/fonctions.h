#include <string.h>

#include "notify.h"

/*fonction pour regarder si le paramètre est bien un nom de registre.
 * Si oui, elle renvoie le numéro du registre (PC=32 SR=33 HI=34 LO=35, si non elle renvoie -1*/
int isregister(char* param);
