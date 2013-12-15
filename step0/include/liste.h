
typedef struct {
	unsigned int bp;
	char actif;
	unsigned int numero;
	char* commande;	// désassemblage de la ligne pointée par le bp
} breakpoint;



struct cellule {
	breakpoint element;
	struct cellule* suiv;
	};
typedef struct cellule Maillon;
typedef Maillon* Liste;



Liste creer_liste(void);

int est_vide(Liste l);

int visualiser_liste(Liste l);

Liste ajout_tete(breakpoint c, Liste l);
 
Liste supprimer_tete(Liste l) ;

Liste ajout_queue(breakpoint c, Liste l);

//ajoute la liste l2 à la suite de la liste l1 sans recréer de nouveaux éléments. La fonction retourne la nouvelle liste ainsi formée. Que deviennent alors les listes l1 et l2 ?
Liste concat(Liste l1, Liste l2);

//crée une nouvelle liste qui est une copie de la liste l. Tous les éléments de la liste l sont dupliqués.
Liste copie(Liste l);

//supprime la carte en position n dans la liste l et retourne la nouvelle liste. Attention : il faut être sur le maillon n-1 pour supprimer le maillon n et chainer le maillon n-1 avec le maillon n.
Liste supprime(int n, Liste l);

//recherche dans la liste l'élément dont l'adresse est champ ; renvoie la position de cet élément dans la liste
int recherche(Liste l, unsigned int champ);

//ajoute l'émément c dans la liste l en respectant l'ordre
Liste ajoute_ordre(breakpoint c, Liste l);

//met à jour les indices dans une liste et renvoie le nombre d'ékéments dans la liste
int numerote(Liste l);

//vide la liste
Liste supprime_tout(Liste l);
