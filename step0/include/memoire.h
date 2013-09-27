#include <stdio.h>
#include <stdlib.h>

//		DÉFINITION DES STRUCTURES MODÉLISANT LA MÉMOIRE ET LES REGISTRES DU MIPS

/*
il y a 32 registres d'usage général, de taille 32 bits.
les registres PC SR HI et LO font eux aussi 32 bits.
*/

typedef int REGISTRE; //32 bits est la taille d'un int

REGISTRE registres[32];

REGISTRE PC; //compteur programme : contient l'adresse de la prochaine instruction

REGISTRE SR; 

REGISTRE HI; //32 bits de poids fort du résultat d'une multiplication MULT ; ou reste d'une division entière

REGISTRE LO; //32 bits de poids faible d'une multiplication ; ou quotient d'une division

/*
on s'intéresse à trois sections : .text .data .bss
on ne connaît que la taille de .bss, pas son contenu
*/

#define taille_nom 4

struct SECTION {
	unsigned int adresse_debut;
	unsigned int taille; 		//en octets, lue dans le programme au moment où on le charge
	char nom[taille_nom];
	int* donnees;
	};

struct ENTREE{ 					//c'est le fichier ELF que l'on veut charger, composé des trois sections :
	struct SECTION text;
	struct SECTION data;
	struct SECTION bss;
	};

/*si on a l'instruction suivante dans le programme : 
	.space 12000
au moment de charger, on réserve les 12000 octets dans la section .bss
*/
