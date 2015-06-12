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
	char 	address[MAXLENGTH];			// editor's address
	char 	abstract[5*MAXLENGTH];		// article's abstract
	char 	annote[MAXLENGTH];			// annotation
	char 	authors[MAXLENGTH];			// authors names
	char 	booktitle[MAXLENGTH]; 		// book's title
	int 	chapter;					// chapter number
	int		edition;					// book's edition
	char	editor[MAXLENGTH];			// editor

	int		year;

	int 	category;
	// 7 categories:
	// 1 - Books and proceedings (@book, @proceedings)
	// 2 - Thesis (@phdthesis)
	// 3 - Articles in journal or book's chapters (@inbook, @incollection, @article)
	// 4 - Conference's articles (@inproceedings)
	// 5 - Internal reports (@techreport)
	// 6 - Manuals, booklets (@booklet, @manual)
	// 7 - Miscellaneous (@mastersthesis, @audiovisual, @film, @misc, @unpublished)
};


//eprint : La spécification d'un publication électronique.
//howpublished : Comment il a été publié, si ce n'est pas avec une méthode standard.
//institution : L'institution impliquée dans la publication (pas forcément l'éditeur).
//journal : La revue ou le magazine dans lequel le travail a été publié.
//key : Un champ caché utilisé pour spécifier ou remplacer l'ordre alphabétique des entrées (quand "author et "editor" ne sont pas présents).
//month : Le mois de la création ou de la publication.
//note : Informations diverses.
//number : Le numéro du journal ou du magazine.
//organization : Le sponsor d'une conférence.
//pages : Les numéros de pages, séparés par des virgules ou sous forme d'intervalles.
//publisher : Le nom de la maison d'édition.
//school : L'école dans laquelle la thèse a été écrite.
//series : La collection dans laquelle la livre a été publié.
//title : Le titre du document.
//type : Le type.
//url : L'adresse URL.
//volume : Le volume, dans le cas où il y a plusieurs volumes.
//year : L'année de publication (ou de création s'il n'a pas été publié).

#endif /* ENTRY_H_ */
