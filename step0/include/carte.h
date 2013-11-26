#ifndef _CARTE
#define _CARTE
enum { TREFLE,CARREAU,COEUR,PIQUE} COULEUR;

typedef struct { 
	int rang; 
	char visible; 
	char couleur; 
	} CARTE;
#endif
