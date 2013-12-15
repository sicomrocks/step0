#include "liste.h"
#include "notify.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global.h"

Liste creer_liste(void) {
	return NULL;
}

int est_vide(Liste l) {
		if (l==NULL) {
			return 1;
		}
		else return 0;
}

int visualiser_liste(Liste l) {
	DEBUG_MSG("entrée dans visualiser liste");

	if (est_vide(l) == 1) {
		return 2;
	}
	
	Liste p;
	p=l;

	//DEBUG_MSG("%d", est_vide(l));
	
	while (est_vide(p)==0) {
		fprintf(stdout, "numero %d, adresse 0x%x,  actif %c, instruction %s\n",p->element.numero, p->element.bp, p->element.actif,  p->element.commande);
		p=p->suiv;
	}

	return 0;
}

Liste ajout_tete(breakpoint c, Liste l) {
	DEBUG_MSG("\nentrée dans la fonction ajout_tete");
	if (est_vide(l)==1) {
		l=calloc(1,sizeof(*l));
		//MATT VERIFIE CALLLOC
		//DEBUG_MSG("recopie des champs");
	
		l->element.actif=c.actif;
		//DEBUG_MSG("actif %c", l->element.actif);
	
		l->element.bp=c.bp;
		//DEBUG_MSG("adresse 0x%x", l->element.bp);
	
		l->element.numero=c.numero;
		//DEBUG_MSG("numero %d", l->element.numero);
	
		l->element.commande=strdup(c.commande);
	//	DEBUG_MSG("commande %s", l->element.commande);
	
		l->suiv=NULL;

	//	DEBUG_MSG("vide ? %d", est_vide(l));
		return l;

	}
	
	Liste p=creer_liste();
	p=calloc(1,sizeof(*p));
	if (p==NULL) {
		//DEBUG_MSG("truc");
		WARNING_MSG("erreur lors de l'allocation d'une liste");
		return NULL;
	}
	//DEBUG_MSG("recopie des champs");
	
	p->element.actif=c.actif;
	//DEBUG_MSG("actif %c", p->element.actif);
	
	p->element.bp=c.bp;
	//DEBUG_MSG("adresse %d", p->element.bp);
	
	p->element.numero=c.numero;
	//DEBUG_MSG("numero %d", p->element.numero);
	
	p->element.commande=strdup(c.commande);
	//DEBUG_MSG("commande %s", p->element.commande);
	
	p->suiv=l;
	return p;
	
}

Liste supprimer_tete(Liste l) {
	if (est_vide(l)==0) {
		Liste p;
		p=l->suiv;
		free (l);
		return p;
	}
	else {
		return NULL;
	}
}
/*
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
*/
Liste supprime(int n, Liste l) { //supprime le maillon en position n dans la liste l
	DEBUG_MSG("entrée dans supprimer");
	if (n==0) {
		DEBUG_MSG("impossible de retirer l'élément 0 d'une liste : liste inchangée");
		return l;
	}

	//on parcourt la liste pour savoir combien elle a d'éléments
	Liste q=creer_liste();
	q=l;
	while (est_vide(q->suiv)==0) {
		q=q->suiv;
	}
	unsigned int nomb;
	nomb=q->element.numero;
	if (nomb<n) {
		DEBUG_MSG("la liste ne comporte pas %d éléments : liste inchangée", n);
		return l;
	}
	
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
	numerote(l);
	return l;	
}

Liste ajoute_ordre(breakpoint c, Liste l) {
	DEBUG_MSG("entrée dans la fonction ajoute ordre");

	if (est_vide(l)==1) {
		l=ajout_tete(c, l);
	//	DEBUG_MSG("ici %d", est_vide(l));
		numerote(l);
		return l;
	}

	//on regarde si l'élément n'est pas déjà dans la liste
	DEBUG_MSG("recherche dans ajoute ordre %d", recherche(l, c.bp));
	if (recherche(l, c.bp)!=0) {
		DEBUG_MSG("l'élément est déjà dans la liste ; liste inchangée");
		return l;
	}

	
	Liste p=creer_liste();
	p=l;
	//DEBUG_MSG("on crée un maillon qui contient l'élément c");
	Liste maillon=creer_liste();
	maillon=ajout_tete(c,maillon);

	Liste j=creer_liste();
	j=p;

	if (maillon->element.bp < j->element.bp) {
		//le maillon doit aller en première position
		p=ajout_tete(c,p);
		//visualiser_liste(p);
		numerote(p);
		return p;
	}

	while(est_vide(p->suiv)==0) {
		//DEBUG_MSG("entrée dans la boucle while");
		if (maillon->element.bp < p->suiv->element.bp) {
			//DEBUG_MSG("lalalala");
			maillon->suiv=p->suiv;
			p->suiv=maillon;
			//visualiser_liste(j);
			numerote(j);
			return j;
		}
		p=p->suiv;
	}
	//DEBUG_MSG("sortie de la boucle while");
	
	//si on arrive ici c'est que le maillon doit aller à la fin de la liste
	p->suiv=maillon;

	//DEBUG_MSG("premier de j %d", j->element.bp);
	numerote(j);
	
	return j;
}

int numerote(Liste l) {
	DEBUG_MSG("entrée dans la fonction numerote");
	int n=1;
	Liste p=creer_liste();
	p=l;
	//DEBUG_MSG("%d", p->element.bp);
	while (est_vide(p)==0) {
		p->element.numero=n;
		p=p->suiv;
		n++;
	}


	return 0;
}



int recherche(Liste l, WORD champ) {
	if (est_vide(l)==1) {
		DEBUG_MSG("il n'y a rien dans la liste de breakpoints");
		return 0;
	}

	
	
	Liste p=creer_liste();
	p=l;
	while (p->element.bp!=champ && p->suiv!=NULL) {
		p=p->suiv;
	}
	if (p->element.bp==champ) {
		return p->element.numero;
	}
	DEBUG_MSG("l'élément d'adresse %d ne figure pas dans la liste", champ);
	return 0;
}









