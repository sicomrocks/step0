#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <readline/readline.h>
#include <readline/history.h>
#include "fonctions.h"
#include "notify.h"
#include "memoire.h"

int parse_and_execute_cmd_testcmd(char * paramsStr);
int execute_cmd_testcmd(int hexValue);

int parse_and_execute_cmd_exit(char * paramsStr);
int execute_cmd_exit();

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
int execute_cmd_lp(char* token);

/*
afficher les registres :
* afficher le contenu des registres sous la forme
* nom : valeur (valeur écrite en 8 chiffres hexadécimaux
*/
int parse_and_execute_cmd_dr(char* paramsStr);
/*l'argument est optionnel ; s'il y a un argument ce doit être le nom du registre à afficher (char*) , sinon on les affiche tous*/
int execute_cmd_dr_un(int num_registre);
int execute_cmd_dr_tous();

/*
modifier une valeur dans un registre :
écrit, dans le registre dont le numéro est num_reg, la valeur value
*/
int parse_and_execute_cmd_lr(char* paramsStr);
int execute_cmd_lr(int num_reg, int value);

int execute_cmd_da(char* adresse, int* nb_instructions);
int parse_and_execute_cmd_da(char* paramsStr);
/*

*/

int execute_cmd_lm(char* adresse,char* valeur);
int parse_and_execute_cmd_lm(char* paramsStr);
/*
La commande écrit à l'adresse (hexa) donnée en premier paramètre la valeur (hexa d'octet) donnée en deuxième paramètre, ces paramètres séparés par un espace.
La valeur entière peut être sur 1 à 8 chiffres hexadécimaux
*/

int execute_cmd_dm(char* Adress);
int parse_and_execute_cmd_dm(char* paramsStr);
/*

*/

/*fonction qui affiche les caractéristiques de l'instruction voulue (donnée par le num de la ligne du dico)*/
int parse_and_execute_cmd_inst(char* paramsStr);
int execute_cmd_inst(int n);
