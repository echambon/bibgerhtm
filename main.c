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
#include <ctype.h>

#include "prototypes.h"
#include "constants.h"
#include "entry.h"

//TODO create folders (output+subdirectories)
//TODO url field management (pdf location on user's disk) --> copy pdf to OUTPUT/pdfdir
//TODO apply flip_authors_names to editors field

int main() {
	// Counters
	int i,j,k;
	int valid_entries;

	// Welcome message
	printf("bibgerhtm build-%d\n#######################################\n",VERSION);

	// Loading project files
	printf("Loading project files...\n");
	FILE *properties_file = NULL;
	char file_reader[MAXLENGTH];

	properties_file = fopen(PROPERTIES_FILENAME, "r");

	// Project variables
	char project_title[MAXLENGTH];
	char author_name[MAXLENGTH];
	char author_name_article[MAXLENGTH]; char author_name_article2[MAXLENGTH];

	// Parsing PROPERTIES.TXT
	if(properties_file != NULL) {
		// Read first line of properties file
		fgets(file_reader, MAXLENGTH, properties_file);
		sprintf(project_title, file_reader);

		// Read second line of properties file
		fgets(file_reader, MAXLENGTH, properties_file);
		sprintf(author_name, file_reader);
		char *author_name_noline = strtok(author_name,"\n");
		strcpy(author_name, author_name_noline);

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
	fclose(properties_file);

	//// Parsing INPUT.BIB file and allocating variables
	// Parse a first time to count number of entries (use @ char)
	int nb_entries = 0;
	nb_entries = bib_entries(INPUT_FILENAME);

	// Initializing array of struct
	bibentry* bib_entries = NULL;
	bib_entries = malloc(nb_entries * sizeof(bibentry));

	// Loading in array of struct
	valid_entries = bib_parser(bib_entries, nb_entries, INPUT_FILENAME);

	// Messages
	printf("%d/%d valid entries detected in %s\n", valid_entries, nb_entries, INPUT_FILENAME);
	printf("%s loaded successfully!\n\nWriting files:\n", INPUT_FILENAME);

	// Everything is loaded now!

	// Available Years
	int *array_years = malloc(valid_entries*sizeof(int)); // supposing all entries have a different year
	for(i=0; i<valid_entries; i++) { array_years[i] = 0; }
	int year_ispresent;
	for(i=0; i<valid_entries; i++) {
		year_ispresent = 0;
		for(j=0; j<valid_entries; j++) {
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
	for(i=0; i<valid_entries; i++) {
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
	char **array_coauthors = malloc(MAX_COAUTHORS*valid_entries*sizeof(char*));
	char coauthors_initials[26];

	// Initialize arrays
	for(i=0;i<26;i++) { coauthors_initials[i] = 0; }
	for(i=0;i<MAX_COAUTHORS*valid_entries;i++) { array_coauthors[i] = malloc(MAXLENGTH*sizeof(char)); array_coauthors[i][0] = '\0'; }

	char initial;
	char entry_authors[MAXLENGTH];
	char current_author[MAXLENGTH]; current_author[0] = '\0';
	int coauthor_ispresent; int initial_ispresent;

	// PROBLEM WITH FIRST AUTHOR SOMETIMES ?
	for(i=0; i<valid_entries; i++) {
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
						for(j=0;j<MAX_COAUTHORS*valid_entries;j++) {
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
				coauthor_ispresent = 0; initial_ispresent = 0;
				if(token == NULL) {
					if((strcmp(current_author, author_name_article) != 0) && (strcmp(current_author, author_name_article2) != 0)) {
						// Detect if author already present in database (array_coauthors)
						for(j=0;j<MAX_COAUTHORS*valid_entries;j++) {
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
			} while(token != NULL);
		}
		current_author[0] = '\0';
	}

	// coauthors_initials ordering
	qsort(coauthors_initials, 26, sizeof(char), compare_char);

	// array_coauthors ordering
	qsort(array_coauthors, MAX_COAUTHORS*valid_entries, sizeof(char*), compare_str);

	//// Creating empty CSS file
	FILE* css_empty_file = NULL;
	css_empty_file = fopen("OUTPUT/style.css", "w");
	fclose(css_empty_file);
	printf("[X] style.css\n");

	//// Creating HTML pages
	//FILE *html_page = NULL;
	//char page_title[MAXLENGTH];

	// Index page as OUTPUT/index.html
	html_write_index(author_name, bib_entries, valid_entries, array_years, array_categories, coauthors_initials, array_coauthors);

	// Year pages (array_years) in OUTPUT/year/
	for(i=0;i<valid_entries;i++) {
		if(array_years[i] != 0) {
			html_write_year_page(array_years[i], valid_entries, bib_entries, author_name);
		}
	}

	// Copying INPUT.BIB to OUTPUT/biblio/complete-bibliography.bib

	// Debugging

	// End message
	printf("HTML pages generation is successful!\n");

	// Freeing memory
	free(bib_entries);
	free(array_years);
	free(array_categories);
	for(i=0;i<MAX_COAUTHORS*valid_entries;i++) { free(array_coauthors[i]); }
	free(array_coauthors);

	// Pausing and exiting
	getchar();
	printf("Exiting...");
	return 0;
}

void html_write_header(FILE* filename, char* title, char* author_name) {
	fputs("<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n\t<meta charset=\"utf-8\">\n", filename);
	fprintf(filename, "\t<meta name=\"keywords\" lang=\"en\" content=\"bibgerhtm, bibliography, article, report, %s\">\n", author_name);
	fprintf(filename, "\t<meta name=\"generator\" content=\"bibgerhtm b%d\">", VERSION);
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

	fputs("\n\t</center>\n\t<hr>\n\t<u><b>Disclaimer:</b></u>",filename);
	fputs("\n\t<br/><br/><i>This material is presented to ensure timely dissemination of scholarly and technical work. Copyright and all rights therein are retained by authors or by other copyright holders.</i>",filename);
	fputs("\n\t<hr>\n\tLast updated: ",filename);
	fprintf(filename, "<i>%02d/%02d/%d - %02d:%02d</i>\n", instant.tm_year+1900, instant.tm_mon+1, instant.tm_mday, instant.tm_hour, instant.tm_min);
	fputs("\t<hr>\n\tGenerated from <i>BibT<sub>E</sub>X</i> by <a href=\"http://echambon.contact.free.fr/en/bibgerhtm.php\" target=\"blank\"><em>bibgerhtm</em></a>, itself inspired by <a href=\"http://www-sop.inria.fr/epidaure/personnel/malandain/codes/bibtex2html.html\" target=\"blank\"><em>bibtex2html</em></a>.\n</body>\n</html>", filename);
}

void html_write(FILE* filename, char* content) {
	fprintf(filename, "%s", content);
}

void html_write_index(char* author_name, bibentry* bib_entries, int nb_entries, int *array_years, int *array_categories, char *coauthors_initials, char **array_coauthors) {
	FILE *html_page = NULL;
	char page_title[MAXLENGTH];
	char temp_str[MAXLENGTH];
	char *token;
	char firstname[MAXLENGTH], upcase_surname[MAXLENGTH], surname[MAXLENGTH], firstname_initials[MAXLENGTH];

	// Counters
	int i,j,k,l;

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

	// For each initial in char* coauthors_initials, find authors will this initial in char** array_coauthors
	for(i=0;i<26;i++) {
		// If there is actually something stored in the array ...
		if(coauthors_initials[i] != 0) {
			fprintf(html_page, "\n\t\t\t\t\t<td width=\"10%%\"><a name=\"#AUTH%c\"></a>%c</td>", coauthors_initials[i], coauthors_initials[i]);

			// Look for authors with the same initial in array_coauthors
			k = 0;
			for(j=0;j<MAX_COAUTHORS*nb_entries;j++) {
				if(array_coauthors[j][0] == coauthors_initials[i]) {
					// Initializations
					firstname[0] = '\0';
					strcpy(surname, "");
					strcpy(firstname_initials,"");
					l = 0;

					// Getting surname!
					token = strtok(array_coauthors[j],",");
					strcpy(surname, token);

					// Getting surname!
					token = strtok(NULL,". ");
					while(token != NULL) {
						firstname_initials[l] = token[0]; l += 1;
						strcat(firstname, token);
						strcat(firstname, ". ");
						token = strtok(NULL,". ");
					}

					// Upper-case surname
					strcpy(upcase_surname,surname);
					str_toupper(upcase_surname);

					// First name (initials to appear in the link)
					char *upcase_firstname = (char*)malloc(l*sizeof(char));
					strncpy(upcase_firstname,firstname_initials,l);
					upcase_firstname[l] = '\0';

					fprintf(html_page, "\n\t\t\t\t\t<td width=\"30%%\"><a href=\"%s-%s.html\"><b>%s</b>, %s</a></td>", upcase_surname, upcase_firstname, surname, firstname);

					free(upcase_firstname);

					k += 1;
					if(k == 3) {
						fputs("\n\t\t\t\t</tr>\n\t\t\t\t<tr>\n\t\t\t\t\t<td></td>", html_page);
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
	fputs("\n\t\t\t<br/><br/>",html_page);

	// Footer
	html_write_footer(html_page);

	// Closing file
	fclose(html_page);

	// Message
	printf("[X] index.html\n");
}

void html_write_year_page(int year, int nb_entries, bibentry* bib_entries, char *author_name) {
	FILE *html_page = NULL;
	char filename[MAXLENGTH];
	char page_title[MAXLENGTH];
	char temp_str[MAXLENGTH];
	int i,j;
	int categories_this_year[CATEGORIES_NB] = {0,0,0,0,0,0,0};

	// Getting filename
	sprintf(filename, "OUTPUT/year/%d.html", year);

	// Creating year page
	html_page = fopen(filename, "w");

	// Titling page
	sprintf(page_title, "Publications by %s in year %d", author_name, year);

	// Writing header
	html_write_header(html_page, page_title, author_name);;

	// Back to index link
	sprintf(temp_str, "\t\t</center><a href=\"../index.html\"><b>BACK TO INDEX</b></a><center>");
	html_write(html_page, temp_str);

	// Body
	//// H1
	sprintf(temp_str, "\n\t\t<h1>%s</h1>", page_title);
	html_write(html_page, temp_str);

	// detect entries corresponding to this year
	for(i=0;i<nb_entries;i++) {
		if(bib_entries[i].year == year) {
			// store available categories
			categories_this_year[bib_entries[i].category-1] = 1;
		}
	}

	// TODO
	//// H2 (categories)
	for(i=0;i<CATEGORIES_NB;i++) {
		if(categories_this_year[i] == 1) {
			char *category_name = malloc(MAXLENGTH*sizeof(char));
			get_category_str(category_name, i+1);
			sprintf(temp_str, "\n\t\t<h2>%s</h2>", category_name);
			html_write(html_page, temp_str);

			// starting an ordered list
			fputs("\n\t\t\t</center><ol>", html_page);

			for(j=0;j<nb_entries;j++) {
				if((bib_entries[j].category == i+1) && (bib_entries[j].year == year)) {
					fputs("\n\t\t\t\t<li>\n\t\t\t\t\t", html_page);
					html_write_entry(html_page, bib_entries[j]);
					fputs("\n\t\t\t\t</li>", html_page);
				}
			}

			// end of the ordered list
			fputs("\n\t\t\t</ol><center>", html_page);

			// garbage collector
			free(category_name);
		}
	}

	// Back to index link (bottom page)
	sprintf(temp_str, "\n\t\t<br/><br/>\n\t\t</center><a href=\"../index.html\"><b>BACK TO INDEX</b></a><center>");
	html_write(html_page, temp_str);

	// Writing footer
	html_write_footer(html_page);

	// Closing file
	fclose(html_page);

	printf("[X] %d.html\n", year);
}

void html_write_entry(FILE* html_page, bibentry entry) {
	// Author's and editors names: print initials then lastname
	char *flipped_authors = malloc(MAXLENGTH*sizeof(char));
	char *flipped_editors = malloc(MAXLENGTH*sizeof(char));

	if(entry.authors != NULL) {
		flip_authors_names(flipped_authors, entry.authors);
	}
	if(entry.editor != NULL) {
		flip_authors_names(flipped_editors, entry.editor);
	}

	printf("%s\n", flipped_authors);

	// Books and proceedings
	if(entry.category == 1) {
		fprintf(html_page, "%s. <b>%s</b>", flipped_authors, entry.title);
		if((entry.volume != 0) && (strcmp(entry.series, "") != 0)) {
			fprintf(html_page, ", volume %d of <i>%s</i>", entry.volume, entry.series);
		}
		fprintf(html_page, ". %s, ", entry.publisher);
		if(strcmp(entry.address, "") != 0) {
			fprintf(html_page, "%s, ", entry.address);
		}
		if(strcmp(entry.edition, "") != 0) {
			fprintf(html_page, "%s, ", entry.edition);
		}
		fprintf(html_page, "%d. ", entry.year);
		if(strcmp(entry.isbn, "") != 0) {
			fprintf(html_page, "ISBN: %s. ", entry.isbn);
		}
		if(strcmp(entry.url, "") != 0) {
			fprintf(html_page, "[PDF] ");
		}

		if(strcmp(entry.keywords, "") != 0) {
			fprintf(html_page, "Keyword(s): %s. ", entry.keywords);
		}

		// print note, if any:
		if(strcmp(entry.note, "") != 0) {
			fprintf(html_page, "<i>Note</i>: %s. ", entry.note);
		}

		// abstract, if any
		if(strcmp(entry.abstract, "") != 0) {
			fprintf(html_page, "<center>Abstract:<table width=\"80%%\" border=\"1\"><tr><td>%s</td></tr></table></center>", entry.abstract);
		}
	// Thesis
	} else if(entry.category == 2) {
		fprintf(html_page, "%s. <b>%s</b>. PhD. thesis, %s, ", flipped_authors, entry.title, entry.school);
		if(strcmp(entry.note, "") != 0) {
			fprintf(html_page, "%s, ", entry.note);
		}
		fprintf(html_page, "%d.", entry.year);
		//abstract, if any
		if(strcmp(entry.abstract, "") != 0) {
			fprintf(html_page, "<center>Abstract:<table width=\"80%%\" border=\"1\"><tr><td>%s</td></tr></table></center>", entry.abstract);
		}
	// Articles in journal or book's chapters
	} else if(entry.category == 3) {
		fprintf(html_page, "%s. <b>%s</b>. ", flipped_authors, entry.title);
		if(strcmp(flipped_editors, "") != 0) {
			fprintf(html_page, "In %s, editors, <i>%s</i>, pages %s. %s, ", flipped_editors, entry.booktitle, entry.pages, entry.publisher);
		} else {
			fprintf(html_page, "<i>%s</i>, %d", entry.journal, entry.volume);
			if(entry.number != 0) {
				fprintf(html_page, "(%d)", entry.number);
			}
			fprintf(html_page, ":%s, ", entry.pages);
		}
		fprintf(html_page, "%d. ", entry.year);
		if(strcmp(entry.url, "") != 0) {
			fprintf(html_page, "[PDF] ");
		}

		if(strcmp(entry.keywords, "") != 0) {
			fprintf(html_page, "Keyword(s): %s. ", entry.keywords);
		}

		// print note, if any:
		if(strcmp(entry.note, "") != 0) {
			fprintf(html_page, "<i>Note</i>: %s. ", entry.note);
		}

		// abstract, if any
		if(strcmp(entry.abstract, "") != 0) {
			fprintf(html_page, "<center>Abstract:<table width=\"80%%\" border=\"1\"><tr><td>%s</td></tr></table></center>", entry.abstract);
		}
	// Conference articles
	} else if(entry.category == 4) {
		fprintf(html_page, "%s. <b>%s</b>. In <i>%s</i>, ", flipped_authors, entry.title, entry.booktitle);
		if(strcmp(entry.pages, "") != 0) {
			fprintf(html_page, "pages %s, ", entry.pages);
		}
		fprintf(html_page, "%d. ", entry.year);
		if(strcmp(entry.url, "") != 0) {
			fprintf(html_page, "[PDF] ");
		}

		if(strcmp(entry.keywords, "") != 0) {
			fprintf(html_page, "Keyword(s): %s. ", entry.keywords);
		}

		// print note, if any:
		if(strcmp(entry.note, "") != 0) {
			fprintf(html_page, "<i>Note</i>: %s. ", entry.note);
		}

		// abstract, if any
		if(strcmp(entry.abstract, "") != 0) {
			fprintf(html_page, "<center>Abstract:<table width=\"80%%\" border=\"1\"><tr><td>%s</td></tr></table></center>", entry.abstract);
		}
	} else {
		fprintf(html_page, "(UNDER WORK) %s, %d", entry.title, entry.year);
	}

	free(flipped_authors); free(flipped_editors);
}
