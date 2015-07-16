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
//eprint : La spécification d'un publication électronique.
//howpublished : Comment il a été publié, si ce n'est pas avec une méthode standard.
//institution : L'institution impliquée dans la publication (pas forcément l'éditeur).
//key : Un champ caché utilisé pour spécifier ou remplacer l'ordre alphabétique des entrées (quand "author et "editor" ne sont pas présents).
//organization : Le sponsor d'une conférence.

#endif /* ENTRY_H_ */
