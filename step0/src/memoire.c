#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memoire.h"


void init_reg(REGISTRE* tab) {
	int i;
	for (i=0 ; i<36 ; i++) {
		tab[i].numero=i;
	}
	strcpy(tab[0].mnemo, "ze");
	strcpy(tab[1].mnemo, "at");
	strcpy(tab[2].mnemo, "v0");
	strcpy(tab[3].mnemo, "v1");
	strcpy(tab[4].mnemo, "a0");
	strcpy(tab[5].mnemo, "a1");
	strcpy(tab[6].mnemo, "a2");
	strcpy(tab[7].mnemo, "a3");
	strcpy(tab[8].mnemo, "t0");
	strcpy(tab[9].mnemo, "t1");
	strcpy(tab[10].mnemo, "t2");
	strcpy(tab[11].mnemo, "t3");
	strcpy(tab[12].mnemo, "t4");
	strcpy(tab[13].mnemo, "t5");
	strcpy(tab[14].mnemo, "t6");
	strcpy(tab[15].mnemo, "t7");
	strcpy(tab[16].mnemo, "s0");
	strcpy(tab[17].mnemo, "s1");
	strcpy(tab[18].mnemo, "s2");
	strcpy(tab[19].mnemo, "s3");
	strcpy(tab[20].mnemo, "s4");
	strcpy(tab[21].mnemo, "s5");
	strcpy(tab[22].mnemo, "s6");
	strcpy(tab[23].mnemo, "s7");
	strcpy(tab[24].mnemo, "t8");
	strcpy(tab[25].mnemo, "t9");
	strcpy(tab[26].mnemo, "k0");
	strcpy(tab[27].mnemo, "k1");
	strcpy(tab[28].mnemo, "gp");
	strcpy(tab[29].mnemo, "sp");
	strcpy(tab[30].mnemo, "fp");
	strcpy(tab[31].mnemo, "ra");
	strcpy(tab[32].mnemo, "PC");
	strcpy(tab[33].mnemo, "SR");
	strcpy(tab[34].mnemo, "HI");
	strcpy(tab[36].mnemo, "LO");
	
	
	//DEBUG_MSG("%s", tab[0].mnemo);
	
}

void init_instr(INSTRUCTION* tab) {	//tab fait 25 lignes et contient des INSTRUCTION
	char* mot=NULL;
	char ligne[512];
	const char* delim=" ";
	
	FILE* f=fopen("src/dico.txt", "rt");
	if (f==NULL) {
		WARNING_MSG("Impossible d'ouvrir le dictionnaire");
		exit(2);
	}
	
	int i=1;
	//while (f!=EOF) {
		//lire une ligne
		//fgets(ligne, 512, f);
		
		while (fgets(ligne, 512, f) != NULL) {
			//ligne[strlen(ligne-1)-1]="\0";
			DEBUG_MSG("ligne n° %d : %s", i, ligne);
		
			mot=strtok(ligne, delim);
			DEBUG_MSG("mot : %s", mot);
			//if (mot != NULL) {
			//	DEBUG_MSG("mot : %s", mot);
			//}
				while( mot != NULL ) {
				// on parcourt toute la ligne
					mot = strtok( NULL, delim  );
					if (mot != NULL) {
						DEBUG_MSG("mot : %s", mot);
					}
				}
				i++;
				//fgets(ligne, 512, f);
			
		}
}
