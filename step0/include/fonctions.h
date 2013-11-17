#include <string.h>
#include "notify.h"
#include <stdio.h>
#include <stdlib.h>  // exit

#include "memoire.h"

#ifndef _FONCTIONS_H_
#define _FONCTIONS_H_

/*fonction pour regarder si le paramètre est bien un nom de registre.
 * Si oui, elle renvoie le numéro du registre (PC=32 SR=33 HI=34 LO=35, si non elle renvoie -1*/
int isregister(char* param);

int automate(char* nombre );
int isadress(char* param);
int adressType(char* param);

/*fonction qui vide le dictionnaire*/
void free_memory();

//fonction qui désassemble l'instruction instr
int desassemble(char* instr_hexa);

//fonction qui convertit une chaîne de caractères représentant un hexadécimal en un binaire et le range bit à bint dans le tableau bin[]
int conv_hex_bin(char* hexa, char bin[]);

//fonction qui convertit un entier en une chaîne de caractères qui le représente en hexadécimal
int conv_int_str(unsigned int value, char** string);

//fonction qui trouve le numéro de l'instruction en binaire
int recup_num(char instr_bin[]);

//fonction qui remplit les champs de l'instruction instr_binaire en cours de désassemblage.
//Cette instruction est à la ligne num de dico.txt, càd porte l'indice i-1 dans DICO
void remplit_inst(INSTRUCTION* decode,char instr_binaire,int num);

#endif
