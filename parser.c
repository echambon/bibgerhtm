/*
 * parser.c
 *
 *  Created on: 21 juin 2015
 *      Author: Emmanuel
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "prototypes.h"
#include "constants.h"
#include "entry.h"

//TODO define arrays of required fields for each type
//TODO review bib_parser!
//TODO manage maximum length for strings wrt considered field

/**
 * bib_parser
 * A (simple) BIBTEX file parser.
 */
int bib_parser(bibentry* bib_entries, int nb_entries, char* filename) {
	int bib_counter = -1;
	int reject_entry = 0;
	int line_count = 0;

	// Resetting previous logfile
	reset_logfile();

	char cur_line[20*MAXLENGTH];
	FILE* bib_file = NULL; bib_file = fopen(filename, "r");

	if(bib_file != NULL) {
		while (fgets(cur_line, 20*MAXLENGTH, bib_file) != NULL) {
			line_count += 1;
			// Check for character '@' which indicates a new entry
			if (strchr("@", *cur_line)) {
				reject_entry = 0; bib_counter += 1;

				// Getting category name
				char *category_name = strtok(cur_line, "@{");

				// Assigning category
				if((strcasecmp(category_name,"book") == 0) || (strcasecmp(category_name,"proceedings") == 0)) {
					bib_entries[bib_counter].category = 1;
				} else if(strcasecmp(category_name,"phdthesis") == 0) {
					bib_entries[bib_counter].category = 2;
				} else if((strcasecmp(category_name,"inbook") == 0) || (strcasecmp(category_name,"incollection") == 0) || (strcasecmp(category_name,"article") == 0)) {
					bib_entries[bib_counter].category = 3;
				} else if(strcasecmp(category_name,"inproceedings") == 0) {
					bib_entries[bib_counter].category = 4;
				} else if(strcasecmp(category_name,"techreport") == 0) {
					bib_entries[bib_counter].category = 5;
				} else if((strcasecmp(category_name,"booklet") == 0) || (strcasecmp(category_name,"manual") == 0)) {
					bib_entries[bib_counter].category = 6;
				} else if((strcasecmp(category_name,"mastersthesis") == 0) || (strcasecmp(category_name,"audiovisual") == 0) || (strcasecmp(category_name,"film") == 0) || (strcasecmp(category_name,"misc") == 0) || (strcasecmp(category_name,"unpublished") == 0)) {
					bib_entries[bib_counter].category = 7;
				} else {
					log_to_file(line_count, "invalid token @%s, entry rejected\n", category_name);
					reject_entry = 1;
				}

				// Write category name to entry if not rejected
				if(!reject_entry) {
					strcpy(bib_entries[bib_counter].type, category_name);
				} else { // otherwise, decrement counter to avoid invalid entries
					bib_counter -= 1;
				}
			} else {
				if(!reject_entry) {
					// Split string along "=", then "{" and "}"
					//TODO problem with token and abstract, the whole abstract is not obtained...
					//TODO problem with strings containing ":" and possibly other punctuation
					char *token = strtok(cur_line, "= {}\t\n");

					if(token) {
						// Getting field
						char *field = malloc(sizeof(char)*strlen(token));
						strcpy(field, token);

						// Getting value
						token = strtok(NULL, "=\n");
						if(token) {
							char *value = malloc(sizeof(char)*strlen(token));

							sscanf(token," {%[0-9a-zA-Z,.- ]},",value);
							if(value != NULL && field != NULL) {
								// Fill fields (that's hard to say!)
								if(strcasecmp(field,"year") == 0) { // YEAR
									char *end;
									int value_year = strtol(value, &end, 10);
									bib_entries[bib_counter].year = value_year;
								} else if(strcasecmp(field,"author") == 0) { // AUTHOR
									strcpy(bib_entries[bib_counter].authors,value);
								} else if(strcasecmp(field,"title") == 0) { // TITLE
									strcpy(bib_entries[bib_counter].title,value);
								} else if(strcasecmp(field,"abstract") == 0) { // ABSTRACT
									strcpy(bib_entries[bib_counter].abstract,value);
								} else if(strcasecmp(field,"keywords") == 0) { // KEYWORDS
									strcpy(bib_entries[bib_counter].keywords,value);
								} else if(strcasecmp(field,"booktitle") == 0) { // BOOKTITLE
									strcpy(bib_entries[bib_counter].booktitle,value);
								} else if(strcasecmp(field,"pages") == 0) { // PAGES
									strcpy(bib_entries[bib_counter].pages,value);
								} else if(strcasecmp(field,"journal") == 0) { // JOURNAL
									strcpy(bib_entries[bib_counter].journal,value);
								} else if(strcasecmp(field,"note") == 0) { // NOTE
									strcpy(bib_entries[bib_counter].note,value);
								} else if(strcasecmp(field,"volume") == 0) { // VOLUME
									char *end;
									int value_volume = strtol(value, &end, 10);
									bib_entries[bib_counter].volume = value_volume;
								} else if(strcasecmp(field,"number") == 0) { // NUMBER
									char *end;
									int value_number = strtol(value, &end, 10);
									bib_entries[bib_counter].number = value_number;
								} else if(strcasecmp(field,"publisher") == 0) { // PUBLISHER
									strcpy(bib_entries[bib_counter].publisher,value);
								} else if(strcasecmp(field,"school") == 0) { // PUBLISHER
									strcpy(bib_entries[bib_counter].school,value);
								} else if(strcasecmp(field,"url") == 0) { // PUBLISHER
									strcpy(bib_entries[bib_counter].url,value);
								} else if(strcasecmp(field,"address") == 0) { // PUBLISHER
									strcpy(bib_entries[bib_counter].address,value);
								} else if(strcasecmp(field,"isbn") == 0) { // PUBLISHER
									strcpy(bib_entries[bib_counter].isbn,value);
								}
							}

							free(value);
						}

						free(field);
					}
				}
			}
		}
	}

	fclose(bib_file);
	return bib_counter+1;
}

/**
 * bib_entries
 * Counts the number of entries in a bib file. It basically detects how many times the character '@' appear.
 */
int bib_entries(char* filename) {
	int res = 0; char cur_line[MAXLENGTH];
	FILE* bib_file = NULL;
	bib_file = fopen(filename, "r");

	if(bib_file != NULL) {
		// Count number of characters @
		while (fgets(cur_line, MAXLENGTH, bib_file) != NULL) {
			if (strchr("@", *cur_line)) {
				res += 1;
			}
		}
	}

	fclose(bib_file);
	return res;
}
