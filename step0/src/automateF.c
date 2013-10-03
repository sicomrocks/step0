#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<automateF.h>

// Def des différents états
enum { INIT , DECIMAL_ZERO, DEBUT_HEXA, HEXA, DECIMAL , OCTAL} ;
// mise en oeuvre de l'automate
int automate(char* nombre ) {
	int c ;         //caractère analyse courante
	int S=INIT ;    // etat de l'automate
	FILE* pf ;
	//int i;

	pf=fopen ( "nombre.txt","wt");
	fputs(nombre,pf);
	fclose(pf);

	pf=fopen ( "nombre.txt","rt");
	while (EOF!=(c=fgetc(pf))){  //EOF = End of File
	switch (S) {
		case INIT :
			//i=0;
			if(isdigit(c)){ // si c'est un chiffre
				if (c=='0')
				S=DECIMAL_ZERO; else S= DECIMAL ;
			}
			else if ( isspace(c)) {
				S=INIT ;
			}
			else if ( c==EOF) {
				return 0 ; // fin de fichier
			}
			//else return erreur_caractere ( string , i , c ) ;
			else {
				perror (  "erreur caracter (etape init)"  ); 
				return 0;
			}
		break ;

		case DECIMAL_ZERO: // reperage du prefixe de l ’ hexa
		if ( c == 'x' || c == 'X' ) S=HEXA;
		else if ( isdigit(c) && c<'8' ) S=OCTAL; // c ' est un octal
		else if ( c==EOF || isspace (c) ) { S=INIT ;
		printf ( "la chaine est sous forme decimale\n" ) ;
                                                }
		//else return erreur_caractere ( string,i,c ) ;
		else {
			perror (  "erreur caracter (etape DeciZero)"  );
			return 0;
		}
		break ;

		case DEBUT_HEXA: // il faut au moins n chiffre après x
		if (isxdigit(c)) {
			S=HEXA;
		}
		//else return erreur_caractere ( string,i,c ) ;
		else {
			perror (  "erreur caracter (etape DebutHexa)"  );
			return 0;
		}
		break ;

		case HEXA: // tant que c'est un chiffre hexa
		if( isxdigit(c)) {
			S=HEXA;
		}
		else if ( c==EOF || isspace(c)) { 
			S=INIT ;
			printf ( " la chaine est sous forme hexadecimale\n") ;
		}
		//else return erreur_caractere ( string,i,c ) ;
		else {
			perror (  "erreur caracter (etape hexa)"  ); 
			return 0;
		}
		break ;

		case DECIMAL : // tant que c'est un chiffre
		if ( isdigit(c)) {
			S=DECIMAL ;
		}
		else if ( c==EOF || isspace(c)) { 
			S=INIT ;
			printf ( " la chaine est sous forme decimale\n");
		}
		//else return erreur_caractere ( string,i,c ) ;
		else {
			perror (  "erreur caracter (etape Decimal)"  ); 
			return 0;
		}
		break ;

		case OCTAL: // tant que c'est un chiffre
		if( isdigit(c)&& c<'8' ) {
			S=OCTAL;
		}
		else if ( c==EOF || isspace(c)) { 
			S=INIT ;
			printf ( " la chaine est sous forme octale \n" ) ;
		}
		//else return erreur_caractere (string,i,c ) ;
		else {
			perror (  "erreur caracter (etape Octal)"  ); 
			return 0;
		}
		break ;
	}
	}
	return S;
}

