/*
 * main.c
 *
 *  Created on: 10 juin 2015
 *      Author: echambon
 */
// Personal adaptation of http://www-sop.inria.fr/members/Gregoire.Malandain/codes/bibtex2html.html
// For fun only!
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "constants.h"
#include "entry.h"

void bib_parser(bibentry*, int, char*);
int bib_entries(char*);

void html_write_header(FILE*, char*, char*);
void html_write_footer(FILE*);
void html_write_index(char*, bibentry*, int, int*, int*, char*, char**);
void html_write(FILE*, char*);

static int compare_int_inc(void const*, void const*);
static int compare_int_dec(void const*, void const*);

//TODO create folders (output+subdirectories)
//TODO use tables to organize content

int main()
{
	// Counters
	int i,j,k;

	// Welcome message
	printf("bibgerhtm V%d\n###############################\n",VERSION);

	// Loading project files
	printf("Loading project files...\n");
	FILE *properties_file = NULL, *input_file = NULL;
	char file_reader[MAXLENGTH];

	properties_file = fopen(PROPERTIES_FILENAME, "r");
	input_file 		= fopen(INPUT_FILENAME, "r");

	// Project variables
	char project_title[MAXLENGTH];
	char author_name[MAXLENGTH];
	char author_name_article[MAXLENGTH]; char author_name_article2[MAXLENGTH];

	// Parsing PROPERTIES.TXT
	if(properties_file != NULL) {
		// Read first line of properties file
		fgets(file_reader, MAXLENGTH, properties_file);
		sprintf(project_title, file_reader); // it seems a \n is added at the end?

		// Read second line of properties file
		fgets(file_reader, MAXLENGTH, properties_file);
		sprintf(author_name, file_reader);

		// Read third line
		fgets(file_reader, MAXLENGTH, properties_file);
		sprintf(author_name_article, file_reader);
		sprintf(author_name_article2, file_reader);
		strcat(author_name_article2," ");

		// Message
		printf("\n%s loaded successfully!\n", PROPERTIES_FILENAME);
	} else {
		// Default values
		printf("\n%s not found!\nProject title set to: %s\nAuthor name set to: %s\n", PROPERTIES_FILENAME, PROJECT_TITLE_DFLT, AUTHOR_NAME_DFLT);
		sprintf(project_title, PROJECT_TITLE_DFLT);
	}

	// Closing project files
	fclose(properties_file); fclose(input_file);

	//// Parsing INPUT.BIB file and allocating variables
	// Parse a first time to count number of entries (use @ char)
	int nb_entries = 0;
	nb_entries = bib_entries(INPUT_FILENAME);

	// Writing number of counted entries
	printf("%d entries detected in %s\n", nb_entries, INPUT_FILENAME);

	// Initializing array of struct
	//printf("%d\n", sizeof(bibentry));
	bibentry* bib_entries = NULL;
	bib_entries = malloc(nb_entries * sizeof(bibentry));

	// Loading in array of struct
	bib_parser(bib_entries, nb_entries, INPUT_FILENAME);

	// Message
	printf("%s loaded successfully!\n\nWriting files:\n", INPUT_FILENAME);

	// Everything is loaded now!

	// Available Years
	int *array_years = malloc(nb_entries*sizeof(int)); // supposing all entries have a different year
	for(i=0; i<nb_entries; i++) { array_years[i] = 0; }
	int year_ispresent;
	for(i=0; i<nb_entries; i++) {
		year_ispresent = 0;
		for(j=0; j<nb_entries; j++) {
			if(array_years[j] == bib_entries[i].year) {
				year_ispresent = 1;
			}
		}
		if(!year_ispresent) {
			j = 0;
			while(array_years[j] != 0) {
				j += 1;
			}
			array_years[j] = bib_entries[i].year;
		}
	}

	// array_years ordering
	qsort(array_years, j+1, sizeof(int), compare_int_dec);

	// Available categories
	int *array_categories = malloc(CATEGORIES_NB*sizeof(int)); // there are 7 categories!
	for(i=0; i<CATEGORIES_NB; i++) { array_categories[i] = 0; }
	int category_ispresent;
	for(i=0; i<nb_entries; i++) {
		category_ispresent = 0;
		for(j=0; j<CATEGORIES_NB; j++) {
			if(array_categories[j] == bib_entries[i].category) {
				category_ispresent = 1;
			}
		}
		if(!category_ispresent) {
			j = 0;
			while(array_categories[j] != 0) {
				j += 1;
			}
			array_categories[j] = bib_entries[i].category;
		}
	}

	// array_categories ordering
	qsort(array_categories, j+1, sizeof(int), compare_int_inc);

	// Available co-authors
	char **array_coauthors = malloc(MAX_COAUTHORS*nb_entries*sizeof(char*));
	char coauthors_initials[26];

	// Initialize arrays
	for(i=0;i<26;i++) { coauthors_initials[i] = 0; }
	for(i=0;i<MAX_COAUTHORS*nb_entries;i++) { array_coauthors[i] = malloc(MAXLENGTH*sizeof(char)); array_coauthors[i][0] = '\0'; }

	char initial;
	char entry_authors[MAXLENGTH];
	char current_author[MAXLENGTH]; current_author[0] = '\0';
	int coauthor_ispresent; int initial_ispresent;

	// PROBLEM WITH FIRST AUTHOR SOMETIMES ?
	for(i=0; i<nb_entries; i++) {
		coauthor_ispresent = 0; initial_ispresent = 0;
		strcpy(entry_authors, bib_entries[i].authors);

		// Split along " "
		char *token = strtok(entry_authors, " ");
		if(token != NULL) {
			do {
				if(strcmp(token,"and") != 0) {
					strcat(current_author, token);
					strcat(current_author, " ");
				} else {
					// Detect if author is not the user
					if((strcmp(current_author, author_name_article) != 0) && (strcmp(current_author, author_name_article2) != 0)) {
						// Detect if author already present in database (array_coauthors)
						for(j=0;j<MAX_COAUTHORS*nb_entries;j++) {
							if(strcmp(current_author, array_coauthors[j]) == 0) {
								coauthor_ispresent = 1;
							}
						}

						// If not, add user
						if(!coauthor_ispresent) {
							k = 0;
							while(array_coauthors[k][0] != '\0') {
								k += 1;
							}
							strcpy(array_coauthors[k],current_author);
						}

						// Check if initial already in database and add if not
						initial = current_author[0];
						j = 0;
						while((coauthors_initials[j] != 0) && j<26) {
							if(coauthors_initials[j] == initial) {
								initial_ispresent = 1;
							}
							j += 1;
						}
						if(!initial_ispresent) {
							coauthors_initials[j] = initial;
						}
					}

					current_author[0] = '\0';
				}
				token = strtok(NULL, " ");

				// Detect last author (or unique author)
				coauthor_ispresent = 0;
				if(token == NULL) {
					if((strcmp(current_author, author_name_article) != 0) && (strcmp(current_author, author_name_article2) != 0)) {
						// Detect if author already present in database (array_coauthors)
						for(j=0;j<MAX_COAUTHORS*nb_entries;j++) {
							if(strcmp(current_author, array_coauthors[j]) == 0) {
								coauthor_ispresent = 1;
							}
						}

						// If not, add user
						if(!coauthor_ispresent) {k = 0;
							while(array_coauthors[k][0] != '\0') {
							k += 1;
							}
							strcpy(array_coauthors[k],current_author);
						}

						// Check if initial already in database and add if not
						initial = current_author[0];
						j = 0;
						while((coauthors_initials[j] != 0) && j<26) {
							if(coauthors_initials[j] == initial) {
								initial_ispresent = 1;
							}
							j += 1;
						}
						if(!initial_ispresent) {
							coauthors_initials[j] = initial;
						}
					}

					current_author[0] = '\0';
				}
			} while(token != NULL);
		}

		current_author[0] = '\0';
	}

	//// Creating empty CSS file
	FILE* css_empty_file = NULL;
	css_empty_file = fopen("OUTPUT/style.css", "w");
	fclose(css_empty_file);
	printf("[X] style.css\n");

	//// Creating HTML pages
	//FILE *html_page = NULL;
	//char page_title[MAXLENGTH];

	// Index page as OUTPUT/index.html
	html_write_index(author_name, bib_entries, nb_entries, array_years, array_categories, coauthors_initials, array_coauthors);

	// Year pages

	// Copying INPUT.BIB to OUTPUT/biblio/complete-bibliography.bib

	// Debugging

	// Freeing memory
	free(bib_entries);
	free(array_years);
	free(array_categories);
	for(i=0;i<MAX_COAUTHORS*nb_entries;i++) { free(array_coauthors[i]); }
	free(array_coauthors);

	// Pausing and exiting
	getchar();
	printf("Exiting...");
	return 0;
}

void html_write_header(FILE* filename, char* title, char* author_name) {
	fputs("<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n\t<meta charset=\"utf-8\">\n", filename);
	fprintf(filename, "\t<meta name=\"keywords\" lang=\"en\" content=\"bibgerhtm, bibliography, article, report, %s\">\n", author_name);
	fprintf(filename, "\t<meta name=\"generator\" content=\"bibgerhtm v%d\">", VERSION);
	fputs("\n\t<link href=\"style.css\" rel=\"stylesheet\" type=\"text/css\">\n", filename);
	//<META name="GENERATOR" content="bibtex2html 1.01">
	fprintf(filename, "\t<title>%s</title>\n", title);
	fputs("</head>\n<body>\n\t<center>\n", filename);
}

void html_write_footer(FILE* filename) {
	// Time management
	time_t secondes;
	struct tm instant;
	time(&secondes);
	instant=*localtime(&secondes);

	fputs("\n\t</center>\n\t<br/><br/>\n\t<hr>\n\tLast updated: ",filename);
	fprintf(filename, "<i>%02d/%02d/%d - %02d:%02d</i>\n", instant.tm_year+1900, instant.tm_mon+1, instant.tm_mday, instant.tm_hour, instant.tm_min);
	fputs("\t<hr>\n\tGenerated from BibT<sub>E</sub>X by <a href=\"http://echambon.contact.free.fr/en/misc.php\" target=\"blank\"><em>bibgerhtm</em></a>, itself inspired by <a href=\"http://www-sop.inria.fr/epidaure/personnel/malandain/codes/bibtex2html.html\" target=\"blank\"><em>bibtex2html</em></a>.\n</body>\n</html>", filename);
}

void html_write(FILE* filename, char* content) {
	fprintf(filename, "%s", content);
}

void html_write_index(char* author_name, bibentry* bib_entries, int nb_entries, int *array_years, int *array_categories, char *coauthors_initials, char **array_coauthors) {
	FILE *html_page = NULL;
	char page_title[MAXLENGTH];
	char temp_str[MAXLENGTH];

	// Counters
	int i,j,k;

	// Creating index page
	html_page = fopen("OUTPUT/index.html", "w");
	sprintf(page_title, "List of Publications of %s: Index", author_name);

	// Header
	html_write_header(html_page, page_title, author_name);

	// Body
	//// H1 + H2 - Year
	sprintf(temp_str, "\t\t<h1>%s</h1>\n\t\t<h2>Selection by year</h2>", page_title);
	html_write(html_page, temp_str);

	j = 0;
	fprintf(html_page, "\n\t\t\t<table style=\"text-align:center;width:%dpx\">\n\t\t\t\t<tr>", YEAR_COLUMNS*YEAR_COLUMN_WIDTH_PX);
	for(i=0; i<nb_entries; i++) {
		if(array_years[i] != 0) {
			sprintf(temp_str, "\n\t\t\t\t\t<td><a href=\"year/%d.html\">%d</a></td>", array_years[i], array_years[i]);
			html_write(html_page, temp_str);
			j += 1;
			if(j>=YEAR_COLUMNS) {
				fputs("\n\t\t\t\t</tr>\n\t\t\t\t<tr>", html_page);
				j = 0;
			}
		}
		temp_str[0] = '\0';
	}
	fputs("\n\t\t\t\t</tr>\n\t\t\t</table>", html_page);

	// H2 - Category
	html_write(html_page,"\n\t\t<h2>Selection by category</h2>");

	j = 0;
	fprintf(html_page, "\n\t\t\t<table style=\"text-align:left;width:%dpx\">\n\t\t\t\t<tr>", CATEGORY_COLUMNS*CATEGORY_COLUMN_WIDTH_PX);
	for(i=0; i<CATEGORIES_NB; i++) {
		if(array_categories[i] != 0) {
			if(array_categories[i] == 1) {
				sprintf(temp_str, "\n\t\t\t\t\t<td><a href=\"category/books.html\">Books and proceedings</a></td>");
			} else if(array_categories[i] == 2) {
				sprintf(temp_str, "\n\t\t\t\t\t<td><a href=\"category/thesis.html\">Thesis</a> ");
			} else if(array_categories[i] == 3) {
				sprintf(temp_str, "\n\t\t\t\t\t<td><a href=\"category/articles.html\">Articles in journal or book's chapters</a></td>");
			} else if(array_categories[i] == 4) {
				sprintf(temp_str, "\n\t\t\t\t\t<td><a href=\"category/conferences.html\">Conference's articles</a></td>");
			} else if(array_categories[i] == 5) {
				sprintf(temp_str, "\n\t\t\t\t\t<td><a href=\"category/reports.html\">Internal reports</a></td>");
			} else if(array_categories[i] == 6) {
				sprintf(temp_str, "\n\t\t\t\t\t<td><a href=\"category/manuals.html\">Manuals, booklets</a></td>");
			} else if(array_categories[i] == 7) {
				sprintf(temp_str, "\n\t\t\t\t\t<td><a href=\"category/misc.html\">Miscellaneous</a></td>");
			}

			html_write(html_page, temp_str);
			j += 1;

			if(j>=CATEGORY_COLUMNS && i<CATEGORIES_NB-2) {
				fputs("\n\t\t\t\t</tr>\n\t\t\t\t<tr>", html_page);
				j = 0;
			}
		}
		temp_str[0] = '\0';
	}
	fputs("\n\t\t\t\t</tr>\n\t\t\t</table>", html_page);

	// H2 - Authors list
	html_write(html_page,"\n\t\t<h2>Selection by author</h2>");

	// initials
	fputs("\n\t\t\t<table style=\"text-align:center\">\n\t\t\t\t<tr>", html_page);
	int initial_ispresent = 0;
	for(i=0;i<26;i++) {
		initial_ispresent = 0;

		//coauthors_initials
		if(i==13) { fputs("\n\t\t\t\t<tr>", html_page); }
		for(j=0;j<26;j++) {
			if(coauthors_initials[j] == 65+i) {
				initial_ispresent = 1;
			}
		}
		if(initial_ispresent) {
			fprintf(html_page, "\n\t\t\t\t\t<td><a href=\"#AUTH%c\">%c</a></td>", 65+i, 65+i);
		} else {
			fprintf(html_page, "\n\t\t\t\t\t<td>%c</td>", 65+i);
		}
		if(i==12) { fputs("\n\t\t\t\t</tr>", html_page); }
	}
	fputs("\n\t\t\t\t</tr>\n\t\t\t</table><br/>", html_page);

	// authors list
	fprintf(html_page, "\n\t\t\t<table style=\"width:%dpx\">\n\t\t\t\t<tr>", AUTHORS_COLUMN_WIDTH_PX*3+10); // style=\"text-align:center\"

	// TODO add links to authors pages
	// For each initial in char* coauthors_initials, find authors will this initial in char** array_coauthors
	for(i=0;i<26;i++) {
		// If there is actually something stored in the array ...
		if(coauthors_initials[i] != 0) {
			fprintf(html_page, "\n\t\t\t\t\t<td width=\"10%%\"><a href=\"#AUTH%c\"></a>%c</td>", coauthors_initials[i], coauthors_initials[i]);

			// Look for authors with the same initial in array_coauthors
			k = 0;
			for(j=0;j<MAX_COAUTHORS*nb_entries;j++) {
				if(array_coauthors[j][0] == coauthors_initials[i]) {
					fprintf(html_page, "\n\t\t\t\t\t<td>%s</td>", array_coauthors[j]);
					k += 1;
					if(k == 2) {
						fputs("\n\t\t\t\t</tr>\n\t\t\t\t<tr>", html_page);
						k = 0;
					}
				}
			}

			fputs("\n\t\t\t\t</tr>\n\t\t\t\t<tr>", html_page);
		}
	}

	fputs("\n\t\t\t\t</tr>\n\t\t\t</table>", html_page);

	// H2 - Keyword
	html_write(html_page,"\n\t\t<h2>Selection by keyword</h2>");

	// H2 - Bibliography
	html_write(html_page,"\n\t\t<h2>Complete bibliography</h2>\n\t\t\t<a href=\"biblio/complete-bibliography.html\">Complete bibliography as a single HTML page</a>\n\t\t\t<br/><a href=\"biblio/complete-bibliography.bib\">Complete bibliography as a single BIBTEX file</a>");

	// Footer
	html_write_footer(html_page);

	// Closing file
	fclose(html_page);

	// Message
	printf("[X] index.html\n");
}

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

void bib_parser(bibentry* bib_entries, int nb_entries, char* filename) {
	int bib_counter = -1;
	//int opened = 0;
	char cur_line[5*MAXLENGTH];
	FILE* bib_file = NULL; bib_file = fopen(filename, "r");
	//bibentry* bib_entries = malloc(nb_entries*sizeof(bibentry));

	if(bib_file != NULL) {
		while (fgets(cur_line, MAXLENGTH, bib_file) != NULL) {
			// Check for character '@' which indicates a new entry
			if (strchr("@", *cur_line)) {
				bib_counter += 1;

				// Getting category name
				char *category_name = strtok(cur_line, "@{");
				//printf("%s\n",category_name);
				// Assigning category
				if((strcmp(category_name,"book") == 0) || (strcmp(category_name,"proceedings") == 0)) {
					bib_entries[bib_counter].category = 1;
				} else if(strcmp(category_name,"phdthesis") == 0) {
					bib_entries[bib_counter].category = 2;
				} else if((strcmp(category_name,"inbook") == 0) || (strcmp(category_name,"incollection") == 0) || (strcmp(category_name,"article") == 0)) {
					bib_entries[bib_counter].category = 3;
				} else if(strcmp(category_name,"inproceedings") == 0) {
					bib_entries[bib_counter].category = 4;
				} else if(strcmp(category_name,"techreport") == 0) {
					bib_entries[bib_counter].category = 5;
				} else if((strcmp(category_name,"booklet") == 0) || (strcmp(category_name,"manual") == 0)) {
					bib_entries[bib_counter].category = 6;
				} else if((strcmp(category_name,"mastersthesis") == 0) || (strcmp(category_name,"audiovisual") == 0) || (strcmp(category_name,"film") == 0) || (strcmp(category_name,"misc") == 0) || (strcmp(category_name,"unpublished") == 0)) {
					bib_entries[bib_counter].category = 7;
				}
			} else {
				// Split string along "=", then "{" and "}"
				// http://en.cppreference.com/w/c/string/byte/strtok
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
							//printf("|%s|%s|\n",field,value);

							// Fill fields (that's hard to say!)
							if(strcmp(field,"year") == 0) {
								char *end;
								int value_year = strtol(value, &end, 10);
								bib_entries[bib_counter].year = value_year;
							} else if(strcmp(field,"author") == 0) {
								strcpy(bib_entries[bib_counter].authors,value);
							}
						}

						free(value);
					}

					free(field);
				}
			}
		}
	}

	fclose(bib_file);
	//return bib_entries;
}

static int compare_int_inc(void const *a, void const *b) {
   /* definir des pointeurs type's et initialise's
      avec les parametres */
   int const *pa = a;
   int const *pb = b;

   /* evaluer et retourner l'etat de l'evaluation (tri croissant) */
   return *pa - *pb;
}

static int compare_int_dec(void const *a, void const *b) {
   /* definir des pointeurs type's et initialise's
      avec les parametres */
   int const *pa = a;
   int const *pb = b;

   /* evaluer et retourner l'etat de l'evaluation (tri croissant) */
   return *pb - *pa;
}
