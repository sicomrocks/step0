/**
 * @file simMips.c
 * @author François Cayre, Nicolas Castagné
 * @date Fri Jun 15 18:13:02 2012 puis aout 2013
 * @brief Main pour le début du projet simulateur MIPS.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* la librairie readline */
#include <readline/readline.h>
#include <readline/history.h>

/* macros de DEBUG_MSG fournies , etc */
#include "notify.h"

//#include "memoire.h"
#include "parseExecute.h"

/* prompt du mode shell interactif */
#define PROMPT_STRING "SimMipsShell : > "

/* taille max pour nos chaines de char */
#define MAX_STR 1024


/*************************************************************\
Valeur de retour speciales lorsqu'on pour la fonction
	int parse_and_execute_cmd_string(char *input) ;
Toute autre valeur signifie qu'une erreur est survenue
 \*************************************************************/
#define CMD_OK_RETURN_VALUE 0
#define CMD_EXIT_RETURN_VALUE -1
#define CMD_EMPTY_RETURN_VALUE -2
#define CMD_UNKOWN_RETURN_VALUE -3


/*************************************************************\
Les commandes du simulateur.

 Dans cette version, deux commandes :
	"testcmd" qui attend un nombre hexa strictement positif et affiche ce nombre +1 dans le terminal
	"exit" qui quitte le simulateur

 Deux fonctions C par commande :
	1/ une qui parse les paramètres "parse_and_execute_cmd_<nomdecommande>(char *)"
	2/ une qui exécute la commande "execute_cmd_<nomdecommande>(...)"
 \*************************************************************/




/*************************************************************\
 Les deux fonctions principales du simulateur.
	parse_and_execute_cmd_string ( char *) parse la commande et l'execute en appelant la bonne fonction C
	acquire_line( FILE *, char *) : recupere une ligne (donc une "commande") dans le flux
 \*************************************************************/
/**
 * @fn uint  parse_and_execute_cmd_string( char *input )
 *
 * @brief parse la chaine input a la recherche d'une commande, et execute cette commande.
 *
 * @return CMD_OK_RETURN_VALUE si la commande s'est exécutée avec succès (0)
 * @return CMD_EXIT_RETURN_VALUE si c'est la commande exit. Dans ce cas, le programme doit se terminer. (-1)
 * @return CMD_EMPTY_RETURN_VALUE si input n'a aucune commande (chaine vide) ou si commentaire (# qq chose) (-2)
 * @return CMD_UNKOWN_RETURN_VALUE si la commande n'est pas reconnue. (-3)
 * @return tout autre nombre (eg tout nombre positif) si erreur d'execution de la commande
 */
int parse_and_execute_cmd_string(char *input) {
    DEBUG_MSG("input '%s'", input);
    char cmdStr[MAX_STR];
    memset( cmdStr, '\0', MAX_STR );

    /* gestion des commandes vides, commentaires, etc*/
    if(strlen(input) == 0
            || sscanf(input, "%s", cmdStr) == 0 //sscanf renvoie le nbe de mots dans input
            || strlen(cmdStr) == 0
            || cmdStr[0] == '#') { /* ligne commence par # => commentaire*/
        return CMD_EMPTY_RETURN_VALUE;
    }

    /*parsing et execution des commandes !*/
    if(strcmp(cmdStr, "testcmd") == 0) {
        return parse_and_execute_cmd_testcmd(input + strlen(cmdStr) );
    } 
    else if(strcmp(cmdStr, "exit") == 0) {
        return parse_and_execute_cmd_exit(input + strlen(cmdStr) );
    }
    else if(strcmp(cmdStr, "lp") == 0) {
        return parse_and_execute_cmd_lp(input + strlen(cmdStr) );
    }
    /*else if(strcmp(cmdStr, "dm") == 0) {
        return parse_and_execute_cmd_dm(input + strlen(cmdStr) );
    }
    else if(strcmp(cmdStr, "da") == 0) {
        return parse_and_execute_cmd_da(input + strlen(cmdStr) );
    }*/
    else if(strcmp(cmdStr, "dr") == 0) {
        return parse_and_execute_cmd_dr(input + strlen(cmdStr) );
    }
    /*else if(strcmp(cmdStr, "lm") == 0) {
        return parse_and_execute_cmd_lm(input + strlen(cmdStr) );
    }*/
    else if(strcmp(cmdStr, "lr") == 0) {
        return parse_and_execute_cmd_lr(input + strlen(cmdStr) );
    }
    
    WARNING_MSG("Unknown Command : '%s'\n", cmdStr);
    return CMD_UNKOWN_RETURN_VALUE;
}


/**
 * @fn uint  acquire_line( FILE *fp, char *input )
 *
 * @brief extrait la prochaine ligne la flux fp.
 * Si fp ==stdin, utilise la librairie readline pour gestion d'historique de commande.
 *
 * @return 0 si succes.
 * @return un nombre non nul si aucune ligne lue
 */
int  acquire_line(FILE *fp, char * input) {
    memset( input, '\0', MAX_STR );
    if ( stdin == fp ) {
        /* mode shell interactif */
        /* on utilise la librarie libreadline pour disposer d'un historique */
        char *chunk = readline( PROMPT_STRING );
        if (  strlen(chunk) == 0) {
            /* commande vide... */
            return 1;
        }
        /* ajout de la commande a l'historique, librairie readline */
        add_history( chunk );
        strcpy( input, chunk );

        free( chunk ); /* liberation de la mémoire allouée par la fonction readline() de libreadline */
    }
    else {
        /* mode fichier */
        /* acquisition d'une ligne dans le fichier */
        char * ret = fgets( input, MAX_STR, fp );
        if(ret == NULL) {
            return 1;
        }
    }
    DEBUG_MSG("Ligne acquise '%s'\n", input); /* macro DEBUG_MSG : uniquement si compil en mode DEBUG_MSG */
    return 0;
}


/****************/
void usage_ERROR_MSG( char *command ) {
    fprintf( stderr, "Usage: %s [file.sim]\n   If no file is given, executes in Shell mode.", command );
}


/**
 * Programme principal
 */
int main ( int argc, char *argv[] ) {
    /* exemples d'utilisation des macros du fichier notify.h */
    INFO_MSG("Un message INFO_MSG : Debut du programme %s", argv[0]); /* macro INFO_MSG */
    WARNING_MSG("Un message WARNING_MSG !"); /* macro INFO_MSG */
    DEBUG_MSG("Un message DEBUG_MSG !"); /* macro DEBUG_MSG : uniquement si compil en mode DEBUG_MSG */

    FILE *fp = NULL; /* le flux dans lequel les commandes seront lues : stdin (mode shell) ou un fichier */

    if ( argc > 2 ) {
        usage_ERROR_MSG( argv[0] );
        exit( EXIT_FAILURE );
    }
    if(argc == 2 && strcmp(argv[1], "-h") == 0) {
        usage_ERROR_MSG( argv[0] );
        exit( EXIT_SUCCESS );
    }

    /*par defaut : mode shell interactif */
    fp = stdin;
    if(argc == 2) {
        /* mode fichier de commandes */
        fp = fopen( argv[1], "r" );
        if ( fp == NULL ) {
            perror( "fopen" );
            exit( EXIT_FAILURE );
        }
    }

    /* boucle principale : lit puis execute une cmd en boucle */
    while ( 1 ) {
        char input[MAX_STR];
        if ( acquire_line( fp,  input )  == 0 ) {
            /* Une nouvelle ligne a ete acquise dans le flux fp*/
            int res = parse_and_execute_cmd_string(input); /* execution de la commande */
            switch(res) {
            case CMD_OK_RETURN_VALUE: /* tout s'est bien passé */
                break;
            case CMD_EMPTY_RETURN_VALUE: /* commande vide */
                /* rien a faire ! */
                break;
            case CMD_EXIT_RETURN_VALUE:
                /* sortie propre du programme */
                if ( fp != stdin ) {
                    fclose( fp );
                }
                exit(EXIT_SUCCESS);
                break;
            default:
                /* erreur durant l'execution de la commande */
                /* En mode "fichier" toute erreur implique la fin du programme ! */
                if ( fp != stdin ) {
                    fclose( fp );
                    /*macro ERROR_MSG : message d'erreur puis fin de programme ! */
                    ERROR_MSG("ERREUR DETECTEE. Aborts");
                }
                break;
            }
        }
        if( fp != stdin && feof(fp) ) {
            /* mode fichier, fin de fichier => sortie propre du programme */
            DEBUG_MSG("FIN DE FICHIER");
            fclose( fp );
            exit(EXIT_SUCCESS);
        }
    }
    /* tous les cas de sortie du programme sont gérés plus haut*/
    ERROR_MSG("SHOULD NEVER BE HERE\n");
}
