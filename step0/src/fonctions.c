#include "fonctions.h"
#include "notify.h"
#include <stdlib.h>
#include <stdio.h>
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
		DEBUG_MSG("param commence par $");
		//regarder si c'est un numero ou un mnemo
		//les chiffres ont un code ascii entre 48 et 57
		char x[1];
		x[0]=param[1]; //on a récupéré le premier catactère
		//DEBUG_MSG("premier caractère %d", atoi(x));
		
		if (x[0]>47 && x[0]<58) {	//cas où c'est le numero du registre qui est appelé
			DEBUG_MSG("cherche le numero");
			int nombre=0; 
			nombre=atoi(x); 
			DEBUG_MSG("premier chiffre %d", nombre); 
	
			char y[1];
			y[0]=param[2];
			//DEBUG_MSG("code ascii du deuxième chiffre = %d", y[0]);
			int nbe=0;
			nbe=atoi(y);
			DEBUG_MSG("deuxième chiffre %d", nbe);
			int numero=10*nombre+nbe;
			DEBUG_MSG("numéro du registre demandé %d", numero);
			//vérifier qu'il est dans le bon intervalle
			if (numero<0 || numero >35) {
				return -1;
			}
			else return numero;
		}
		
		else {		//cas où c'est le mnemo qui est appelé
			DEBUG_MSG("cherche le nom");
			DEBUG_MSG("%d %d", param[1], param[2]);
			char mot[2];
			mot[0]=param[1];
			mot[1]=param[2];
			mot[2]='\0';
			
			//strcat(mot, atoi(lettre2));
			DEBUG_MSG("%s", mot);
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
	
