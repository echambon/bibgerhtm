/*
 * entry.h
 *
 *  Created on: 10 juin 2015
 *      Author: Emmanuel
 */

#ifndef ENTRY_H_
#define ENTRY_H_

#include "constants.h"

typedef struct bibentry bibentry;

struct bibentry
{
	char 	annote[MAXLENGTH];			// annotation
	int 	chapter;					// chapter number
	char	edition[MAXLENGTH];			// book's edition
	char	editor[MAXLENGTH];			// editor
	char	month[MAXLENGTH];			// 3 letters month identifier
	char 	series[MAXLENGTH];
	char 	url[5*MAXLENGTH];			// document's path on computer?

	char 	address[MAXLENGTH];			// editor's address
	int		year;
	char 	authors[MAXLENGTH];			// authors names
	char 	title[5*MAXLENGTH];
	char 	keywords[10*MAXLENGTH];		// keywords
	char 	abstract[20*MAXLENGTH];		// article's abstract
	char 	pages[MAXLENGTH];			//
	char 	booktitle[MAXLENGTH]; 		// book's title
	char 	journal[MAXLENGTH];			//
	char 	note[5*MAXLENGTH];			// various information
	int 	volume;
	int 	number;						// journal number
	char 	publisher[MAXLENGTH];
	char 	school[MAXLENGTH];			// (for thesis)
	char	type[MAXLENGTH];
	char 	isbn[MAXLENGTH];

	int 	category;
	// 7 categories:
	// 1 - Books and proceedings (@book, @proceedings)
	// 2 - Thesis (@phdthesis)
	// 3 - Articles in journal or book's chapters (@inbook, @incollection, @article)
	// 4 - Conference articles (@inproceedings)
	// 5 - Internal reports (@techreport)
	// 6 - Manuals, booklets (@booklet, @manual)
	// 7 - Miscellaneous (@mastersthesis, @audiovisual, @film, @misc, @unpublished)
};

// NOT IMPLEMENTED:
//eprint : La sp�cification d'un publication �lectronique.
//howpublished : Comment il a �t� publi�, si ce n'est pas avec une m�thode standard.
//institution : L'institution impliqu�e dans la publication (pas forc�ment l'�diteur).
//key : Un champ cach� utilis� pour sp�cifier ou remplacer l'ordre alphab�tique des entr�es (quand "author et "editor" ne sont pas pr�sents).
//organization : Le sponsor d'une conf�rence.

#endif /* ENTRY_H_ */
