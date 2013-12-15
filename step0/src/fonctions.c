#include "fonctions.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>  // exit
#include <ctype.h>
#include "notify.h"
#include "parseExecute.h"
#include "reg.h"

#include <math.h>


int isnull(char* pointer)
{	if (pointer==NULL) return 1;
	else return 0;
}
int isadtype1(char* pointer)
{ 	if(adressType(pointer)==1) return 1;
	else return 0;
}


/* @param adresse adresse à afficher 
 * @brief affiche le contenu de l'adresse
 */
void display_one(int adresse)
{	display_poly(adresse,1);
}


/* @param adresse adresse a partir de laquelle on affiche
 * @param nombre nombre d'octets à afficher
 * @brief affiche le contenu des "nombre" octets à partir de "adresse", 
 * @brief en parcourant toutes les sections.
 */
void display_poly(int adresse, int nombre)
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
}


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
	

int isadress(char* param)
{	char* buffer;
	buffer=strdup(param);
	int a = (int)strtol(param,NULL,0);
    if (automate(buffer)==3 && a<=0xffffffff)
    return 1;
    else return 0;
}

int isaddressbusy(char* param)
{	if (isadress(param)==1)
	{	int m=(int)strtol(param,NULL,0);
	
		if ( 		( (m >= textSection->startAddress + textSection->size) 
					&& (m < dataSection->startAddress) )
					|| ( (m >= dataSection->startAddress + dataSection->size) 
					&& (m < bssSection->startAddress) )
					|| ( (m >= bssSection->startAddress + bssSection->size) 
					)
			)
		{	WARNING_MSG("Adresse inoccupée");
			return 0;
		}
		else return 1;
	}
	else WARNING_MSG("Adresse incorrecte");
	return 0;
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
            //printf ( " la chaine est sous forme decimale\n");                 
            else {//perror (  "erreur caracter (etape Decimal)"  ); 
				return 0;}
        break ;

        case OCTAL: // tant que c'est un chiffre
            if( isdigit(c)&& c<'8' ) S=OCTAL;
            else if (isspace(c)) { S=INIT ;
            //printf ( " la chaine est sous forme octale \n" ) ;*/
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


int get_opcode(unsigned int mot){

	return (mot &0xFC000000 )>>26; 
}

int get_rs(unsigned int mot){
	return (mot & 0x03C00000) >> 21;
	
}

int get_rd(unsigned int mot){
	return (mot & 0x0000F800) >> 11;
}

int get_rt(unsigned int mot){
	return (mot & 0x000F8000) >> 16;
}

int get_sa(unsigned int mot){
	return (mot & 0x000007C0) >> 6;
}

int get_function(unsigned int mot){

	return (mot & 0x0000003F);
}

int get_imm(unsigned int mot){
	
	return (short)(mot & 0x0000FFFF); 
}

int get_target(unsigned int mot){
	return (mot & 0x03FFFFFF);
}

int get_memory(int adresse)
{	int val=0;
	char chaine[10];
	int i;

	if ( (adresse <textSection->startAddress + textSection->size) 
		&& (adresse >=textSection->startAddress) )  
		{	for (i=0 ; i<4 ; i++) {
				sprintf(chaine+i*2, "%02x", textSection->data[adresse+i]);
			}	
		}
		
	if ( (adresse <dataSection->startAddress + dataSection->size) 
		&& (adresse >=dataSection->startAddress) )  
		{	for (i=0 ; i<4 ; i++) {
				sprintf(chaine+i*2, "%02x", dataSection->data[adresse-dataSection->startAddress+i]);
			}	
		}
		
	if ( (adresse <bssSection->startAddress + bssSection->size) 
		&& (adresse >=bssSection->startAddress) )  
		{	for (i=0 ; i<4 ; i++) {
				sprintf(chaine+i*2, "%02x", textSection->data[adresse-bssSection->startAddress+i]);
			}	
		}
	sscanf(chaine,"%x",&val);
	//fprintf(stdout,"chaine = %s\n",chaine);
	//fprintf(stdout,"val = %x\n",val);
	return val;
}


INSTRUCTION desassemble(char* instr_hexa) {
	//DEBUG_MSG("désassemblage de l'instruction %s", instr_hexa);
	INSTRUCTION error;
	strcpy(error.nom, "ERREUR");
	//convertir la chaîne de caractères en binaire
	char instr_binaire[32];
	unsigned int mot =0;
	sscanf(instr_hexa,"%x",&mot);
	conv_hex_bin(instr_hexa, instr_binaire); //binaire est un tableau de 32 bits contenant tous les bits de l'instruction ; big endian

	//vérification
	//INFO_MSG("traduction en binaire %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d\n", instr_binaire[0], instr_binaire[1], instr_binaire[2], instr_binaire[3], instr_binaire[4], instr_binaire[5], instr_binaire[6], instr_binaire[7], instr_binaire[8], instr_binaire[9], instr_binaire[10], instr_binaire[11], instr_binaire[12], instr_binaire[13], instr_binaire[14], instr_binaire[15], instr_binaire[16], instr_binaire[17], instr_binaire[18], instr_binaire[19], instr_binaire[20], instr_binaire[21], instr_binaire[22], instr_binaire[23], instr_binaire[24], instr_binaire[25], instr_binaire[26], instr_binaire[27], instr_binaire[28], instr_binaire[29], instr_binaire[30], instr_binaire[31]);

	//récupérer le numéro de l'instruction
	int num; //numero de l'instruction dans le dictionnaire
	num=recup_num(instr_binaire);
	if (num==0) {
		DEBUG_MSG("erreur lors de la recherche de l'intruction");
		return error  ;
	}
	else {
		//DEBUG_MSG("l'instruction porte le numéro %d", num);
	}

	//remplir la structure de l'instruction décodée
	INSTRUCTION decode;
	strcpy(decode.nom, DICO[num-1].nom);
	//DEBUG_MSG("nom de l'instruction %s", decode.nom);


	strcpy(decode.type, DICO[num-1].type);
	//DEBUG_MSG("type %s", decode.type);

	decode.nbe_op=DICO[num-1].nbe_op;
	//DEBUG_MSG("opérandes %d", DICO[num-1].nbe_op);

	strcpy(decode.opcode, DICO[num-1].opcode);
	//DEBUG_MSG("opcode %s", decode.opcode);

	strcpy(decode.func, DICO[num-1].func);
	//DEBUG_MSG("func %s", decode.func);

	//récupérer les opérandes

	//initialisation du tableau d'opérandes : s'il y a moins de trois opérandes il ne faut pas que ops[2]
	//par exemple contienne des trucs
	int i;
	for (i=0 ; i<4 ; i++) {
		strcpy(decode.ops[i], "init");
		//DEBUG_MSG("%s", decode.ops[i]);
	}
	
	char op0[26];
	char op1[16];
	char op2[16];
	int j;
	
	if (decode.nbe_op==0) {
	}

	if (decode.nbe_op==1) {
		//DEBUG_MSG("1 operande");
		if (strcmp(DICO[num-1].ops[0], "$rd")==0) {
			//DEBUG_MSG("\nopérande : rd");
			//récupérer l'opérande dans instr_bin :
			for (j=0 ; j<5 ; j++) { //$rd est codé sur 5bits
				op0[j]=instr_binaire[j+16];
				//DEBUG_MSG("op0[%d] %d", j, op0[j]);
				if(op0[j]==1) {op0[j]=49;}
				if(op0[j]==0) {op0[j]=48;}
			}
			
			//enlever les bits de trop à la fin:
			op0[5]='\0';
			
			strcpy(decode.ops[0], op0);
			//DEBUG_MSG("decode.ops[0] %s %d\n", decode.ops[0], (int)strlen(decode.ops[0]));
			//c'est bon !
		}

		if (strcmp(DICO[num-1].ops[0], "$rs")==0) {
		//	DEBUG_MSG("\nopérande : rs");
			//récupérer l'opérande dans instr_bin :
			for (j=0 ; j<5 ; j++) { //$rs est codé sur 5bits
				op0[j]=instr_binaire[j+6]; //$rs commence juste après les 6bits du opcode
				//DEBUG_MSG("op0[%d] %d", j, op0[j]);
				if(op0[j]==1) {op0[j]=49;}
				if(op0[j]==0) {op0[j]=48;}
			}
			
			//enlever les bits de trop à la fin:
			op0[5]='\0';
			
			strcpy(decode.ops[0], op0);
		//	DEBUG_MSG("decode.ops[0] %s %d\n", decode.ops[0], (int)strlen(decode.ops[0]));
			//c'est codé pareil que les autres cas donc c'est bon
		}

		if (strcmp(DICO[num-1].ops[0], "target")==0) {
		//	DEBUG_MSG("\nopérande : target");
			//récupérer l'opérande dans instr_binaire
			for (j=0 ; j<26 ; j++) {
				op0[j]=instr_binaire[j+6]; //l'opérande commence juste après les 6bits du opcode
				//DEBUG_MSG("op0[%d] %d", j, op0[j]);
				if(op0[j]==1) {op0[j]=49;}
				if(op0[j]==0) {op0[j]=48;}
			}
			
			//enlever les bits de trop à la fin :
			op0[26]='\0';
			
			strcpy(decode.ops[0], op0);
		//	DEBUG_MSG("decode.ops[0] %s %d\n", decode.ops[0], (int)strlen(decode.ops[0]));
			//et maintenant on a pile ce qu'il faut =)
		}
		
	}

	if (decode.nbe_op==2) { //on fait grosso modo la même chose que dans le cas nbe=1
		//DEBUG_MSG("2 opérandes");
		
		//deux cas possibles pour ops[0] : $rs et $rt
		
		if (strcmp(DICO[num-1].ops[0], "$rs")==0) { //cas ops[0]=$rs
			//DEBUG_MSG("\n1e opérande : rs");
			//récupérer l'opérande dans instr_bin :
			for (j=0 ; j<5 ; j++) { //$rs est codé sur 5bits
				op0[j]=instr_binaire[j+6]; //$rs commence juste après les 6bits du opcode
				//DEBUG_MSG("op0[%d] %d", j, op0[j]);
				if(op0[j]==1) {op0[j]=49;}
				if(op0[j]==0) {op0[j]=48;}
			}
			
			//enlever les bits de trop à la fin:
			op0[5]='\0';
			
			strcpy(decode.ops[0], op0);
			//DEBUG_MSG("decode.ops[0] %s %d\n", decode.ops[0], (int)strlen(decode.ops[0]));
			//c'est codé pareil que les autres cas donc c'est bon
		}
			
		if (strcmp(DICO[num-1].ops[0], "$rt")==0) { //cas ops[0]=$rt
			//DEBUG_MSG("\n1e opérande : rt");
			//récupérer l'opérande dans instr_bin :
			for (j=0 ; j<5 ; j++) { //$rt est codé sur 5bits
				op0[j]=instr_binaire[j+11];
				//DEBUG_MSG("op0[%d] %d", j, op0[j]);
				if(op0[j]==1) {op0[j]=49;}
				if(op0[j]==0) {op0[j]=48;}
			}
			
			//enlever les bits de trop à la fin:
			op0[5]='\0';
			
			strcpy(decode.ops[0], op0);
			//DEBUG_MSG("decode.ops[0] %s %d\n", decode.ops[0], (int)strlen(decode.ops[0]));
		}

		//trois cas possibles pour ops[1] : $rt, imm et offset

		if (strcmp(DICO[num-1].ops[1], "$rt")==0) { //cas ops[1]=$rt
			//DEBUG_MSG("\n2e opérande : rt");
			//récupérer l'opérande dans instr_bin :
			for (j=0 ; j<5 ; j++) { //$rt est codé sur 5bits
				op1[j]=instr_binaire[j+11];
				//DEBUG_MSG("op1[%d] %d", j, op1[j]);
				if(op1[j]==1) {op1[j]=49;}
				if(op1[j]==0) {op1[j]=48;}
			}
			
			//enlever les bits de trop à la fin:
			op1[5]='\0';
			
			strcpy(decode.ops[1], op1);
			//DEBUG_MSG("decode.ops[1] %s %d\n", decode.ops[1], (int)strlen(decode.ops[1]));
		}

		if (strcmp(DICO[num-1].ops[1], "imm")==0) { //cas ops[1]=imm
			//DEBUG_MSG("\n2e opérande : immediate");
			//récupérer l'opérande dans instr_bin :
			for (j=0 ; j<16 ; j++) { //immediate est codé sur 16bits
				op1[j]=instr_binaire[j+16];
				//DEBUG_MSG("op1[%d] %d", j, op1[j]);
				if(op1[j]==1) {op1[j]=49;}
				if(op1[j]==0) {op1[j]=48;}
			}
			
			//enlever les bits de trop à la fin:
			op1[16]='\0';
			
			strcpy(decode.ops[1], op1);
			//DEBUG_MSG("decode.ops[1] %s %d\n", decode.ops[1], (int)strlen(decode.ops[1]));
		}

		if (strcmp(DICO[num-1].ops[1], "offset")==0) { //cas ops[1]=offset
			//DEBUG_MSG("\n2e opérande : offset");
			//récupérer l'opérande dans instr_bin :
			for (j=0 ; j<16 ; j++) { //offset est codé sur 16bits
				op1[j]=instr_binaire[j+16];
				//DEBUG_MSG("op1[%d] %d", j, op1[j]);
				if(op1[j]==1) {op1[j]=49;}
				if(op1[j]==0) {op1[j]=48;}
			}
			
			//enlever les bits de trop à la fin:
			op1[16]='\0';
			
			strcpy(decode.ops[1], op1);
			//DEBUG_MSG("decode.ops[1] %s %d\n", decode.ops[1], (int)strlen(decode.ops[1]));
		}

	}

	if (decode.nbe_op==3) { //et c'est reparti pour un tour
		//DEBUG_MSG("3 opérandes");

		//trois cas possibles pour ops[0] : rd rs rt
		if (strcmp(DICO[num-1].ops[0], "$rd")==0) { //cas 1e op = $rd
		//	DEBUG_MSG("\n1e opérande : rd");
			//récupérer l'opérande dans instr_bin :
			for (j=0 ; j<5 ; j++) { //$rd est codé sur 5bits
				op0[j]=instr_binaire[j+16];
				//DEBUG_MSG("op0[%d] %d", j, op0[j]);
				if(op0[j]==1) {op0[j]=49;}
				if(op0[j]==0) {op0[j]=48;}
			}
			
			//enlever les bits de trop à la fin:
			op0[5]='\0';
			
			strcpy(decode.ops[0], op0);
		//	DEBUG_MSG("decode.ops[0] %s %d\n", decode.ops[0], (int)strlen(decode.ops[0]));
			//c'est bon !
		}

		if (strcmp(DICO[num-1].ops[0], "$rs")==0) { //cas 1e op =$rs
		//	DEBUG_MSG("\n1e opérande : rs");
			//récupérer l'opérande dans instr_bin :
			for (j=0 ; j<5 ; j++) { //$rs est codé sur 5bits
				op0[j]=instr_binaire[j+6]; //$rs commence juste après les 6bits du opcode
				//DEBUG_MSG("op0[%d] %d", j, op0[j]);
				if(op0[j]==1) {op0[j]=49;}
				if(op0[j]==0) {op0[j]=48;}
			}
			
			//enlever les bits de trop à la fin:
			op0[5]='\0';
			
			strcpy(decode.ops[0], op0);
		//	DEBUG_MSG("decode.ops[0] %s %d\n", decode.ops[0], (int)strlen(decode.ops[0]));
			//c'est codé pareil que les autres cas donc c'est bon
		}

		if (strcmp(DICO[num-1].ops[0], "$rt")==0) { //cas ops[0]=$rt
		//	DEBUG_MSG("\n1e opérande : rt");
			//récupérer l'opérande dans instr_bin :
			for (j=0 ; j<5 ; j++) { //$rt est codé sur 5bits
				op0[j]=instr_binaire[j+11];
				//DEBUG_MSG("op0[%d] %d", j, op0[j]);
				if(op0[j]==1) {op0[j]=49;}
				if(op0[j]==0) {op0[j]=48;}
			}
			
			//enlever les bits de trop à la fin:
			op0[5]='\0';
			
			strcpy(decode.ops[0], op0);
		//	DEBUG_MSG("decode.ops[0] %s %d\n", decode.ops[0], (int)strlen(decode.ops[0]));
		}

		//trois cas possibles pour ops[1] : rs rt offset
		if (strcmp(DICO[num-1].ops[1], "$rs")==0) { //cas 2e op =$rs
		//	DEBUG_MSG("\n2e opérande : rs");
			//récupérer l'opérande dans instr_bin :
			for (j=0 ; j<5 ; j++) { //$rs est codé sur 5bits
				op1[j]=instr_binaire[j+6]; //$rs commence juste après les 6bits du opcode
				//DEBUG_MSG("op1[%d] %d", j, op1[j]);
				if(op1[j]==1) {op1[j]=49;}
				if(op1[j]==0) {op1[j]=48;}
			}
			
			//enlever les bits de trop à la fin:
			op1[5]='\0';
			
			strcpy(decode.ops[1], op1);
		//	DEBUG_MSG("decode.ops[1] %s %d\n", decode.ops[1], (int)strlen(decode.ops[1]));
			//c'est codé pareil que les autres cas donc c'est bon
		}
		
		if (strcmp(DICO[num-1].ops[1], "$rt")==0) { //cas ops[1]=$rt
		//	DEBUG_MSG("\n2e opérande : rt");
			//récupérer l'opérande dans instr_bin :
			for (j=0 ; j<5 ; j++) { //$rt est codé sur 5bits
				op1[j]=instr_binaire[j+11];
				//DEBUG_MSG("op1[%d] %d", j, op1[j]);
				if(op1[j]==1) {op1[j]=49;}
				if(op1[j]==0) {op1[j]=48;}
			}
			
			//enlever les bits de trop à la fin:
			op1[5]='\0';
			
			strcpy(decode.ops[1], op1);
		//	DEBUG_MSG("decode.ops[1] %s %d\n", decode.ops[1], (int)strlen(decode.ops[1]));
		}

		if (strcmp(DICO[num-1].ops[1], "offset")==0) { //cas ops[1]=offset
		//	DEBUG_MSG("\n2e opérande : offset");
			//récupérer l'opérande dans instr_bin :
			for (j=0 ; j<16 ; j++) { //offset est codé sur 16bits
				op1[j]=instr_binaire[j+16];
				//DEBUG_MSG("op1[%d] %d", j, op1[j]);
				if(op1[j]==1) {op1[j]=49;}
				if(op1[j]==0) {op1[j]=48;}
			}
			
			//enlever les bits de trop à la fin:
			op1[16]='\0';
			
			strcpy(decode.ops[1], op1);
		//	DEBUG_MSG("decode.ops[1] %s %d\n", decode.ops[1], (int)strlen(decode.ops[1]));
		}

		//cinq cas possibles pour ops[2] : rt imm sa rs offset

		if (strcmp(DICO[num-1].ops[2], "offset")==0) { //cas ops[2]=offset
		//	DEBUG_MSG("\n3e opérande : offset");
			//récupérer l'opérande dans instr_bin :
			for (j=0 ; j<16 ; j++) { //offset est codé sur 16bits
				op2[j]=instr_binaire[j+16];
				//DEBUG_MSG("op2[%d] %d", j, op2[j]);
				if(op2[j]==1) {op2[j]=49;}
				if(op2[j]==0) {op2[j]=48;}
			}
			
			//enlever les bits de trop à la fin:
			op2[16]='\0';
			
			strcpy(decode.ops[2], op2);
		//	DEBUG_MSG("decode.ops[2] %s %d\n", decode.ops[2], (int)strlen(decode.ops[2]));
		}

		if (strcmp(DICO[num-1].ops[2], "$rs")==0) { //cas 3e op =$rs
		//	DEBUG_MSG("\n3e opérande : rs");
			//récupérer l'opérande dans instr_bin :
			for (j=0 ; j<5 ; j++) { //$rs est codé sur 5bits
				op2[j]=instr_binaire[j+6]; //$rs commence juste après les 6bits du opcode
				//DEBUG_MSG("op2[%d] %d", j, op2[j]);
				if(op2[j]==1) {op2[j]=49;}
				if(op2[j]==0) {op2[j]=48;}
			}
			
			//enlever les bits de trop à la fin:
			op2[5]='\0';
			
			strcpy(decode.ops[2], op2);
		//	DEBUG_MSG("decode.ops[2] %s %d\n", decode.ops[2], (int)strlen(decode.ops[2]));
			//c'est codé pareil que les autres cas donc c'est bon
		}
		
		if (strcmp(DICO[num-1].ops[2], "$rt")==0) { //cas ops[2]=$rt
		//	DEBUG_MSG("\n3e opérande : rt");
			//récupérer l'opérande dans instr_bin :
			for (j=0 ; j<5 ; j++) { //$rt est codé sur 5bits
				op2[j]=instr_binaire[j+11];
				//DEBUG_MSG("op1[%d] %d", j, op1[j]);
				if(op2[j]==1) {op2[j]=49;}
				if(op2[j]==0) {op2[j]=48;}
			}
			
			//enlever les bits de trop à la fin:
			op2[5]='\0';
			
			strcpy(decode.ops[2], op2);
		//	DEBUG_MSG("decode.ops[2] %s %d\n", decode.ops[2], (int)strlen(decode.ops[2]));
		}

		if (strcmp(DICO[num-1].ops[2], "imm")==0) { //cas ops[2]=imm
		//	DEBUG_MSG("\n3e opérande : immediate");
			//récupérer l'opérande dans instr_bin :
			for (j=0 ; j<16 ; j++) { //immediate est codé sur 16bits
				op2[j]=instr_binaire[j+16];
				//DEBUG_MSG("op2[%d] %d", j, op2[j]);
				if(op2[j]==1) {op2[j]=49;}
				if(op2[j]==0) {op2[j]=48;}
			}
			
			//enlever les bits de trop à la fin:
			op2[16]='\0';
			
			strcpy(decode.ops[2], op2);
		//	DEBUG_MSG("decode.ops[2] %s %d\n", decode.ops[2], (int)strlen(decode.ops[2]));
		}

		if (strcmp(DICO[num-1].ops[2], "$sa")==0) { //cas 3e op =$sa
		//	DEBUG_MSG("\n3e opérande : sa");
			//récupérer l'opérande dans instr_bin :
			for (j=0 ; j<5 ; j++) { //$sa est codé sur 5bits
				op2[j]=instr_binaire[j+21];
				//DEBUG_MSG("op2[%d] %d", j, op2[j]);
				if(op2[j]==1) {op2[j]=49;}
				if(op2[j]==0) {op2[j]=48;}
			}
			
			//enlever les bits de trop à la fin:
			op2[5]='\0';
			
			strcpy(decode.ops[2], op2);
		//	DEBUG_MSG("decode.ops[2] %s %d\n", decode.ops[2], (int)strlen(decode.ops[2]));
		


		}

	}

	return decode;
}

int conv_hex_bin(char* hexa, char bin[]) {
	//DEBUG_MSG("conversion en binaire");

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
	//DEBUG_MSG("entrée dans la fonction conv_int_str");
	
	//char* chaine;
	//DEBUG_MSG("%s", *string);
	sprintf(*string, "%x", value);
	//DEBUG_MSG("là");
	//DEBUG_MSG("conversion de la valeur %x en la chaîne %s", value, *string);

	
	return CMD_OK_RETURN_VALUE;
}

int recup_num(char instr_bin[]) {
	//DEBUG_MSG("entrée dans la fonction recup_num");
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

	//DEBUG_MSG("opcode de l'instruction %d%d%d%d%d%d", OPCODE[0], OPCODE[1], OPCODE[2], OPCODE[3], OPCODE[4], OPCODE[5]);
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

		//	DEBUG_MSG("code func de l'instruction %d%d%d%d%d%d", FUNC[0], FUNC[1], FUNC[2], FUNC[3], FUNC[4], FUNC[5]);

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
		WARNING_MSG("l'instruction n'est pas dans le dictionnaire");
		num=0;
	}
	
	return num;
}

void affiche_inst(INSTRUCTION i) {
	//DEBUG_MSG("entrée dans la fonction affiche_inst");

	if (i.nbe_op==3) {
		fprintf(stdout, "nom: %s nbe d'op: %d 1è op: %s 2è op: %s 3è op: %s opcode: %s function: %s\n",
		i.nom, i.nbe_op, i.ops[0], i.ops[1], i.ops[2], i.opcode, i.func);
	}

	if (i.nbe_op==2) {
		fprintf(stdout, "nom: %s nbe d'op: %d 1è op: %s 2è op: %s opcode: %s function: %s\n",
		i.nom, i.nbe_op, i.ops[0], i.ops[1], i.opcode, i.func);
	}

	if (i.nbe_op==1) {
		fprintf(stdout, "nom: %s nbe d'op: %d 1è op: %s opcode: %s function: %s\n",
		i.nom, i.nbe_op, i.ops[0], i.opcode, i.func);
	}

	if (i.nbe_op==0) {
		fprintf(stdout, "nom: %s nbe d'op: %d opcode: %s function: %s\n",
		i.nom, i.nbe_op, i.opcode, i.func);
	}
}


void affiche_inst_brut(INSTRUCTION inst, int adresse) {  // !!!!! ATTENTION à la valeur de PC !
	//DEBUG_MSG("entrée dans la fonction affiche_inst");
	
		//if (inst.nbe_op==3) {
		//fprintf(stdout, "%s %s %s %s %s %s\n",
		//inst.nom, inst.ops[0], inst.ops[1], inst.ops[2], inst.opcode, inst.func);	
		//	}
/*
	if (inst.nbe_op==2) {
		fprintf(stdout, "%s %d %s %s %s %s\n",
		inst.nom, inst.nbe_op, inst.ops[0], inst.ops[1], inst.opcode, inst.func);
	}

	if (inst.nbe_op==1) {
		fprintf(stdout, "%s %d %s %s %s\n",
		inst.nom, inst.nbe_op, inst.ops[0], inst.opcode, inst.func);
	}

	if (inst.nbe_op==0) {
		fprintf(stdout, "%s %d %s %s\n",
		inst.nom, inst.nbe_op, inst.opcode, inst.func);
	}
*/

	//INSTRUCTIONS TYPE R
	//R : rd, rs, rt
	if ((strcmp(inst.nom,"ADD")==0)||
		(strcmp(inst.nom,"SUB")==0)||
		(strcmp(inst.nom,"AND")==0)||
		(strcmp(inst.nom,"OR") ==0)||
		(strcmp(inst.nom,"XOR")==0)||
		(strcmp(inst.nom,"SLT")==0)
		)
	{	fprintf(stdout, "%s $%d $%d $%d\n",
			inst.nom,
			(get_rd(get_memory(adresse))), 
			(get_rs(get_memory(adresse))),
			(get_rt(get_memory(adresse)))
			);
	}
	
	//R : rs, rt, imm
	if (strcmp(inst.nom,"ADDI")==0)
	{	fprintf(stdout, "%s $%d $%d %d\n",
			inst.nom,
			(get_rt(get_memory(adresse))), 
			(get_rs(get_memory(adresse))),
			(get_imm(get_memory(adresse)))
			);
	}
	
	//R : rs, rt
	if ((strcmp(inst.nom,"MULT")==0)||
		(strcmp(inst.nom,"DIV")==0) 
		)
	{	fprintf(stdout, "%s $%d $%d\n",
			inst.nom,
			(get_rs(get_memory(adresse))), 
			(get_rt(get_memory(adresse)))
			);
	}
	
	//R : rd, rt, sa
	if ((strcmp(inst.nom,"ROTR")==0)||
		(strcmp(inst.nom,"SLL")==0) ||
		(strcmp(inst.nom,"SRL")==0) 
		)
	{ 	fprintf(stdout, "%s $%d $%d %d\n",
			inst.nom,
			(get_rd(get_memory(adresse))), 
			(get_rt(get_memory(adresse))),
			(get_sa(get_memory(adresse)))
			);
	}
	
	//R : rd
	if ((strcmp(inst.nom,"MFHI")==0)||
		(strcmp(inst.nom,"MFLO")==0)
		)
	{	fprintf(stdout, "%s $%d\n", inst.nom, (get_rd(get_memory(adresse))) );
	}
	
	//R : 0
	if ((strcmp(inst.nom,"SYSCALL")==0)||
		(strcmp(inst.nom,"NOP")==0)
		)
	{	fprintf(stdout, "%s\n", inst.nom);
	}
	
	//INSTRUCTIONS TYPE I
	//I : rt, base , offset   | offset obtenu par get_imm
	if ((strcmp(inst.nom,"LW")==0)||
		(strcmp(inst.nom,"SW")==0)
		)
	{	fprintf(stdout, "%s $%d, %d(%d)\n",
			inst.nom,
			(get_rt(get_memory(adresse))), 
			(get_imm(get_memory(adresse))),
			(get_rs(get_memory(adresse)))
			);
	}
	
	//I : rt, imm
	if (strcmp(inst.nom,"LUI")==0)
	{	fprintf(stdout, "%s $%d %d\n",
			inst.nom,
			(get_rt(get_memory(adresse))), 
			(get_imm(get_memory(adresse)))
			);
	}

	//I : rs, rt, offset
	if ((strcmp(inst.nom,"BEQ")==0)||
		(strcmp(inst.nom,"BEQ")==0)
		)
	{	fprintf(stdout, "%s $%d $%d %d\n",
			inst.nom,
			(get_rs(get_memory(adresse))), 
			(get_rt(get_memory(adresse))),
			(get_imm(get_memory(adresse)))
			);
	}
	
	//I : rs, offset
	if ((strcmp(inst.nom,"BQTZ")==0)||
		(strcmp(inst.nom,"BLEZ")==0)
		)
	{	fprintf(stdout, "%s $%d %d\n",
			inst.nom,
			(get_rs(get_memory(adresse))),
			(get_imm(get_memory(adresse))) );
	}
	
	//INSTRUCTIONS TYPE J
	//J : target
	if ((strcmp(inst.nom,"J")==0)||
		(strcmp(inst.nom,"JAL")==0)
		)
	{	fprintf(stdout, "%s %d\n", inst.nom,
			get_target(get_memory(adresse)) );
	}
	
	//J: rs
	if (strcmp(inst.nom,"JR")==0)
	{	fprintf(stdout, "%s $%d\n",inst.nom,
			(get_rs(get_memory(adresse)))  );
	}
	
}

int exec_inst(INSTRUCTION inst, int adresse)
{
	//int i;
	//int j;
	/*for (i=0;i<25;i++)
	{
		//printf("%s,%s, strcmp = %d\n",DICO[i].nom,DICO[i].nom,strcmp(DICO[i].nom,DICO[i].nom));
		//printf("%s,%s, strcmp2 = %d\n",DICO[i].nom,inst.nom,strcmp(DICO[i].nom,inst.nom));
		j=strcmp(DICO[i].nom,inst.nom);
		if (j='0'){execute_cmd_inst(i+1);i=30;}
		
	}*/
	
	if (strcmp(inst.nom,"ADD")==0)
	{	ADD(get_memory(adresse) );
	}
	if (strcmp(inst.nom,"ADDI")==0)
	{	ADDI(get_memory(adresse) );
	}
	if (strcmp(inst.nom,"SUB")==0)
	{	SUB(get_memory(adresse) );
	}
	if (strcmp(inst.nom,"MULT")==0)
	{	MULT(get_memory(adresse) );
	}
	if (strcmp(inst.nom,"DIV")==0)
	{	DIV(get_memory(adresse) );
	}
	if (strcmp(inst.nom,"AND")==0)
	{	AND(get_memory(adresse) );
	}
	if (strcmp(inst.nom,"OR")==0)
	{	OR(get_memory(adresse) );
	}
	if (strcmp(inst.nom,"XOR")==0)
	{	XOR(get_memory(adresse) );
	}
	if (strcmp(inst.nom,"ROTR")==0)
	{	ROTR(get_memory(adresse) );
	}
	if (strcmp(inst.nom,"SLL")==0)
	{	SLL(get_memory(adresse) );
	}
	if (strcmp(inst.nom,"SRL")==0)
	{	SRL(get_memory(adresse) );
	}
	if (strcmp(inst.nom,"SLT")==0)
	{	SLT(get_memory(adresse) );
	}
	if (strcmp(inst.nom,"LW")==0)
	{	LW(get_memory(adresse) );
	}
	if (strcmp(inst.nom,"SW")==0)
	{	SW(get_memory(adresse) );
	}
	if (strcmp(inst.nom,"LUI")==0)
	{	LUI(get_memory(adresse) );
	}
	if (strcmp(inst.nom,"MFHI")==0)
	{	MFHI(get_memory(adresse) );
	}
	if (strcmp(inst.nom,"MFLO")==0)
	{	MFLO(get_memory(adresse) );
	}
	if (strcmp(inst.nom,"BEQ")==0)
	{	BEQ(get_memory(adresse) );
	}
	if (strcmp(inst.nom,"BNE")==0)
	{	BNE(get_memory(adresse) );
	}
	if (strcmp(inst.nom,"BQTZ")==0)
	{	BGTZ(get_memory(adresse) );
	}
	if (strcmp(inst.nom,"BLEZ")==0)
	{	BLEZ(get_memory(adresse) );
	}
	if (strcmp(inst.nom,"J")==0)
	{	J(get_memory(adresse) );
	}
	if (strcmp(inst.nom,"JAL")==0)
	{	JAL(get_memory(adresse) );
	}
	if (strcmp(inst.nom,"JR")==0)
	{	JR(get_memory(adresse) );
	}
	if (strcmp(inst.nom,"SYSCALL")==0)
	{	SYSCALL(get_memory(adresse) );
	}
return CMD_OK_RETURN_VALUE;	
}

int isbp(unsigned int adresse){
	/*int i;
	
	for(i=0;i<taille_bp;i++)
	{	if(breakpoints[i]==adresse)
		return 1;
	}*/
	return 0;
}

void test_liste() {
	DEBUG_MSG("entrée dans la fonction test_liste");
	breakpoint b;
	b.bp=9+1;
	b.numero=69;
	b.commande="commandehqofe";
	//DEBUG_MSG("initial %s", b.commande);
	//DEBUG_MSG("initial numero %d", b.numero);
	b.actif='0';
	//DEBUG_MSG("initial actif %c", b.actif);
	
	Liste ma_liste=creer_liste();
	{breakpoint b;
	b.bp=9;
	b.numero=70;
	b.commande="blouce";
	b.actif='0';
	ma_liste=ajout_tete(b,ma_liste);
	//visualiser_liste(ma_liste);
	
	breakpoint op;
	op.bp=0x4;
	op.numero=95;
	op.commande="bloussssse";
	op.actif='0';	
	ma_liste=ajout_tete(op,ma_liste);
	//visualiser_liste(ma_liste);
}

	DEBUG_MSG("AJOUT ORDRE");
	ma_liste=ajoute_ordre(b, ma_liste);

	breakpoint caca;
	caca.bp=2;
	caca.numero=41;
	caca.commande="caca";
	caca.actif='0';

	ma_liste=ajoute_ordre(caca, ma_liste);


	//DEBUG_MSG("\nrésultat de ajoute ordre\n");
	//visualiser_liste(ma_liste);

	
	//DEBUG_MSG("ON RECOMMENCE");
	caca.commande="recommenciation";
	ma_liste=ajoute_ordre(caca, ma_liste);
	visualiser_liste(ma_liste);

	DEBUG_MSG("         TEST DE SUPPRESSION");
	ma_liste=supprime(5,ma_liste);
	visualiser_liste(ma_liste);

	DEBUG_MSG("         TEST DE RECHERCHE");
	DEBUG_MSG("%d", recherche(ma_liste, 0x9));


}
	



	
