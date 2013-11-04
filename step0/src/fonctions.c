#include "fonctions.h"
#include "notify.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "reg.h"
#include "memoire.h"




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
{
	char* buffer;
	buffer=strdup(param);
    if (automate(buffer)==3 /*&& param<adresse_max*/)
    return 1;
    else return 0;
}

// Def des différents états
enum { INIT , DECIMAL_ZERO, DEBUT_HEXA, HEXA, DECIMAL , OCTAL} ;
// mise en oeuvre de l'automate
int automate(char* nombre )
{
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
int adressType(char* param)
{	
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
	
	
void free_memory() {
	DEBUG_MSG("Entrée dans la fonction free_memory");
	int i;
	
	//vidage du dictionnaire d'instructions
	for (i=0 ; i<25 ; i++) {
		DICO[i].nom=NULL;
		DICO[i].type=NULL;
		DICO[i].ops[0]=NULL;
		DICO[i].ops[1]=NULL;
		DICO[i].ops[2]=NULL;
	}	
}
}


