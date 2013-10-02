#include "fonctions.h"
#include "notify.h"
#include <stdlib.h>
#include <stdio.h>


int isregister(char* param) {
	//DEBUG_MSG("entrée dans la fonction isregister");
	/*il y a deux types de noms de registres : 
		PC SR HI LO
		$x avec 0<=x<=31 ; si x=4 : $04
	*/
	if (strlen(param)<2 || strlen(param)>3) {
		return 0;
	}
	
	if (param[0]==36) {	//commence par "$"
		//regarder le numéro du registre demandé
	
		char x[1];
		x[0]=param[1];
		//DEBUG_MSG("code ascii du premier chiffre = %d", x[0]);
		int nombre=0; 
		nombre=atoi(x); 
		//DEBUG_MSG("premier chiffre %d", nombre); 
	
		char y[1];
		y[0]=param[2];
		//DEBUG_MSG("code ascii du deuxième chiffre = %d", y[0]);
		int nbe=0;
		nbe=atoi(y);
		//DEBUG_MSG("deuxième chiffre %d", nbe);
	
		int numero=10*nombre+nbe;
		//DEBUG_MSG("numéro du registre demandé %d", numero);
	
		//vérifier qu'il est dans le bon intervalle
		if (numero<0 || numero >31) {
			return 0;
		}
	}
	
	else {
		if (param[0]==80 && param[1]==67) return 1;	//PC
		if (param[0]==83 && param[1]==82) return 1;	//SR
		if (param[0]==72 && param[1]==73) return 1;	//HI
		if (param[0]==76 && param[1]==79) return 1;	//LO
		return 0;
	}
	
	return 1;
}
	
