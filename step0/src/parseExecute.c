#include "parseExecute.h"
#include "notify.h"
#include "reg.h"
#include <stdio.h>

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

int execute_cmd_exit() {
    INFO_MSG("Bye bye !");
    return CMD_EXIT_RETURN_VALUE;
}

int parse_and_execute_cmd_exit(char * paramsStr) {
    /*on ignore tous les parametres ! */
    return execute_cmd_exit();
}

int parse_and_execute_cmd_lp(char* paramsStr) {
	DEBUG_MSG("Parametres : %s", paramsStr);
	
	//un paramètre : nom de fichier
	//vérifier que le fichier appelé existe
	
	char* token;
	char* token2;
	char* separateur = { " " };
	char* buffer;
	
	buffer = strdup(paramsStr);

	// premier appel, pour vérifier s'il y a des paramètres
	token = strtok( buffer, separateur  );
	if (token == NULL) {		// cas où il n'y a pas de paramètres
		WARNING_MSG("Invalid param : file name awaited");
	}
	else {
		//on regarde s'il y a d'autres paramètres
		token2=strtok(NULL, separateur);
		if (token2!=NULL) {
			WARNING_MSG("Un seul paramètre est attendu ; l'exécution de lp va se poursuivre sans tenir compte des suivants");
		}
		FILE* fichier=NULL;
		fichier=fopen(token, "r");
		if (fichier==NULL) {
			WARNING_MSG("Impossible d'ouvrir %s", token);
			//return 2;
		}
		else {
			DEBUG_MSG("Ouverture réussie de %s", token);
			return execute_cmd_lp(token);
		}
		
	}
	return 2;
}
int execute_cmd_lp(char* token) {
	return CMD_OK_RETURN_VALUE;
}

int parse_and_execute_cmd_dr(char* paramsStr) {
	DEBUG_MSG("Parametres : %s", paramsStr);
	
	//regarder s'il y a un paramètre
	//s'il y en a un, vérifier que c'est bien un nom de registre
	//s'il y en a plusieurs, verifier que ce sont tous des noms de registres
	
	char* token;
	char* separateur = { " " };
	char* buffer;
	int nb_param=1;		//compteur du nombre de paramètres
	int c;
	
	buffer = strdup ( paramsStr );

	// premier appel, pour vérifier s'il y a des paramètres
	token = strtok( buffer, separateur  );
	if (token == NULL) {		// cas où il n'y a pas de paramètres
		WARNING_MSG("Affichage de tous les registres");
		return execute_cmd_dr_tous();
	}
	else {
		DEBUG_MSG ("paramètre numéro %d : %s",nb_param, token);
		c=isregister(token);
		if (c<0) {
			WARNING_MSG("Invalid param : HI LO PC SR or $x awaited in %s", token);
		}
		if (c>-1) {
			DEBUG_MSG("le paramètre est le registre %d", c);
			execute_cmd_dr_un(c);
		}
	}
	    
	while( token != NULL ) {
		// Cherche les autres paramètres
		token = strtok( NULL, separateur  );
		if ( token != NULL ) {		
			nb_param++; // incrément du nombre de paramètres	
			DEBUG_MSG ("paramètre numéro %d : %s",nb_param, token);
			c=isregister(token);
			if (c<0) {
				WARNING_MSG("Invalid param : HI LO PC SR or $x awaited in %s", token);
			}
			if (c>-1) {
				DEBUG_MSG("le paramètre est le registre %d", c);
				execute_cmd_dr_un(c);
			}
		}
	}

	return 2;
}

int execute_cmd_dr_un(int num_registre) {
	fprintf(stdout, "registre %d valeur 0x%x\n", registres[num_registre].numero, registres[num_registre].valeur);
	//fprintf(stdout, "CMD TEST RESULT %d\n", num_registre);
	return CMD_OK_RETURN_VALUE;
}

int execute_cmd_dr_tous() {
	int i;
	for (i=0 ; i<36 ; i++) {
		execute_cmd_dr_un(i);
		//fprintf(stdout, "CMD TEST RETURN RESULT %d\n", i);
	}
	return CMD_OK_RETURN_VALUE;
}

int parse_and_execute_cmd_lr(char* paramsStr) {
	DEBUG_MSG("Parametres : %s", paramsStr);
	
	/*deux paramètres : un nom de registre valide
	 * 		    une valeur hexa de 1 à 8 chiffres hexa
	*/
	
	char* token;
	char* separateur = { " " };
	char* buffer;
	
	buffer = strdup ( paramsStr );

	// premier appel, pour vérifier s'il y a des paramètres
	token = strtok( buffer, separateur  );
	if (token == NULL) {		// cas où il n'y a pas de paramètres
		WARNING_MSG("Invalid param : register and hexadecimal value awaited in %s", token);
	}
	else {
		int c=isregister(token);
		if (c < 0) {
			WARNING_MSG("Invalid param : register name awaited in %s", token);
			return 2;
		}
		else if (c > -1) {
			DEBUG_MSG("%s est le registre %d", token, c);
			//tester si le 2è param est de la bonne forme
			char* token2=NULL;
			token2=strtok( NULL, separateur  );
			if (token2==NULL) {
				WARNING_MSG("Invalid param : 32 bits hexadecimal value missing after %s", token);
			}
			else {
				DEBUG_MSG("Deuxième paramètre : %s", token2);
				int S=automate(token2);
				if (automate(token2)!=3 || strlen(token2)>10) {
					WARNING_MSG("Invalid param : 8 digit hexadecimal value awaited in %s", token2);
				}
				else {
					//on va regarder s'il y a trop de paramètres
					char* token3;
					token3=strtok(NULL, separateur);
					if (token3!=NULL) {
						WARNING_MSG("Seuls deux paramètres sont attendus dans lr, la fonction ne tient pas compte des suivants");
					}
					//conversion de token2 en it :
					int value = (int)strtol(token2, NULL, 0);
					DEBUG_MSG("valeur à écrire %d", value);
				
					return execute_cmd_lr(c,value);
				}
			}
		
		}
	}
	return 2;
}

int execute_cmd_lr(int num_reg, int value) {
	//FILE* output=fopen("test/10_lr.out", "w");
	registres[num_reg].valeur=value;
	fprintf(stdout, "CMD TEST RESULT %d 0x%x\n", registres[num_reg].numero, registres[num_reg].valeur);
	return CMD_OK_RETURN_VALUE;
}
