/*
 * misc.c
 *
 *  Created on: 17 juil. 2015
 *      Author: Emmanuel
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "prototypes.h"

int compare_int_inc(void const *a, void const *b) {
   int const *pa = a;
   int const *pb = b;

   return *pa - *pb;
}

int compare_int_dec(void const *a, void const *b) {
   int const *pa = a;
   int const *pb = b;

   return *pb - *pa;
}

int compare_str(void const *a, void const *b) {
    const char **ia = (const char **)a;
    const char **ib = (const char **)b;
    return strcmp(*ia, *ib);
}

int compare_char( const void *a, const void *b) {
	return *(char*)a - *(char*)b;
}

void str_toupper(char* input) {
	int i = 0;
	while(input[i]) {
		input[i] = toupper(input[i]);
		i++;
	}
}

void get_category_str(char* output, int category_id) {
	if(category_id == 1) {
		sprintf(output, "Books and proceedings");
	} else if(category_id == 2) {
		sprintf(output, "Thesis");
	} else if(category_id == 3) {
		sprintf(output, "Articles in journal or book's chapters");
	} else if(category_id == 4) {
		sprintf(output, "Conference articles");
	} else if(category_id == 5) {
		sprintf(output, "Internal reports");
	} else if(category_id == 6) {
		sprintf(output, "Manuals, booklets");
	} else if(category_id == 7) {
		sprintf(output, "Miscellaneous");
	}
}

//TODO add "and" instead of ", " before last author's name
void flip_authors_names(char *flipped_names, const char *authors_names) {
	char *tmpstr = malloc(MAXLENGTH*sizeof(char));
	char *tmpLastname = malloc(MAXLENGTH*sizeof(char));

	strcpy(tmpstr, authors_names);
	flipped_names[0] = '\0';

	char *token = strtok(tmpstr, ",");
	while(token != NULL) {
		strcpy(tmpLastname, token);

		token = strtok(NULL, "., ");
		while(token != NULL && strcmp(token, "and") != 0) {
			strcat(flipped_names, token);
			strcat(flipped_names, ". ");
			token = strtok(NULL, ". ");
		}

		strcat(flipped_names, tmpLastname);
		if(token != NULL) {
			strcat(flipped_names, ", ");
		}

		token = strtok(NULL, ",");
	}

	free(tmpstr); free(tmpLastname);
}
