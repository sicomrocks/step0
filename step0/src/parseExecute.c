#include "parseExecute.h"
#include <math.h>
#include "notify.h"
#include "reg.h"
#include <stdio.h>
#include "memoire.h"






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
			WARNING_MSG("Un seul paramètre est attendu ; l'exécution de lp va se poursuivre sans tenir compte des suivants");
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
			execute_cmd_lp(paramsStr);
			free(buffer);
			return CMD_OK_RETURN_VALUE;
		}

	}
	free(buffer);
	return 2;
}

int execute_cmd_lp(const char* filename) {
/*
	SectionELF *textSection;
    SectionELF *dataSection;
    SectionELF *bssSection;
*/
/*
	free(textSection);
    free(dataSection);
    free(bssSection);
*/
    /* Ne pas oublier d'allouer les differentes sections */
   // textSection = (SectionELF *) calloc(1,sizeof(SectionELF));
    //dataSection = (SectionELF *) calloc(1,sizeof(SectionELF));
    //bssSection  = (SectionELF *) calloc(1,sizeof(SectionELF));
	
    if (mipsloader(filename, textSection, dataSection, bssSection)) {
        fprintf(stderr,"**** ERREUR");
        exit(1) ;
    }

	//printf("mid lp\n");
	/*
     printf("\n------ Fichier ELF \"%s\" : sections lues lors du chargement ------\n", filename) ;
     printELFSection(textSection);
     printELFSection(dataSection);
     printELFSection(bssSection);
	*/
	 
    /* Faire le ménage avant de sortir */
   // free(textSection);
    //free(dataSection);
    //free(bssSection);

    return 2;
	
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
			free(buffer);
			return 2;
		}
		if (c>-1) {
			DEBUG_MSG("le paramètre est le registre %d", c);
			//free(buffer);
			execute_cmd_dr_un(c);
			//return CMD_OK_RETURN_VALUE;
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
				free(buffer);
				return 2;
			}
			if (c>-1) {
				DEBUG_MSG("le paramètre est le registre %d", c);
				execute_cmd_dr_un(c);
				//return CMD_OK_RETURN_VALUE;
			}
		}
	}

	return CMD_OK_RETURN_VALUE;
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
		free(buffer);
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
				free(buffer);
				return 2;
			}
			else {
				DEBUG_MSG("Deuxième paramètre : %s", token2);
				if (automate(token2)!=3 || strlen(token2)>10) {
					WARNING_MSG("Invalid param : 8 digit hexadecimal value awaited in %s", token2);
					free(buffer);
					return 2;
				}
				else {
					//on va regarder s'il y a trop de paramètres
					char* token3;
					token3=strtok(NULL, separateur);
					if (token3!=NULL) {
						WARNING_MSG("Seuls deux paramètres sont attendus dans lr");
						free(buffer);
						return 2;
					}
					//conversion de token2 en int :
					int value = (int)strtol(token2, NULL, 0);
					DEBUG_MSG("valeur à écrire %d = 0x%.8x", value, value);
					//free(buffer);
					execute_cmd_lr(c,value);
					free(buffer);
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
	//fprintf(stdout, "%d 0x%x\n", registres[num_reg].numero, registres[num_reg].valeur);
	execute_cmd_dr_un(num_reg);
	return CMD_OK_RETURN_VALUE;
}

int execute_cmd_da(char* adresse, int* nb_instructions) {
	WORD A=(int)strtol(adresse, NULL, 0);
	int N=(int)strtol(nb_instructions, NULL, 0);
	fprintf(stdout, "adresse de départ 0x%.8x, %d instructions\n", A, N);

	inst_da instruction;	//instruction après désassemblage

	//vérifier que A est bien dans la zone .text et qu'elle est bien alignée
	if (A>=textSection->size) {
		DEBUG_MSG("L'adresse spécifiée n'est pas dans la section .text");
		return 2;
	}

	DEBUG_MSG("A vaut 0x%x", A);
	if (A%4!=0) {
		DEBUG_MSG("adresse mal alignée");
		return 2;
	}
	
	//vérifier que A+N reste dans le fichier ; si ce n'est pas le cas on n'affiche pas
	

	//translater l'adresse virtuelle en adresse réelle
		//BYTE* data
		//WORD address

	/*int offset=&textSection;
		DEBUG_MSG("offset %d", offset);
		DEBUG_MSG("adresse réelle de textSection %d", &textSection);*/

	int numero=A-textSection->startAddress;	//c'est le numéro de l'octet dans la section .text

	//récupérer l'octet n° numero

	char* chaine;
	int i;
	for (i=0 ; i<4 ; i++) {
		sprintf(chaine+i*2, "%02x", textSection->data[numero+i]);
	}
	DEBUG_MSG("Récupération de l'instruction '%s' à l'adresse 0x%x", chaine, A);
	
	//mot=swap(mot)
	//mot=(int)strtol(&mot,NULL, 16);
		//mot=2;
		//DEBUG_MSG("%d", mot);
	//	mot=((mot>>24)&0xff) | ((mot<<8)&0xff0000) | ((mot>>8)&0xff00) | ((mot<<24)&0xff000000);
	//	DEBUG_MSG("%d", mot);

	//inst=désassemble(mot)
	desassemble(chaine);
		

	//afficher l'instruction désassemblée

	
	
	return CMD_OK_RETURN_VALUE;
}

int parse_and_execute_cmd_da(char* paramsStr) {
	DEBUG_MSG("Parametres : %s", paramsStr);

	/*deux paramètres : un numéro d'adresse
	 *		    un nombre entier d'instructions à afficher
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
			execute_cmd_da(token, token2);
			free(buffer);
			return CMD_OK_RETURN_VALUE;
		}
	}
	free(buffer);
	return 2;
}

int execute_cmd_lm(char* adresse,char* valeur) {
	DEBUG_MSG("%s %s", adresse, valeur);
	int A=(int)strtol(adresse, NULL, 0);
	int val=(int)strtol(valeur, NULL, 0);
	fprintf(stdout, "adresse 0x%.8x valeur 0x%.8x\n", A, val);
	return CMD_OK_RETURN_VALUE;
}

int parse_and_execute_cmd_lm(char* paramsStr)
{
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

/* @param adresse adresse à afficher 
 * @brief affiche le contenu de l'adresse
 */
int display_one(int adresse)
{
	//printPartELFSection(textSection,1,adresse);
	//printPartELFSection(dataSection,1,adresse);
	//printPartELFSection(bssSection,1,adresse);
	display_poly(adresse,1);
	return CMD_OK_RETURN_VALUE;
}

/* @param adresse adresse a partir de laquelle on affiche
 * @param nombre nombre d'octets à afficher
 * @brief affiche le contenu des "nombre" octets à partir de "adresse", 
 * @brief en parcourant toutes les sections.
 */
int display_poly(int adresse, int nombre)
{
	int a = adresse;
	int n0 = nombre;
	int n1 = nombre;
	
	if ( (a <textSection->startAddress + textSection->size) 
		&& (a >=textSection->startAddress) )  
		{	
			if (n0>=textSection->size - (a-textSection->startAddress))
				{
					n0 = textSection->size - (a-textSection->startAddress);
					n1 = n1-n0;
				}
			else { n1 = 0 ;}
		
			printPartELFSection(textSection,n0,a);

			n0=n1;
			a = dataSection->startAddress;
		}
		
	if ( (a <dataSection->startAddress + dataSection->size) 
		&& (a >=dataSection->startAddress) )  
		{	
			if (n0>=dataSection->size - (a-dataSection->startAddress))
				{
					n0 = dataSection->size - (a-dataSection->startAddress);
					n1 = n1-n0;
				}
			else { n1 = 0 ;}
		
			printPartELFSection(dataSection,n0,a);

			n0=n1;
			a = bssSection->startAddress;
		}
		
	if ( (a <bssSection->startAddress + bssSection->size) 
		&& (a >=bssSection->startAddress) )  
		{	
			if (n0>=bssSection->size - (a-bssSection->startAddress))
				{
					n0 = bssSection->size - (a-bssSection->startAddress);
					n1 = n1-n0;
				}
			else { n1 = 0 ;}
		
			printPartELFSection(bssSection,n0,a);
		}
		return CMD_OK_RETURN_VALUE;
}

int execute_cmd_dm(char** Adresses, int* type , int nb_adresses) 
{
	char* buffer;
	char* token;
	int m=0;
	int n=0;
	int j=0;
	int i=0;
	 
	while (i<nb_adresses)
	{	
		//fprintf(stdout, "CMD TEST RESULT Adresse %s Type %d\n", Adresses[i], type[i]);
		
		switch(type[i]) {
			case 1: // adresse simple
				display_one((int)strtol(Adresses[i],NULL,0));
				//fprintf(stdout, "Adresse 0x%.8x\n", ((int)strtol(Adresses[i],NULL,0)));
				
				break;
			case 2: // adresse avec nombre d'octets
				buffer = strdup(Adresses[i]);
				token = strtok(buffer,":");
				m = (int)strtol(token, NULL, 0);
				token = strtok(NULL,":");
				n = (int)strtol(token, NULL, 0);
				display_poly(m,n);
				//fprintf(stdout, "Adresse 0x%.8x, %d Octets\n", m,n);
				
				break;
			case 3: // intervalle d'adresse
				buffer = strdup(Adresses[i]);
				token = strtok(buffer,"~");
				m = (int)strtol(token, NULL, 0); 
				token = strtok(NULL,"~");
				n = (int)strtol(token, NULL, 0);
				if (n<m) {int temp = m; m=n; n=temp;}
			
				// On parcourt tous les cas possibles (corrects..) pour les deux adresses n et m
				
				if ((m <textSection->startAddress + textSection->size) 
				&& (m >=textSection->startAddress) ) // m appartient à text
				{	
					if ( n <dataSection->startAddress ) // n appartient à text
					{	
						if (n>textSection->size) 
							{display_poly(m,textSection->size-m);}
						else {display_poly(m,n-m+1);}
					}
					else if ( n <bssSection->startAddress ) // n appartient à data
					{	
						if (n-dataSection->startAddress >= dataSection->size) 
							{display_poly(m,textSection->size - m + dataSection->size);}
						else {display_poly(m,textSection->size - m + n - dataSection->startAddress+1);}
					}
					else // n appartient à bss
					{	
						if (n-bssSection->startAddress >= bssSection->size) 
							{display_poly(m,textSection->size - m + dataSection->size + bssSection->size);}
						else {display_poly(m,textSection->size - m  
								+ dataSection->size + n - bssSection->startAddress+1);}
					}
				}
				
				else if((m <dataSection->startAddress + dataSection->size) 
						&& (m >=dataSection->startAddress) ) // m appartient à data
				{	
					if ( n <bssSection->startAddress ) // n appartient à data
					{	
						if (n-dataSection->startAddress > dataSection->size) 
							{display_poly(m,dataSection->size-m);}
						else {display_poly(m,n-m+1);}
					}
					else // n appartient à bss
					{
						if (n-bssSection->startAddress >= bssSection->size) 
							{display_poly(m,dataSection->size - m + bssSection->size);}
						else {display_poly(m,dataSection->size - m + n - bssSection->startAddress+1);}
					}
				}
				else // m appartient à bss, donc n aussi
				{
					if (n-bssSection->startAddress > bssSection->size) 
							{display_poly(m,bssSection->size-m);}
					else {display_poly(m,n-m+1);}
				}
				
				if ( ( (m > textSection->startAddress + textSection->size) 
					&& (m < dataSection->startAddress) )
					|| ( (m > dataSection->startAddress + dataSection->size) 
					&& (m < bssSection->startAddress) )
					|| ( (m > bssSection->startAddress + bssSection->size) )
					)
					{ WARNING_MSG("Adresse 0x%.8x incorrecte\n",m);}
				if ( ( (n > textSection->startAddress + textSection->size) 
					&& (n < dataSection->startAddress) )
					|| ( (n > dataSection->startAddress + dataSection->size) 
					&& (n < bssSection->startAddress) )
					|| ( (n > bssSection->startAddress + bssSection->size) )
					)
					{ WARNING_MSG("Adresse 0x%.8x incorrecte\n",n);}	 
				
				for(j=0;j<=n-m;j++)
				{	
					//fprintf(stdout, "Adresse 0x%.8x\n", m+j);
				}
				break;
			}
		i++;
	}
	
	
	return CMD_OK_RETURN_VALUE;
}

int parse_and_execute_cmd_dm(char* paramsStr)
{
// Si l'entrée est nulle, msg d'erreur et on sort de la fonction
char* buffer0;
char* token0;
buffer0=strdup(paramsStr);
token0=strtok(buffer0," ");
if (token0==NULL){ 
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
			tokenespace=strtok(NULL," ");
		}
	compteur=0;

	// Definition du tableau qui recevra les Adresses à traiter par adressType
	char* adresses[nb_espace+1];
	int* type[nb_espace+1];
	int numero=0;
	// Initialisation du tableau
	while (numero!=nb_espace+2) {adresses[numero]=NULL; numero++;}
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
	if (adressType(token)==1) {	// cas où la première adresse est incorrecte
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
		return execute_cmd_dm(adresses,type,m);
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
	fprintf(stdout,"numero: %d\nnom: %s\ntype: %s\nnbe d'op: %d\n1è op: %s\n2è op: %s\n3è op: %s\nopcode: %x\nfunction: %x\n", n, DICO[n-1].nom, DICO[n-1].type, DICO[n-1].nbe_op, DICO[n-1].ops[0], DICO[n-1].ops[1], DICO[n-1].ops[2], DICO[n-1].opcode, DICO[n-1].func);
	return CMD_OK_RETURN_VALUE;
}
