#include "parseExecute.h"
#include <math.h>
#include "notify.h"
#include "reg.h"
#include <stdio.h>
#include "fonctions.h"
//#include "memoire.h"






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
	//free_memory();
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
	return CMD_OK_RETURN_VALUE;
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
	return CMD_OK_RETURN_VALUE;
}

int execute_cmd_lr(int num_reg, int value) {
	//FILE* output=fopen("test/10_lr.out", "w");
	registres[num_reg].valeur=value;
	//fprintf(stdout, "%d 0x%x\n", registres[num_reg].numero, registres[num_reg].valeur);
	execute_cmd_dr_un(num_reg);
	return CMD_OK_RETURN_VALUE;
}

int execute_cmd_da(char* adresse, char* nb_instructions) {
	WORD A=(int)strtol(adresse, NULL, 0);
	int N=(int)strtol(nb_instructions, NULL, 0); 
	//fprintf(stdout, "adresse de départ 0x%.8x, %d instructions\n", A, N);

	//vérifier que A est bien dans la zone .text et qu'elle est bien alignée
	if (textSection->size==0) {
		fprintf(stdout, "Aucun programme n'est chargé\n");
		return 2;
	}
	
	if (A>=textSection->size) {
		fprintf(stdout, "L'adresse spécifiée n'est pas dans la section .text\n");
		return 2;
	}

	//DEBUG_MSG("A vaut 0x%x", A);
	if (A%4!=0) {
		fprintf(stdout, "Adresse mal alignée\n");
		return 2;
	}
	
	//vérifier que A+N reste dans le fichier ; si ce n'est pas le cas on n'affiche pas
	if (A+4*N-1 >= textSection->size) {
		fprintf(stdout, "Il n'y a pas autant d'instructions à afficher à partir de l'adresse 0x%.8x\n", A);
		return 2;
	}

		//////////////////////////////////////////
		//					//
		//	Début du désassemblage		//
		//					//
		//////////////////////////////////////////

	int z=0;
	while (z<N) {	

		//trouver la position de l'instruction A dans la section .text
		int numero=A-textSection->startAddress;

		//récupérer l'octet n° numero
		char* chaine=calloc(1,sizeof(*chaine));
		int i;
		for (i=0 ; i<4 ; i++) {
			sprintf(chaine+i*2, "%02x", textSection->data[numero+i]);
		}
		//INFO_MSG("Récupération de l'instruction '%s' à l'adresse 0x%x", chaine, A);
	
		INSTRUCTION decode; //résultat du désassemblage
		decode=desassemble(chaine);
		
		printf("0x%.8x %s ",A,chaine);
		//afficher l'instruction désassemblée
		affiche_inst_brut(decode,A);

	A=A+4;
	z++;
	}
	
	return CMD_OK_RETURN_VALUE;
}

INSTRUCTION execute_cmd_da_un(char* adresse) {
	WORD A=(int)strtol(adresse, NULL, 0);
	
		//////////////////////////////////////////
		//					//
		//	Début du désassemblage		//
		//					//
		//////////////////////////////////////////

		
		//trouver la position de l'instruction A dans la section .text
		int numero=A-textSection->startAddress;

		//récupérer l'octet n° numero
		char* chaine=calloc(1,sizeof(*chaine));
		int i;
		for (i=0 ; i<4 ; i++) {
			sprintf(chaine+i*2, "%02x", textSection->data[numero+i]);
		}
	//	INFO_MSG("Récupération de l'instruction '%s' à l'adresse 0x%x", chaine, A);
	
		INSTRUCTION decode; //résultat du désassemblage
		decode=desassemble(chaine);

		//afficher l'instruction désassemblée
		printf("0x%.8x %s ",A,chaine);
		affiche_inst_brut(decode,A);
		//affiche_inst(decode);
	
	
	return decode;
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
	return CMD_OK_RETURN_VALUE;
}

int execute_cmd_lm(char* adresse,char* valeur) {
	//DEBUG_MSG("adresse %s valeur %s", adresse, valeur);
	int val = (int)strtol(valeur,NULL,0);
	char vstring[10];
	sprintf(vstring,"0x%.8x",val);
	//DEBUG_MSG("vstring = %s\n",vstring);
	char vtemp[4];
	
	int i;
	int a = (int)strtol(adresse,NULL,0);
	
	for (i=0;i<=3;i++)
	{	//DEBUG_MSG("a=%d",a);
		sprintf(vtemp,"0x%c%c",vstring[2*i+2],vstring[2*i+3]);
		val = (int)strtol(vtemp,NULL,0);
		//DEBUG_MSG("valeur%d = %d",i,valeur);
		
		if ( (a <textSection->startAddress + textSection->size) 
			&& (a >=textSection->startAddress) )  
			{	textSection->data[a-textSection->startAddress]=val;
			}
			
		if ( (a <dataSection->startAddress + dataSection->size) 
			&& (a >=dataSection->startAddress) )  
			{	dataSection->data[a-dataSection->startAddress]=val;
			}
			
		if ( (a <bssSection->startAddress + bssSection->size) 
			&& (a >=bssSection->startAddress) )  
			{	bssSection->data[a-bssSection->startAddress]=val;
			}
		a = a + 1;
	}
	return CMD_OK_RETURN_VALUE;
}

int parse_and_execute_cmd_lm(char* paramsStr)
{
    // Définition des sorties de la fonction
	DEBUG_MSG("Parametres : %s", paramsStr);

	/*deux paramètres : une adresse valide
	 * 		    		une valeur entière de 1 à 8 chiffres hexa
	 */

	char* token;
	char* token2;
	char* separateur = { " " };
	char* buffer;
	buffer = strdup ( paramsStr );

	// premier appel, pour vérifier s'il y a des paramètres
	token = strtok( buffer, separateur);
	if (token == NULL) {		// cas où il n'y a pas de paramètres
		WARNING_MSG("Invalid param : address and hexadecimal value awaited in %s", token);
		return 2;
	}

    else if (isadress(token)==0)
    WARNING_MSG("Invalid param : hexadecimal address expected in first parameter");

	else if (isadress(token)==1)
	{  	// Tester si le 2è paramètre est de la bonne forme
		token2=strtok(NULL,separateur);
		
        if ( token2==NULL || automate(token2)!=3 || strlen(token2)>10)
        {	WARNING_MSG("Invalid value : 1 to 8 hexa numbers are expected");
			free(buffer);
			return 2;
		}
	}
	DEBUG_MSG("adresse = %s\nvaleur = %s",token,token2);
return execute_cmd_lm(token, token2);
}



int execute_cmd_dm(char** Adresses, int* type , int nb_adresses) 
{	char* buffer;
	char* token;
	int m=0;
	int n=0;
	//int j=0;
	int i=0;
	 
	while (i<nb_adresses)
	{	
		
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
					{ WARNING_MSG("Adresse 0x%.8x incorrecte\n",m);
						return 2;}
				if ( ( (n > textSection->startAddress + textSection->size) 
					&& (n < dataSection->startAddress) )
					|| ( (n > dataSection->startAddress + dataSection->size) 
					&& (n < bssSection->startAddress) )
					|| ( (n > bssSection->startAddress + bssSection->size) )
					)
					{ WARNING_MSG("Adresse 0x%.8x incorrecte\n",n);
						return 2;}	 
				
				/*for(j=0;j<=n-m;j++)
				 *	
				 *	//fprintf(stdout, "Adresse 0x%.8x\n", m+j);
				 */
				break;
			}
		i++;
	}
	
	
	return CMD_OK_RETURN_VALUE;
}


int parse_and_execute_cmd_dm(char* paramsStr)
{// Si l'entrée est nulle, msg d'erreur et on sort de la fonction
	char* buffer0;
	char* token0;
	buffer0=strdup(paramsStr);
	token0=strtok(buffer0," ");
	
	if (!(token0==NULL))
	{
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
		int type[nb_espace+1];
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

		if ( !(isnull(token)) || (!(isadtype1(token))) )
		{	// la première adresse est correcte
			adresses[numero]=token;
			token2=strtok(buffer2,separateur);

			//Boucle qui remplit le tableau avec les adresses
			while(!(isnull(token2)))
			{	numero++;
				token2=strtok(NULL,separateur);
				if (!(isnull(token))) {adresses[numero]=token2;}
			}
			free(token2);
			
			// Boucle qui vérifie les types des adresses
			int m=0;
			int n;
			while (m<nb_espace+1)
			{n = adressType(adresses[m]);
			type[m]=n-1;

			if ((isadtype1(adresses[m]))) {
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
			execute_cmd_dm(adresses,type,m);
		}
		else{		// cas où il n'y a pas de paramètres
			WARNING_MSG("Invalid param : Addresses invalid \n");
			free(buffer);
			free(buffer2);
			free(bufferespace);
			return 2;
		}
		/* if (adressType(token)==1) {	// cas où la première adresse est incorrecte
		WARNING_MSG("First address invalid\n");
		free(buffer);
		free(buffer2);
		free(bufferespace);
		return 2;
		}
		else
		*/
	}
	
	else { 
		free(buffer0);
	//	display_poly(0,textSection->size + dataSection->size + bssSection->size);
		return 2;
	}
	
	
	/*free(buffer);
	free(bufferbarre);
	free(buffer2);*/
	return CMD_OK_RETURN_VALUE;
}


int parse_and_execute_cmd_inst(char* paramsStr) {
	DEBUG_MSG("Parametres : %s", paramsStr);
	int num=atoi(paramsStr);
	DEBUG_MSG("instruction %d", num);
	if (num == 0 || num > taille_DICO) {
		WARNING_MSG("les instructions vont de 1 à %d", taille_DICO);
		return 2;
	}
	else {
		execute_cmd_inst(num);
		return CMD_OK_RETURN_VALUE;
	}
}

int execute_cmd_inst(int n) {
	//fprintf(stdout,"numero: %d\nnom: %s\ntype: %s\nnbe d'op: %d\n1è op: %s\n2è op: %s\n3è op: %s\nopcode: %s\nfunction: %s\n",
	//n, DICO[n-1].nom, DICO[n-1].type, DICO[n-1].nbe_op, DICO[n-1].ops[0], DICO[n-1].ops[1], DICO[n-1].ops[2], DICO[n-1].opcode, DICO[n-1].func);
	
	return CMD_OK_RETURN_VALUE;
}


int execute_cmd_run(char* adresse_debut)
{	
	int compteur;
	int res;
	char mot[12];
	INSTRUCTION inst; 
	
	compteur = 0;
	
	while (compteur< 50 && registres[32].valeur<textSection->size /* ||isbp(registres[32].valeur)==0*/)
	{	sprintf(mot,"0x%.8x",registres[32].valeur);
		inst=execute_cmd_da_un(mot);
		res=exec_inst(inst,registres[32].valeur);
		
		if (res==0)
		{ 	ERROR_MSG("erreur adresse 0x%.8x",registres[32].valeur);
		}
		else
		 {
		registres[32].valeur = registres[32].valeur + 4;
		compteur ++;
		}
	}
	
	return CMD_OK_RETURN_VALUE;
}

int parse_and_execute_cmd_run(char* paramsStr)
{
	char* token;
	char* buffer;
	token = NULL;
	
	buffer = paramsStr;
	token = strtok(buffer," ");
	if (token==NULL || buffer==NULL) 
	{	//registres[32].valeur=0;
		DEBUG_MSG("Lancement du MIPS32 à l'actuel PC : 0x%.8x",registres[32].valeur);
	}
	else if (isaddressbusy(token)==1 || isaddressbusy(buffer)==1)
	{	registres[32].valeur=(int)strtol(token,NULL,0);
		DEBUG_MSG("Lancement du MIPS32 à PC : 0x%.8x",registres[32].valeur);
	}
	
	return execute_cmd_run(token);
}

int parse_and_execute_cmd_s(char* paramsStr)
{
	return CMD_OK_RETURN_VALUE;
}

int execute_cmd_s(char* paramsStr)
{
	return CMD_OK_RETURN_VALUE;
}

int parse_and_execute_cmd_si(char* paramsStr)
{
	if (!(paramsStr==NULL))
	{ERROR_MSG("Pas de parametres attendus en entrée");
	}
	
	else return execute_cmd_si(paramsStr); 
}

int execute_cmd_si(char* paramsStr)
{	char newbp[10];
	int J;
	J=0;
	if(J!=0) {
		DEBUG_MSG("shouldn't be there");
	}
	// On ajoute un nouveau breakpoint
	if (isbp(registres[32].valeur + 4)==1)
	{	J=1;}
	else 
	{	sprintf(newbp,"0x%.8x",registres[32].valeur + 4);
		//execute_cmd_bp(newbp);
	}
	
	execute_cmd_run(paramsStr); // run pour une étape
	
	//if (J==0) er(newbp);

	
	return CMD_OK_RETURN_VALUE;
} 

int parse_and_execute_cmd_bp(char* paramsStr) {
	DEBUG_MSG("Parametres : %s", paramsStr);

	//verifier que c'est une adresse
	
	if (isadress(paramsStr)==0) {
		fprintf(stdout, "Invalid parameter : valid address awaited\n");
		return 2;
	}

	//vérifier que c'est dans la section .text

	if (textSection->size==0) {
		fprintf(stdout, "aucun programme n'est chargé\n");
		return 2;
	}

	WORD A=(int)strtol(paramsStr, NULL, 0);
	DEBUG_MSG("l'adresse est 0x%.8x", A);

	if (A>=textSection->size) {
		fprintf(stdout, "L'adresse spécifiée n'est pas dans la section .text\n");
		return 2;
	}

	if (A<=textSection->size) {
		DEBUG_MSG("L'adresse est dans la zone .text");
	}
	
	if (A%4!=0) {
		fprintf(stdout, "Adresse mal alignée\n");
		return 2;
	}

	if (A%4==0) {
		DEBUG_MSG("adresse bien alignée");
	}

	DEBUG_MSG("adresse valide");
	

	//vérifier que cette adresse n'a pas déjà un breakpoint

	int s;
	s=recherche(liste_bp, A);

	if (s==0) {
		//ajouter cette adresse dans la liste
		execute_cmd_bp(A);
		fprintf(stdout, "élément ajouté dans la liste des breakpoints\n");
	}

	if (s!=0) {
		fprintf(stdout, "élément déjà compris dans les breakpoints\n");
		/*fprintf(stdout, "afficher la liste des breakpoints ? o/[n]\n");
		char reponse;
		scanf("%c", &reponse);
		if (reponse=='o') {
			visualiser_liste(liste_bp);
		}*/
	}

	
	
	return CMD_OK_RETURN_VALUE;
}

int execute_cmd_bp(unsigned int A)
{

	//visualiser_liste(liste_bp);
	breakpoint c;
		c.bp=A;
		c.actif='1';
		c.commande="pas encore remplie";

		liste_bp=ajoute_ordre(c, liste_bp);

		//visualiser_liste(liste_bp);
		
	return CMD_OK_RETURN_VALUE;
}

int parse_and_execute_cmd_db() {
	if (visualiser_liste(liste_bp)==2) {
		fprintf(stdout, "il n'y a pas de breakpoints enregistrés\n");
	}
	return CMD_OK_RETURN_VALUE;
}

int parse_and_execute_cmd_er(char* paramsStr) {
	DEBUG_MSG("Parametres : %s", paramsStr);

	char* token;
	char* separateur = { " " };
	char* buffer;

	buffer = strdup ( paramsStr );

	//si pas de paramètres, on efface tous les breakpoints
	token = strtok(buffer, separateur);
	if (token == NULL) {
		DEBUG_MSG("effacement de tous les breakpoints");
		execute_cmd_er(0);
		return CMD_OK_RETURN_VALUE;
	}

	else {
		WORD A=(int)strtol(buffer, NULL, 0);
		int position;
		DEBUG_MSG("A = 0x%x", A);

		//vérifier qu'il est dans la liste
		position=recherche(liste_bp, A);
		DEBUG_MSG("position dans la liste %d", position);
		
		if (position == 0) {
			fprintf(stdout, "l'adresse 0x%x ne contient pas de point d'arrêt\n", A);
			return CMD_OK_RETURN_VALUE;
		}
		
		liste_bp=supprime(position, liste_bp);
		fprintf(stdout, "le point d'arrêt à l'adresse 0x%x a été retiré\n", A);
	}

	return CMD_OK_RETURN_VALUE;
}

int execute_cmd_er(int position) {

	if (position==0) {
		liste_bp=supprime_tout(liste_bp);
		fprintf(stdout, "tous les points d'arrêt ont été supprimés\n");
		return CMD_OK_RETURN_VALUE;
	}

	DEBUG_MSG("numero du bp à supprimer : %d", position);

	return CMD_OK_RETURN_VALUE;
}


