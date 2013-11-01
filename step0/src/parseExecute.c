#include "parseExecute.h"
#include "notify.h"
#include "reg.h"
#include <stdio.h>
#include "memoire.h"

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
    fprintf(stdout, "CMD TEST RESULT 0x%.8x\n", hexValue + 1);

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
    free_memory();
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
		return 2;
	}
	else {
		//on regarde s'il y a d'autres paramètres
		token2=strtok(NULL, separateur);
		if (token2!=NULL) {
			WARNING_MSG("Un seul paramètre est attendu");
			return 2;
		}
		FILE* fichier=NULL;
		fichier=fopen(token, "r");
		if (fichier==NULL) {
			WARNING_MSG("Impossible d'ouvrir %s", token);
			return 2;
		}
		else {
			DEBUG_MSG("Ouverture réussie de %s", token);
			free(buffer);
			return execute_cmd_lp(token);
		}

	}
	free(buffer);
	return 2;
}
int execute_cmd_lp(char* token) {
	fprintf(stdout, "ouverture réussie\n", token);
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
		free(buffer);
		execute_cmd_dr_tous();
		return CMD_OK_RETURN_VALUE;
	}
	else {
		DEBUG_MSG ("paramètre numéro %d : %s",nb_param, token);
		c=isregister(token);
		if (c<0) {
			WARNING_MSG("Invalid param : HI LO PC SR or $x awaited in %s", token);
			return 2;
		}
		if (c>-1) {
			DEBUG_MSG("le paramètre est le registre %d", c);
			free(buffer);
			execute_cmd_dr_un(c);
			return CMD_OK_RETURN_VALUE;
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
				return CMD_OK_RETURN_VALUE;
			}
		}
	}

	return 2;
}

int execute_cmd_dr_un(int num_registre) {
	switch(num_registre) {
		case 32:
			fprintf(stdout, "PC : 0x%.8x\n", registres[num_registre].valeur);
			return CMD_OK_RETURN_VALUE;
			//break;
		case 33:
			fprintf(stdout, "SR : 0x%.8x\n", registres[num_registre].valeur);
			return CMD_OK_RETURN_VALUE;
			//break;
		case 34:
			fprintf(stdout, "HI : 0x%.8x\n", registres[num_registre].valeur);
			return CMD_OK_RETURN_VALUE;
			//break;
		case 35:
			fprintf(stdout, "LO : 0x%.8x\n", registres[num_registre].valeur);
			return CMD_OK_RETURN_VALUE;
			//break;
		default:
			fprintf(stdout, "%.2d : 0x%.8x\n", registres[num_registre].numero, registres[num_registre].valeur);
			return CMD_OK_RETURN_VALUE;
			//fprintf(stdout, "CMD TEST RESULT %d\n", num_registre);
	}	
	return 2;
}

int execute_cmd_dr_tous() {
	int i;
	for (i=0 ; i<36 ; i++) {
		execute_cmd_dr_un(i);
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
		return 2;
	}
	else {
		int c=isregister(token);
		if (c < 0) {
			WARNING_MSG("Invalid param : register name awaited in %s", token);
			free(buffer);
			return 2;
		}
		else {
			DEBUG_MSG("%s est le registre %d", token, c);
			if (c==0) {
				WARNING_MSG("On ne peut pas écrire dans ce registre");
				free(buffer);
				return 2;
			}
			//tester si le 2è param est de la bonne forme
			char* token2=NULL;
			token2=strtok( NULL, separateur  );
			if (token2==NULL) {
				WARNING_MSG("Invalid param : 32 bits hexadecimal value missing after %s", token);
				return 2;
			}
			else {
				DEBUG_MSG("Deuxième paramètre : %s", token2);
				if (automate(token2)!=3 || strlen(token2)>10) {
					WARNING_MSG("Invalid param : 8 digit hexadecimal value awaited in %s", token2);
					return 2;
				}
				else {
					//on va regarder s'il y a trop de paramètres
					char* token3;
					token3=strtok(NULL, separateur);
					if (token3!=NULL) {
						WARNING_MSG("Seuls deux paramètres sont attendus dans lr");
						return 2;
					}
					//conversion de token2 en int :
					int value = (int)strtol(token2, NULL, 0);
					DEBUG_MSG("valeur à écrire %d = 0x%.8x", value, value);
					free(buffer);
					execute_cmd_lr(c,value);
					return CMD_OK_RETURN_VALUE;
				}
			}

		}
	}
	free(buffer);
	return 2;
}

int execute_cmd_lr(int num_reg, int value) {
	//FILE* output=fopen("test/10_lr.out", "w");
	registres[num_reg].valeur=value;
	//fprintf(stdout, "CMD TEST RESULT %d 0x%x\n", registres[num_reg].numero, registres[num_reg].valeur);
	execute_cmd_dr_un(num_reg);
	return CMD_OK_RETURN_VALUE;
}

int execute_cmd_da(char* adresse, int* nb_instructions) {
	int A=(int)strtol(adresse, NULL, 0);
	int N=(int)strtol(nb_instructions, NULL, 0);
	fprintf(stdout, "CMD TEST RESULT adresse de départ 0x%.8x, %d instructions\n", A, N);
	return CMD_OK_RETURN_VALUE;
}

int parse_and_execute_cmd_da(char* paramsStr) {
	DEBUG_MSG("Parametres : %s", paramsStr);

	/*deux paramètres : un numéro d'adresse
	 *					nombre entier d'instructions à afficher
	 */

	// Pour le strtok
	char* token;
	char* separateur = { " " };
	char* buffer;

	buffer = strdup ( paramsStr );

	// premier appel, pour vérifier s'il y a des paramètres
	token = strtok( buffer, separateur  );
	if (token == NULL) /* cas où il n'y a pas de paramètres*/ {	
		WARNING_MSG("Invalid param : address and value awaited");
	}
	else if (isadress(token) == 0) {	
		WARNING_MSG("Invalid param : valid address awaited in first parameter");
	}
	else if (isadress(token) == 1) /*tester si le 2è param est de la bonne forme*/ {
		char* token2=NULL;
		token2=strtok( NULL, separateur  );
		if (token2==NULL || automate(token2)!=4) /* automate = 4 si DECIMAL*/{
			WARNING_MSG("Nombre entier attendu après %s", token);
		}
		else {
			free(buffer);
			return execute_cmd_da(token, token2);
		}
	}
	free(buffer);
	return 2;
}

int execute_cmd_lm(char* adresse,char* valeur) {
	DEBUG_MSG("%s %s", adresse, valeur);
	int A=(int)strtol(adresse, NULL, 0);
	int val=(int)strtol(valeur, NULL, 0);
	fprintf(stdout, "CMD TEST RESULT adresse 0x%.8x valeur 0x%.8x\n", A, val);
	return CMD_OK_RETURN_VALUE;
}

int parse_and_execute_cmd_lm(char* paramsStr) {
    // Définition des sorties de la fonction
	char* adresse=NULL;
	char* valeur=NULL;

	DEBUG_MSG("Parametres : %s", paramsStr);

	/*deux paramètres : une adresse valide
	 * 		    		une valeur entière de 1 à 8 chiffres hexa
	 */

	char* token;
	char* separateur = { " " };
	char* buffer;
	char* buffer2;
	int compteur=0;

	buffer = strdup ( paramsStr );
	buffer2 = strdup ( paramsStr ); // utilisé à la fin pour adresse et valeur

	// premier appel, pour vérifier s'il y a des paramètres
	token = strtok( buffer, separateur);
	if (token == NULL) {		// cas où il n'y a pas de paramètres
		WARNING_MSG("Invalid param : address and hexadecimal value awaited in %s", token);
	}

    else if (isadress(token)==0)
    WARNING_MSG("Invalid param : hexadecimal address expected in first parameter");

	else if (isadress(token)==1)
	{   adresse=token; // utile pour la fin

		// Tester si le 2è paramètre est de la bonne forme
		char* token2;
		token2=strtok(NULL,separateur);

		do
		{
            if ( (token2==NULL && compteur==0) || (token2!=NULL && automate(token2)!=3) || strlen(token2)>10)
            {	WARNING_MSG("Invalid value : 1 to 8 hexa numbers are expected");
                free(buffer);
                free(buffer2);
                return 2;
            }
            else
            {   compteur=compteur +1;
                token2=strtok( NULL,separateur);
                if (token2!=NULL && compteur==8)
                {   WARNING_MSG("Invalid value : too many parameters");
                    free(buffer);
                    free(buffer2);
                    return 2;
                }
            }
		} while ( token2!=NULL  && compteur < 8);
		adresse = strtok( buffer2, separateur);
		valeur = strtok( NULL, ".");
		DEBUG_MSG("adresse = %s\nvaleur = %s",adresse,valeur);
	}
	free(buffer);
	free(buffer2);
	return execute_cmd_lm(adresse, valeur);
}

int execute_cmd_dm(char** Adresses, int* type, int nb_adresses) {
	char* buffer;
	char* token;
	int m=0;
	int n=0;
	int j=0;

	int i=0;
	//fprintf(stdout, "j=%d\n", nb_adresses);
	while (i<nb_adresses) 
	{	
		//fprintf(stdout, "CMD TEST RESULT Adresse %s Type %d\n", Adresses[i], type[i]);
		
		switch(type[i]) {
			case 1: // adresse simple
				//display_one(Adresses[i]);
				fprintf(stdout, "CMD TEST RESULT Adresse 0x%.8x\n", ((int)strtol(Adresses[i],NULL,0)));
				break;
			case 2: // adresse avec nombre d'instructions
				buffer = strdup(Adresses[i]);
				token = strtok(buffer,":");
				m = (int)strtol(token, NULL, 0);
				token = strtok(NULL,":");
				n = (int)strtol(token, NULL, 0);
				//display_inst(m,n);
				fprintf(stdout, "CMD TEST RESULT Adresse 0x%.8x, %d Instructions\n", m,n);
				break;
			case 3: // intervalle d'adresse
				buffer = strdup(Adresses[i]);
				token = strtok(buffer,"~");
				m = (int)strtol(token, NULL, 0);
				token = strtok(NULL,"~");
				n = (int)strtol(token, NULL, 0);
				if (n<m) {int temp = m; m=n; n=temp;}
			
				for(j=0;j<=n-m;j++)
				{	//display_one(m+j);
					fprintf(stdout, "CMD TEST RESULT Adresse 0x%.8x\n", m+j);
				}
				break;
			}
		i++;
	}
	
	
	return CMD_OK_RETURN_VALUE;
}

int parse_and_execute_cmd_dm(char* paramsStr) {
// Si l'entrée est nulle, msg d'erreur et on sort de la fonction
char* buffer0;
char* token0;
buffer0=strdup(paramsStr);
token0=strtok(buffer0," ");
if (token0==NULL){ 
	WARNING_MSG("Missing one or more parameters");
	free(buffer0);
	return 2;
}

else {
	// Déterminer le nombre d'espaces (donc d'adresses)
	char* bufferespace;
	bufferespace = strdup (paramsStr);
	char* tokenespace;
	int compteur=0;
	int nb_espace;

	tokenespace = strtok(bufferespace," ");
		while (tokenespace!=NULL)
		{
			compteur++;
			nb_espace=compteur-1;
			tokenespace=strtok(NULL,"|");
		}
	compteur=0;

	// Definition du tableau qui recevra les Adresses à traiter par adressType
	char* adresses[nb_espace+1];
	int* type[nb_espace+1];
	int numero=0;
	// Initialisation du tableau
	while (numero!=nb_espace+2) {
		adresses[numero]=NULL;
		numero++;
	}
	numero=0;


	char* token=NULL;
	char* token2=NULL;
	char* buffer=NULL;
	char* buffer2=NULL;
	char* separateur = {" "};

	buffer=strdup(paramsStr);
	buffer2=strdup(paramsStr);
	token = strtok(buffer,separateur);

	if (token == NULL) {		// cas où il n'y a pas de paramètres
		WARNING_MSG("Invalid param : Address(es) expected\n");
		free(buffer);
		free(buffer2);
		free(bufferespace);
		return 2;
	}
	if (adressType(token)==1) {	// cas où la première adresse est incorrect
	WARNING_MSG("First address invalid\n");
	free(buffer);
	free(buffer2);
	free(bufferespace);
	return 2;
	}
	else
	{	// la première adresse est correcte
		adresses[numero]=token;
		token2=strtok(buffer2,separateur);

		//Boucle qui remplit le tableau avec les adresses
		while(token2!=NULL)
		{	numero++;
			token2=strtok(NULL,separateur);
			if (token2!=NULL) {adresses[numero]=token2;}
		}
		free(token2);

		// Boucle qui vérifie les types des adresses
		int m=0;
		int n;
		while (m<nb_espace+1)
		{n = adressType(adresses[m]);
		type[m]=n-1;
		if (n==1) {
			WARNING_MSG("adresse numero %d invalide\n",m+1);
			free(buffer);
			free(buffer2);
			free(bufferespace);
			return 2;
		} 
		// Si l'adresse est fausse, on sort du programme
		// Sinon, on poursuit jusqu'à commande OK
		m++;
		
		}
		return execute_cmd_dm(adresses, type, m);
	}
}
/*free(buffer);
free(bufferbarre);
free(buffer2);*/
return 2;
}

int parse_and_execute_cmd_inst(char* paramsStr) {
	DEBUG_MSG("Parametres : %s", paramsStr);
	int num=atoi(paramsStr);
	DEBUG_MSG("instruction %d", num);
	if (num == 0 || num > 25) {
		WARNING_MSG("les instructions vont de 1 à 25");
		return 2;
	}
	else {
		return execute_cmd_inst(num);
	}
}

int execute_cmd_inst(int n) {
	fprintf(stdout,"numero: %d nom: %s type: %s nbe d'op: %d 1è op: %s 2è op: %s 3è op: %s opcode: 0x%x function: 0x%x\n", n, DICO[n-1].nom, DICO[n-1].type, DICO[n-1].nbe_op, DICO[n-1].ops[0], DICO[n-1].ops[1], DICO[n-1].ops[2], DICO[n-1].opcode, DICO[n-1].func);
	return CMD_OK_RETURN_VALUE;
}
