#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memoire.h"


void init_reg(REGISTRE* tab) {
	
	INFO_MSG("Initialisation des registres");
	
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

void init_instr(INSTRUCTION* tab) { //tab fait 25 lignes et contient des INSTRUCTION

        INFO_MSG("Initialisation du dictionnaire d'instructions");

        char* mot=NULL;
        char ligne[512];
        const char* delim=" ";
        
        FILE* f=fopen("src/dico.txt", "rt");
        if (f==NULL) {
                WARNING_MSG("Impossible d'ouvrir le dictionnaire");
                exit(2);
        }
        
        int i=1;
        while (fgets(ligne, 512, f) != NULL) {
		delim=" ";
        
                //premier mot : char* nom
                mot=strtok(ligne, delim);
                strcpy(DICO[i-1].nom, mot);
                
                //2è mot : char* type
                mot=strtok(NULL, delim);
                strcpy(DICO[i-1].type, mot);

                
                //3è mot : int nbe_op
                mot=strtok(NULL, delim);
                DICO[i-1].nbe_op=atoi(mot);


                
                //4èmot : char* ops[3] : tableau de char* contenant le nom des opérandes (jusqu'à trois)
                if (DICO[i-1].nbe_op > 0) {
                        mot=strtok(NULL, delim);
                        strcpy((DICO[i-1].ops[0]), mot);

                        DICO[i-1].ops[1][0]=0;
                        DICO[i-1].ops[2][0]=0;

                }
                if (DICO[i-1].nbe_op > 1) {
                        mot=strtok(NULL, delim);
                        strcpy(DICO[i-1].ops[1],mot);
                        DICO[i-1].ops[2][0]=0;
                }
                if (DICO[i-1].nbe_op > 2) {
                        mot=strtok(NULL, delim);
                        strcpy(DICO[i-1].ops[2],mot);
                }
                
                //mot suivant :  opcode
                mot=strtok(NULL, delim); //mot a pour valeur "opcode"
                delim="\n";
                mot=strtok(NULL, delim);
            //    DEBUG_MSG("mot %s", mot);
                delim=" ";
                char *mot2=strtok(mot, delim);
                strcpy(DICO[i-1].opcode, mot);
                
                //DEBUG_MSG("%d", strlen(DICO[i-1].opcode));
                //DEBUG_MSG("opcode : %s", DICO[i-1].opcode);
                
		//dernier mot : unsigned int function
                mot2=strtok(NULL, delim);
              //  DEBUG_MSG("gyhkgi %s", mot2);
                if (mot2 != NULL) {
			delim="\n";
                        mot2=strtok(NULL, delim);
                        strcpy(DICO[i-1].func, mot2);
                        
                   //     DEBUG_MSG("function : %s", DICO[i-1].func);
                }
                else {
                        //DICO[i-1].func='noFunc';
                        //DEBUG_MSG("function : 0x%x", DICO[i-1].func);
                }
                i++;
        }
        //affichage des valeurs du dico
        for (i=0 ; i<taille_DICO ; i++) {
                if (DICO[i].nom == NULL) {
                        DEBUG_MSG("error");
                }
                //DEBUG_MSG("%d %s %s %d %s %s %s 0x%x 0x%x",i+1, DICO[i].nom, DICO[i].type, DICO[i].nbe_op, DICO[i].ops[0], DICO[i].ops[1], DICO[i].ops[2], DICO[i].opcode, DICO[i].func);
        }
        //int nombre=101011;
        //DEBUG_MSG("%d", nombre);

}
