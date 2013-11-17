#include "fonctions.h"
#include "notify.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "reg.h"





int isregister(char* param) {
	DEBUG_MSG("entrée dans la fonction isregister");
	/*il y a deux types de noms de registres : 
	 * $numero
	 * $mnemonique
	*/
	if (param[0]!=36) {	//si ça ne commence pas par "$"
		if (param[0]==80 && param[1]==67) return 32;	//PC
		if (param[0]==83 && param[1]==82) return 33;	//SR
		if (param[0]==72 && param[1]==73) return 34;	//HI
		if (param[0]==76 && param[1]==79) return 35;	//LO
		return -1;
	}
	
	if (param[0]==36) {	//si ça commence par "$"
		//DEBUG_MSG("param commence par $");
		//regarder si c'est un numero ou un mnemo
		//les chiffres ont un code ascii entre 48 et 57
		char x[1];
		x[0]=param[1]; //on a récupéré le premier catactère
		//DEBUG_MSG("premier caractère %d", atoi(x));
		
		if (x[0]>47 && x[0]<58) {	//cas où c'est le numero du registre qui est appelé
			//DEBUG_MSG("cherche le numero");
			int nombre=0; 
			nombre=atoi(x); 
			//DEBUG_MSG("premier chiffre %d", nombre); 
			
			if (strlen(param)==2) {
				//DEBUG_MSG("un seul chiffre");
				return registres[nombre].numero;
			}
	
			char y[1];
			y[0]=param[2];
			//DEBUG_MSG("code ascii du deuxième chiffre = %d", y[0]);
			int nbe=0;
			nbe=atoi(y);
			//DEBUG_MSG("deuxième chiffre %d", nbe);
			int numero=10*nombre+nbe;
			//DEBUG_MSG("numéro du registre demandé %d", numero);
			//vérifier qu'il est dans le bon intervalle
			if (numero<0 || numero >35) {
				return -1;
			}
			else return numero;
		}
		
		else {		//cas où c'est le mnemo qui est appelé
			//DEBUG_MSG("cherche le nom");
			//DEBUG_MSG("%d %d", param[1], param[2]);
			char mot[2];
			mot[0]=param[1];
			mot[1]=param[2];
			mot[2]='\0';
			
			//strcat(mot, atoi(lettre2));
			//DEBUG_MSG("%s", mot);
			//int b=strcmp(mot,"az");
		
			//DEBUG_MSG("ici on a b= %d",b);
			
			//il faut parcourir le tableau des registres pour trouver le mnemo
			int i;
			for(i=0 ; i<36 ; i++) {
				
				//DEBUG_MSG("registre n° %d",i );
				if (strcmp(mot, registres[i].mnemo)==0)
				 return registres[i].numero;
						
			}
		}
	
	}
	return -1;
}
	

int isadress(char* param) {
	char* buffer;
	buffer=strdup(param);
    if (automate(buffer)==3 /*&& param<adresse_max*/)
    return 1;
    else return 0;
}

// Def des différents états
enum { INIT , DECIMAL_ZERO, DEBUT_HEXA, HEXA, DECIMAL , OCTAL} ;
// mise en oeuvre de l'automate
int automate(char* nombre ) {
int c ;         //caractère analyse courante
int S=INIT ;    // etat de l'automate
int i=0;

while (i<strlen(nombre))
{  
	c=nombre[i];
    switch (S) {
        case INIT :
                    if(isdigit(c)){ // si c'est un chiffre
                        if (c=='0')
                        S=DECIMAL_ZERO; else S= DECIMAL ;}
                    else if ( isspace(c)) S=INIT ;
                    else {//perror (  "erreur caracter (etape init)"  );
							return 0;}
        break ;

        case DECIMAL_ZERO: // reperage du prefixe de l ’ hexa
            if ( c == 'x' || c == 'X' ) S=DEBUT_HEXA;
            else if ( isdigit(c) && c<'8' ) S=OCTAL; // c ' est un octal
            else {//perror (  "erreur caracter (etape DeciZero)"  ); 
					return 2;}

        break ;

        case DEBUT_HEXA: // il faut au moins n chiffre après x
            if (isxdigit(c)) S=HEXA;
            else {//perror (  "erreur caracter (etape DebutHexa)"  ); 
				return 2;}
        break ;

        case HEXA: // tant que c'est un chiffre hexa
            if( isxdigit(c)) S=HEXA;
            else if ( c==EOF || isspace(c)) { S=INIT ;
            /*printf ( " la chaine est sous forme hexadecimale\n") ;*/
										 }
            else {//perror (  "erreur caracter (etape hexa)"  ); 
				return 2;}
        break ;

        case DECIMAL : // tant que c'est un chiffre
            if ( isdigit(c)) S=DECIMAL ;
            else if (isspace(c)) { S=INIT ;}
            /*printf ( " la chaine est sous forme decimale\n");*/                   
            else {//perror (  "erreur caracter (etape Decimal)"  ); 
				return 0;}
        break ;

        case OCTAL: // tant que c'est un chiffre
            if( isdigit(c)&& c<'8' ) S=OCTAL;
            else if (isspace(c)) { S=INIT ;
            /*printf ( " la chaine est sous forme octale \n" ) ;*/
                                                }
            else {//perror (  "erreur caracter (etape Octal)"  ); 
				return 0;}
        break ;
         }
     i++;
	}
//printf("S= %d\n",S);
return S;
}


//Definition des types d'adresse
enum {INI,FAUX,SIMPLE,NB_OCTETS,INTERVALLE};
//Mise en place du vérificateur
int adressType(char* param) {	
	if (param==NULL) return 1;
	
	char* token0=NULL;
	char* buffer0=NULL;
	
	buffer0=strdup(param);
	token0 = strtok(buffer0," ");
	if (token0==NULL) return 1;

	char* token=NULL;
	char* buffer=NULL;
	int compteur = 0;
	int nb_espaces = 0;
	int nb_tildes = 0;
	int nb_deuxpoints = 0;
	int S = INI;

	buffer=strdup(param);
	
	//Si l'entree est vide
	token = strtok(buffer," ");
	if (token==NULL) return 1;
	
	else
	{	//Déterminer le nombre d'espaces
		token = strtok(buffer," ");
			while (token!=NULL)
			{
				compteur++;
				nb_espaces=compteur-1;
				token=strtok(NULL," ");
			}
		compteur=0;
		buffer = strdup (param);

		//Déterminer le nombre de tildes
		token = strtok(buffer,"~");
			while (token!=NULL)
			{	compteur++;
				nb_tildes=compteur-1;
				token=strtok(NULL,"~");
			}
		compteur=0;
		buffer = strdup (param);

		//Déterminer le nombre de deuxpoints
		token = strtok(buffer,":");
			while (token!=NULL)
			{	compteur++;
				nb_deuxpoints=compteur-1;
				token=strtok(NULL,":");
			}
		compteur=0;
		buffer = strdup (param);
		
		//Désignation du type d'adresse
		if( (nb_espaces==0) && (nb_tildes==0) && (nb_deuxpoints==0))
		{
			buffer=strdup(param);
			if (isadress(buffer)==1) {/*printf("Adresse simple\n");*/ S=SIMPLE;}	
			else {S = FAUX; /*printf("Adresse incorrecte\n");*/}
		}

		else if( (nb_espaces==0) && (nb_tildes==1) && (nb_deuxpoints==0))
		{
			buffer=strdup(param);
			token = strtok(buffer,"~");
			if(isadress(token)!=1) {S = FAUX; /*printf("Premiere adresse incorrecte\n");*/}
			else{ 	char* token2 = NULL;
					token2 = strtok(NULL,"~");
					if(isadress(token2)!=1){S = FAUX; /*printf("Second adresse incorrecte\n");*/}
					else {S=INTERVALLE; /*printf("Adresse intervalle\n");*/}
				}
		}
		
		else if( (nb_espaces==0) && (nb_tildes==0) && (nb_deuxpoints==1))
		{
			buffer=strdup(param);
			token = strtok(buffer,":");
			if(isadress(token)!=1) {S = FAUX; /*printf("Adresse incorrecte\n");*/}
			else{ 	char* token2 = NULL;
					token2 = strtok(NULL,":");
					if(automate(token2)==4){S=NB_OCTETS; /*printf("Adresse et nb_octets\n");*/}
					else {S = FAUX; /*printf("Nb octets doit etre entier\n");*/}
				}
		}
		else {S = FAUX; /*printf("Syntaxe incorrecte\n");*/}
	//printf("\nparam = %s\nbuffer = %s\n\n",param,buffer);
	//printf("S = %d\n",S);
	return S;
	}
}	
	
void free_memory() {
	DEBUG_MSG("Entrée dans la fonction free_memory");
	int i;
	
	//vidage du dictionnaire d'instructions
	for (i=0 ; i<25 ; i++) {
		free(DICO[i].nom);
 		free(DICO[i].type);
		free(DICO[i].ops[0]);
 		free(DICO[i].ops[1]);
		free(DICO[i].ops[2]);
	}	

}

int desassemble(char* instr_hexa) {
	DEBUG_MSG("désassemblage de l'instruction %s", instr_hexa);

//	int i;

	//convertir la chaîne de caractères en binaire
	char instr_binaire[32];
	conv_hex_bin(instr_hexa, instr_binaire); //binaire est un tableau de 32 bits contenant tous les bits de l'instruction ; big endian

	//vérification
	fprintf(stdout, "traduction en binaire %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d\n", instr_binaire[0], instr_binaire[1], instr_binaire[2], instr_binaire[3], instr_binaire[4], instr_binaire[5], instr_binaire[6], instr_binaire[7], instr_binaire[8], instr_binaire[9], instr_binaire[10], instr_binaire[11], instr_binaire[12], instr_binaire[13], instr_binaire[14], instr_binaire[15], instr_binaire[16], instr_binaire[17], instr_binaire[18], instr_binaire[19], instr_binaire[20], instr_binaire[21], instr_binaire[22], instr_binaire[23], instr_binaire[24], instr_binaire[25], instr_binaire[26], instr_binaire[27], instr_binaire[28], instr_binaire[29], instr_binaire[30], instr_binaire[31]);

	//récupérer le numéro de l'instruction
	int num; //numero de l'instruction dans le dictionnaire
	num=recup_num(instr_binaire);
	if (num==0) {
		DEBUG_MSG("erreur lors de la recherche de l'intruction");
	}
	else {
		DEBUG_MSG("l'instruction porte le numéro %d", num);
	}

	//remplir la structure de l'instruction décodée
	INSTRUCTION decode;
	strcpy(decode.nom, DICO[num-1].nom);
	DEBUG_MSG("nom de l'instruction %s", decode.nom);


	strcpy(decode.type, DICO[num-1].type);
	DEBUG_MSG("type %s", decode.type);

	decode.nbe_op=DICO[num-1].nbe_op;
	DEBUG_MSG("opérandes %d", DICO[num-1].nbe_op);

	strcpy(decode.opcode, DICO[num-1].opcode);
	DEBUG_MSG("opcode dans le dico %s", DICO[num-1].opcode);
	DEBUG_MSG("func dans le dico %s", DICO[num-1].func);
	DEBUG_MSG("opcode dans decode %s", decode.opcode);
	DEBUG_MSG("longueur %zu", strlen(decode.opcode));
	DEBUG_MSG("%zu", strlen(DICO[num-1].opcode));


	
	return CMD_OK_RETURN_VALUE;
}

void remplit_inst(INSTRUCTION* decode,char instr_binaire,int num) {
	DEBUG_MSG("entrée dans la fonction remplit_inst");

	/*int i;
	for (i=0 ; i<strlen(DICO[num-1].nom) ; i++) {
		DEBUG_MSG("%d", i);
		DEBUG_MSG("%c", DICO[num-1].nom[i]);
		decode->nom[i]=DICO[num-1].nom[i];
	}
	DEBUG_MSG("%s", decode->nom);*/

	DEBUG_MSG("%s", decode->nom);
// MATT	decode->nom="bonjour";					//JE SAIS PAS CE QUE C'EST
	//strcpy(*decode->nom, "bonjour");
	DEBUG_MSG("ici");
	DEBUG_MSG("nom %s", decode->nom);

}

int conv_hex_bin(char* hexa, char bin[]) {
	DEBUG_MSG("conversion en binaire");

	//regarder les caractères de hexa un par un. Pour chacun :
		//convertir en pseudo-décimal : b=11, f=15, etc
		//convertir le pseudo-décimal en binaire

		//fonction codée à la barbare mais qui marche

	//DEBUG_MSG("chaine : %s", hexa);

	char chaine[8];
	strncpy(chaine, hexa, 8); //chaine[i] contient le code ascii du caractère correspondant de hexa
	int i;
	for (i=0 ; i<8 ; i++) {
		switch (chaine[i]) {
			case '0':
				chaine[i]=0;
				bin[4*i+0]=0;
				bin[4*i+1]=0;
				bin[4*i+2]=0;
				bin[4*i+3]=0;
				break;
			case '1':
				chaine[i]=1;
				bin[4*i+0]=0;
				bin[4*i+1]=0;
				bin[4*i+2]=0;
				bin[4*i+3]=1;
				break;
			case '2':
				chaine[i]=2;
				bin[4*i+0]=0;
				bin[4*i+1]=0;
				bin[4*i+2]=1;
				bin[4*i+3]=0;
				break;
			case '3':
				chaine[i]=3;
				bin[4*i+0]=0;
				bin[4*i+1]=0;
				bin[4*i+2]=1;
				bin[4*i+3]=1;
				break;
			case '4':
				chaine[i]=4;
				bin[4*i+0]=0;
				bin[4*i+1]=1;
				bin[4*i+2]=0;
				bin[4*i+3]=0;
				break;
			case '5':
				chaine[i]=5;
				bin[4*i+0]=0;
				bin[4*i+1]=1;
				bin[4*i+2]=0;
				bin[4*i+3]=1;
				break;
			case '6':
				chaine[i]=6;
				bin[4*i+0]=0;
				bin[4*i+1]=1;
				bin[4*i+2]=1;
				bin[4*i+3]=0;
				break;
			case '7':
				chaine[i]=7;
				bin[4*i+0]=0;
				bin[4*i+1]=1;
				bin[4*i+2]=1;
				bin[4*i+3]=1;
				break;
			case '8':
				chaine[i]=8;
				bin[4*i+0]=1;
				bin[4*i+1]=0;
				bin[4*i+2]=0;
				bin[4*i+3]=0;
				break;
			case '9':
				chaine[i]=9;
				bin[4*i+0]=1;
				bin[4*i+1]=0;
				bin[4*i+2]=0;
				bin[4*i+3]=1;
				break;
			case 'a':
				chaine[i]='a';
				bin[4*i+0]=1;
				bin[4*i+1]=0;
				bin[4*i+2]=1;
				bin[4*i+3]=0;
				break;
			case 'b':
				chaine[i]='b';
				bin[4*i+0]=1;
				bin[4*i+1]=0;
				bin[4*i+2]=1;
				bin[4*i+3]=1;
				break;
			case 'c':
				chaine[i]='c';
				bin[4*i+0]=1;
				bin[4*i+1]=1;
				bin[4*i+2]=0;
				bin[4*i+3]=0;
				break;
			case 'd':
				chaine[i]='d';
				bin[4*i+0]=1;
				bin[4*i+1]=1;
				bin[4*i+2]=0;
				bin[4*i+3]=1;
				break;
			case 'e':
				chaine[i]='e';
				bin[4*i+0]=1;
				bin[4*i+1]=1;
				bin[4*i+2]=1;
				bin[4*i+3]=0;
				break;
			case 'f':
				chaine[i]='f';
				bin[4*i+0]=1;
				bin[4*i+1]=1;
				bin[4*i+2]=1;
				bin[4*i+3]=1;
				break;
		}
		//DEBUG_MSG("caract %d  en decimal %d", chaine[i], p_decimal[i]);
	}
	
		
	return CMD_OK_RETURN_VALUE;
}

int conv_int_str(unsigned int value, char** string) {
	DEBUG_MSG("entrée dans la fonction conv_int_str");
	
	//char* chaine;
	DEBUG_MSG("%s", *string);
	sprintf(*string, "%x", value);
	DEBUG_MSG("là");
	DEBUG_MSG("conversion de la valeur %x en la chaîne %s", value, *string);

	
	return CMD_OK_RETURN_VALUE;
}

int recup_num(char instr_bin[]) {
	DEBUG_MSG("entrée dans la fonction recup_num");
	int num=0;
	int i;
	int j=0;	
	int trouve=0;

	//on récupère le opcode
	char *OPCODE=calloc(6, sizeof(*OPCODE));
	//verifie si calloc est OK
	for (i=0 ; i<6 ; i++) {
		OPCODE[i]=instr_bin[i];
	}

	DEBUG_MSG("opcode de l'instruction %d%d%d%d%d%d", OPCODE[0], OPCODE[1], OPCODE[2], OPCODE[3], OPCODE[4], OPCODE[5]);
	//transformation du tableau d'entiers en un tableau de caractères pour povoir utiliser strcmp
	int l=0;
	while (l<6) {
		if (OPCODE[l]==1) {
			OPCODE[l]=49;
			l++;
		}
		else if (OPCODE[l]==0) {
			OPCODE[l]=48;
			l++;
		}
	
	}
	//DEBUG_MSG("opcode %s", OPCODE);

	//DEBUG_MSG("%d", strcmp(OPCODE,"001000\0"));

	/*if (trouve==0 && j<taille_DICO) {
		DEBUG_MSG("entrée dans la boucle while");
	}*/
	
	while (trouve==0 && j<taille_DICO) {	//tant qu'on n'a pas trouvé de quelle instruction il s'agit
		//DEBUG_MSG("opcode du dico %s", DICO[j].opcode);

		if(strcmp(OPCODE,"001000")==0) { //ADDI
			num=2;
			trouve=1;
		}

		if(strcmp(OPCODE, "100011")==0) { //LW
			num=13;
			trouve=1;
		}	

		if(strcmp(OPCODE, "101011")==0) { //SW
			num=14;
			trouve=1;
		}

		if(strcmp(OPCODE, "001111")==0) { //LUI
			num=15;
			trouve=1;
		}

		if (strcmp(OPCODE, "000100")==0) { //BEQ
			num=18;
			trouve=1;
		}

		if(strcmp(OPCODE, "000101")==0) { //BNE
			num=19;
			trouve=1;
		}

		if (strcmp(OPCODE, "000111")==0) { //BGTZ
			num=20;
			trouve=1;
		}
		
		if (strcmp(OPCODE, "000110")==0) { //BLEZ
			num=21;
			trouve=1;
		}

		if (strcmp(OPCODE, "000010")==0) { //J
			num=22;
			trouve=1;
		}

		if (strcmp(OPCODE, "000011")==0) { //JAL
			num=23;
			trouve=1;
		}

		if(strcmp("000000", OPCODE)==0) {
			//DEBUG_MSG("15 instructions peuvent correspondre");
			//on récupère le code func
			char *FUNC=calloc(6, sizeof(*FUNC));
			//verifie si calloc est OK
			for (i=0 ; i<6 ; i++) {
				FUNC[i]=instr_bin[26+i];
			}

			DEBUG_MSG("code func de l'instruction %d%d%d%d%d%d", FUNC[0], FUNC[1], FUNC[2], FUNC[3], FUNC[4], FUNC[5]);

			//transformation du tableau d'entiers en un tableau de caractères pour povoir utiliser strcmp
			l=0;
			while (l<6) {
				if (FUNC[l]==1) {
					FUNC[l]=49;
					l++;
				}
				else if (FUNC[l]==0) {
					FUNC[l]=48;
					l++;
				}
	
			}
			//DEBUG_MSG("func %s", FUNC);

			//DEBUG_MSG("comparaison %d", strcmp("001100", FUNC));
			//DEBUG_MSG("recomparaison %d", strcmp("100000", FUNC));

			
			if (strcmp("100000", FUNC)==0) { //ADD
				//DEBUG_MSG("on est là");
				num=1;
				trouve=1;
			}

			if (strcmp("100010", FUNC)==0) { //SUB
				num=3;
				trouve=1;
			}

			if (strcmp("011000", FUNC)==0) { //MULT
				num=4;
				trouve=1;
			}

			if (strcmp("011010", FUNC)==0) { //DIV
				num=5;
				trouve=1;
			}

			if (strcmp("100100", FUNC)==0) { //AND
				num=6;
				trouve=1;
			}

			if (strcmp("100101", FUNC)==0) { //OR
				num=7;
				trouve=1;
			}

			if (strcmp("100110", FUNC)==0) { //XOR
				num=8;
				trouve=1;
			}

			if (strcmp("101010", FUNC)==0) { //SLT
				num=12;
				trouve=1;
			}

			if (strcmp("010000", FUNC)==0) { //MFHI
				num=16;
				trouve=1;
			}

			if (strcmp("010010", FUNC)==0) { //MFLO
				num=17;
				trouve=1;
			}

			if (strcmp("001000", FUNC)==0) { //JR
				num=24;
				trouve=1;
			}

			if (strcmp("000010", FUNC)==0) {
				if (instr_bin[11]==0) { //SRL
					num=11;
					trouve=1;
				}
				if (instr_bin[11]==0) { //ROTR
					num=9;
					trouve=1;
				}
			}

			if (strcmp("001100", FUNC)==0) { //SYSCALL
				num=26;
				trouve=1;
			}

			if (strcmp("000000", FUNC)==0) {
				l=0;
				int s=0;
				while (l==0 && s<32) {
					if (instr_bin[s]!=0) {
						l=1;
					}
					s++;
				}
				if (l==0) { //l'instruction NOP n'a que des 0
					num=25;
					trouve=1;
				}
				if (l==1) { //SLL
					num=10;
					trouve=1;
				}
			}
			else {
				j=taille_DICO+45;
			}
			

		//Et là on a étudié tous les cas possibles
						
		}
		
		j++;
	}
	
	if (trouve==0) {
		DEBUG_MSG("l'instruction n'est pas dans le dictionnaire");
		num=0;
	}
	
	return num;
}


