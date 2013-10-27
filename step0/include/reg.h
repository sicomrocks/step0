#ifndef _REG_H_
#define _REG_H_


typedef struct {
	char mnemo[5]; // nom du registre
	int numero;
	int valeur;
	} REGISTRE;



REGISTRE registres[36];


#endif /* _REG_H_ */
