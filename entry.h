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


//eprint : La sp�cification d'un publication �lectronique.
//howpublished : Comment il a �t� publi�, si ce n'est pas avec une m�thode standard.
//institution : L'institution impliqu�e dans la publication (pas forc�ment l'�diteur).
//journal : La revue ou le magazine dans lequel le travail a �t� publi�.
//key : Un champ cach� utilis� pour sp�cifier ou remplacer l'ordre alphab�tique des entr�es (quand "author et "editor" ne sont pas pr�sents).
//month : Le mois de la cr�ation ou de la publication.
//note : Informations diverses.
//number : Le num�ro du journal ou du magazine.
//organization : Le sponsor d'une conf�rence.
//pages : Les num�ros de pages, s�par�s par des virgules ou sous forme d'intervalles.
//publisher : Le nom de la maison d'�dition.
//school : L'�cole dans laquelle la th�se a �t� �crite.
//series : La collection dans laquelle la livre a �t� publi�.
//title : Le titre du document.
//type : Le type.
//url : L'adresse URL.
//volume : Le volume, dans le cas o� il y a plusieurs volumes.
//year : L'ann�e de publication (ou de cr�ation s'il n'a pas �t� publi�).

#endif /* ENTRY_H_ */
