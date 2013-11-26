
#ifndef _FONCTIONS_H_
#define _FONCTIONS_H_

#include <string.h>
#include "notify.h"
#include <stdio.h>
#include <stdlib.h>  // exit

#include "instructions.h"
#include "memoire.h"



/*fonction pour regarder si le paramètre est bien un nom de registre.
 * Si oui, elle renvoie le numéro du registre (PC=32 SR=33 HI=34 LO=35, si non elle renvoie -1*/
int isregister(char* param);

//fonction qui récupère le type (binaire, décimal ou hexa) d'un nombre
int automate(char* nombre );

int isadress(char* param);
int adressType(char* param);

/*fonction qui vide le dictionnaire*/
void free_memory();

//fonction qui désassemble l'instruction instr
INSTRUCTION desassemble(char* instr_hexa);

//fonction qui convertit une chaîne de caractères représentant un hexadécimal en un binaire et le range bit à bint dans le tableau bin[]
int conv_hex_bin(char* hexa, char bin[]);

//fonction qui convertit un entier en une chaîne de caractères qui le représente en hexadécimal
int conv_int_str(unsigned int value, char** string);

//fonction qui trouve le numéro de l'instruction en binaire
int recup_num(char instr_bin[]);

//fonction qui affiche une instruction (nom, type, nombre d'opérandes, liste les opérande, opcode, func
void affiche_inst(INSTRUCTION i);
void affiche_inst_brut(INSTRUCTION i);

//fonction qui execute l'instruction
int exec_inst(INSTRUCTION inst);

//regarde si l'adresse a un breakpoint : retourne 1 si oui et 0 si non
int isbp(unsigned int adresse);


#endif
