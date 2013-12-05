#include "liste.h"
#include "notify.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Liste creer_liste(void) {
	return NULL;
}

int est_vide(Liste l) {
	return !l;
}

void visualiser_liste(Liste l) {
	Liste p;
	p=l;
	while (!est_vide(p)) {
		DEBUG_MSG("adresse %x,  actif %c, numero %d, instruction %s", p->element.bp, p->element.actif, p->element.numero, p->element.commande);
		p=p->suiv;
	}
}

Liste ajout_tete(breakpoint c, Liste l) {
	DEBUG_MSG("entrée dans la fonction ajout_tete");
	Liste p=calloc(1,sizeof(*p));
	if (p==NULL) {
		DEBUG_MSG("truc");
		return NULL;
	}
	DEBUG_MSG("recopie des champs");
	
	p->element.actif=c.actif;
	DEBUG_MSG("actif %c", p->element.actif);
	
	p->element.bp=c.bp;
	DEBUG_MSG("adresse %d", p->element.bp);
	
	p->element.numero=c.numero;
	DEBUG_MSG("numero %d", p->element.numero);
	
	p->element.commande=strdup(c.commande);
	DEBUG_MSG("commande %s", p->element.commande);
	
	p->suiv=l;
	return p;
}

Liste supprimer_tete(Liste l) {
	if (!est_vide(l)) {
		Liste p;
		p=l->suiv;
		free (l);
		return p;
	}
	else {
		return NULL;
	}
}

Liste ajout_queue(breakpoint c, Liste l) {

	//plus malin : return concat(l,c);

	Liste copie=l;
	if (!l) {
		return ajout_tete(c,NULL);
	}
	while (l->suiv!=NULL) {
		l=l->suiv;
	}
	l->suiv=ajout_tete(c,NULL);
	return copie;
}

Liste concat(Liste l1, Liste l2) {
	Liste p=l1;
	if (p==NULL) {
		return l2;
	}
	else {
		while (p->suiv != NULL) { //tant qu'on n'est pas au dernier maillon
			p=p->suiv; //on avance d'un cran
		}
		fprintf(stdout, "on est arrivé au dernier maillon\n");
		//on raccroche la deuxième liste
		p->suiv=l2;
		fprintf(stdout, "on a raccroché la 2eme liste\n");
		return p; //l1 est devenue "l1 + l2", tandis que l2 reste intouchée. On pourrait faire free(l2) pour libérer l'espace
	}
}

Liste copie(Liste l) {
	Liste p=l;
	Liste copie=creer_liste();
	while (!est_vide(p)) {
		copie=ajout_queue(p->element, copie);
		p=p->suiv;
	}
	return copie;
}

Liste supprime(int n, Liste l) { //supprime le maillon en position n dans la liste l
	if (n!=1){
		Liste p;
		p=l;
		int i;
		for (i=0 ; i<n-2 ; i++) {
			p=p->suiv;
		}
		//on est maintenant sur le maillon n-1
		p->suiv=supprimer_tete(p->suiv); //p->suiv est la liste dont le premier élément est celui d'indice n. On retire cet élément et on raccroche la suite
	}
	else {
		l=supprimer_tete(l);		
	}
	return l;	
}

Liste ajoute_ordre(breakpoint c, Liste l) {
	Liste p;
	p=l;

	DEBUG_MSG("on crée un maillon qui contient l'élément c");
	Liste maillon=calloc(1, sizeof(*maillon));
	
	maillon->element.actif='5';
	DEBUG_MSG("coucou");
	
	/*DEBUG_MSG("%c", maillon->element.actif);
	DEBUG_MSG("%c", c.actif);
	maillon->element.actif=c.actif;
	DEBUG_MSG("idfs");
	maillon->element.bp=c.bp;
	maillon->element.commande=strdup(c.commande);
	maillon->element.numero=c.numero;*/
	maillon->element=c;
	maillon->suiv=NULL;
	
	
	DEBUG_MSG("on regarde si c devrait être le premier élément");
	if (c.bp < l->element.bp) {
		l=ajout_tete(c, l);
		return p;
	}

	DEBUG_MSG("si c est en 2è position");
	if (c.bp < l->suiv->element.bp) {
		maillon->suiv=l->suiv;
		l->suiv=maillon;
		return p;
	}
	
	while (c.bp > l->suiv->element.bp) {
		l=l->suiv;
	}
	DEBUG_MSG("on se trouve maintenant au maillon immédiatement inférieur à c.bp");
	maillon->suiv=l->suiv;
	l->suiv=maillon;

	return l;

	
}

int recherche(Liste l, unsigned int champ) {
	Liste p;
	p=l;
	while (p->element.bp!=champ) {
		p=p->suiv;
	}
	if (p->element.bp==champ) {
		return p->element.numero;
	}
	DEBUG_MSG("erreur lors de la recherche de l'élément dans la liste de bp");
	return -1;
}










