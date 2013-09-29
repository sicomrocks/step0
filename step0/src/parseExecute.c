#include "../include/parseExecute.h"
#include "notify.h"

#define CMD_OK_RETURN_VALUE 0
#define CMD_EXIT_RETURN_VALUE -1
#define CMD_EMPTY_RETURN_VALUE -2
#define CMD_UNKOWN_RETURN_VALUE -3

#define MAX_STR 1024

int execute_cmd_testcmd(int hexValue) {
    if(hexValue <= 0) {
        WARNING_MSG("command %s  - invalid call. The argument should be positive.\n", "test");
        return 2;
    }

    /* ecriture du resultat de la commande dans la sortie standard */
    fprintf(stdout, "CMD TEST RESULT 0x%x\n", hexValue + 1);

    /* Notez que c'est le SEUL fprintf(stdout ...) (donc le seul "printf") de tout le programme ! */

    return CMD_OK_RETURN_VALUE;
}

int parse_and_execute_cmd_testcmd(char * paramsStr) {
    DEBUG_MSG("Parametres : %s", paramsStr);
    /* la commande test attend un argument hexa */
    int hexValue;

    /* ATTENTION : dans cet exemple de code,
     on suppose que le nombre qui suit est bien au format hexa avec le %x.

     Votre simulateur devra etre capable de differencier les
     formats de nombre, eg 0x10 (hexa), 020 (octal), 16 (decimal)...*/

    if(sscanf(paramsStr, "%x", &hexValue) != 1 ) {
        WARNING_MSG("Invalid param : hexadecimal number awaited in '%s'\n", paramsStr);
        return 2;
    }
    return execute_cmd_testcmd(hexValue);
}


/***************/
int execute_cmd_exit() {
    INFO_MSG("Bye bye !");
    return CMD_EXIT_RETURN_VALUE;
}

int parse_and_execute_cmd_exit(char * paramsStr) {
    /*on ignore tous les parametres ! */
    return execute_cmd_exit();
}

int parse_and_execute_cmd_lp(char* paramsStr) {
	return 8;
}

int parse_and_execute_cmd_dr(char* paramsStr) {
	DEBUG_MSG("Parametres : %s", paramsStr);
	
	//regarder s'il y a un paramètre
	//s'il y en a un, vérifier que c'est bien un nom de registre
	//s'il y en a plusieurs, verifier que ce sont tous des noms de registres
	
	int i; //compteur du nombre de paramètres
	char* token;
	token=strtok(paramsStr, " ");
	//DEBUG_MSG("premier param : %s", token);
	if (token == NULL) {				//cas où il n'y en a pas
		DEBUG_MSG("Affichage de tous les registres");
		//return execute_cmd_dr0;
	}
	/*il faut vérifier que c'est bien le nom d'un registre */
	int c=isregister(token);
	if (c==0) {
		WARNING_MSG("Invalid param : HI LO PC SR or $x awaited in %s", token);
	}
	if (c==1) {
		DEBUG_MSG("le paramètre est bien un registre");
	}
	/*int j;
	for (j=0 ; j<32 ; j++) {
		if (registres[j]
	*/
	
	//return execute_cmd_dr(***);
	return 2;
}

int parse_and_execute_cmd_lr(char* paramsStr) {
	return 8;
}

