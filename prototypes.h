/*
 * _headers.h
 *
 *  Created on: 21 juin 2015
 *      Author: Emmanuel
 */

#ifndef PROTOTYPES_H_
#define PROTOTYPES_H_

#include "entry.h"

// main.c
void html_write_header(FILE*, char*, char*);
void html_write_footer(FILE*);
void html_write_index(char*, bibentry*, int, int*, int*, char*, char**);
void html_write_year_page(int, int, bibentry*, char*);
void html_write(FILE*, char*);
void html_write_entry(FILE*, bibentry);

// misc.c
int compare_int_inc(void const*, void const*);
int compare_int_dec(void const*, void const*);
int compare_str(void const*, void const*);
int compare_char( const void*, const void*);

void str_toupper(char*);
void get_category_str(char*, int);
void flip_authors_names(char*, const char*);

// parser.c
int bib_parser(bibentry*, int, char*);
int bib_entries(char*);

// logfile.c
void log_to_file(int, const char*, ...);
void reset_logfile();

// utilities
#ifdef _WINDOWS
	#define strcasecmp stricmp
#endif

#endif /* PROTOTYPES_H_ */
