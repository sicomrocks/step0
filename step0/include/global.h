/**
 * SiMIPS - Simulateur d'un microprocesseur MIPS compatible Linux/ELF
 * @author (c) Laurent Fesquet 2003,
 * @author (m) Matthieu Chabanas 2005
 * @author (m) Fran�ois Portet 2013 <francois.portet@imag.fr>
 * @brief Definitions (type, constantes...) communes � l'ensemble du projet
 */


#ifndef _GLOBAL_H_
#define _GLOBAL_H_


/***----------------------------------------------------------------------***/
/***          TYPES DE DONNEES                                            ***/
/***----------------------------------------------------------------------***/

/* ATTENTION: les tailles de (unsigned char) et (unsigned int) ne sont pas
 * forc�ment les m�mes sur toutes les machines!!
 * V�rifiez le bien (avec sizeof) si vous travaillez en dehors de l'�cole*/

/* Type de donnee representant un octet (8 bits) */
typedef unsigned char BYTE;

/* Type de donnee representant un mot de 32 bits */
typedef unsigned int WORD;



/***----------------------------------------------------------------------***/
/***          CONSTANTES                                                  ***/
/***----------------------------------------------------------------------***/

/* Taille d'un segment de m�moire du simulateur (en nombre d'octets) */
#define TAILLESEGMENT 4096

/* Taille totale de la m�moire du simulateur (en nombre d'octets) */
/*     1Mo = 256 segments de 4096 octets                          */
#define TAILLEMEM TAILLESEGMENT*256


#endif
