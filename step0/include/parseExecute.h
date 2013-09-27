#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "notify.h"
#include "memoire.h"

int execute_cmd_testcmd(int hexValue);

int parse_and_execute_cmd_testcmd(char * paramsStr);

int execute_cmd_exit();

int parse_and_execute_cmd_exit(char * paramsStr);


/*
charger un programme : 
vérifier que le fichier est au format ELF relogeable
vérifier que les sections .txt .data et .bss ne sont pas vides
recopier chacune de ces sections dans la mémoire, les octets dans chaque section étant copiés les uns à la suite des autres
	.text à partir de l'adresse 0x0
	.data juste après .text ; le premier octet de .data a une adresse multiple de 4096
	.bss juste après .data ; le premier octet de .bss a une adresse multiple de 4096
s'il y a des sections de relocation .rel.text .rel.data, mettre à jour les adresses relatives
*/
int parse_and_execute_cmd_lp(char* paramsStr);

/*
afficher les registres :
* afficher le contenu des registres sous la forme
* nom : valeur (valeur écrite en 8 chiffres hexadécimaux
*/
int parse_and_execute_cmd_dr(char* paramsStr);

/*
modifier une valeur dans un registre :
écrit, dans le registre dont le nom est passé en paramètre, la valeur passée en 2è paramètre
*/
int parse_and_execute_cmd_lr(char* paramsStr);
