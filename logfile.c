/*
 * logfile.c
 *
 *  Created on: 21 juin 2015
 *      Author: Emmanuel
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "prototypes.h"

/**
 * log_to_file
 * Use this function to log something happening at line "line_nb".
 * Variable number of input variables is supported.
 */
void log_to_file(int line_nb, const char *str, ...) {
	FILE* file = fopen(LOG_FILENAME, "w");
	va_list ap;

	fprintf(file, "%d: ", line_nb);
	va_start(ap, str);
	vfprintf(file, str, ap);
	va_end(ap);

	fclose(file);
}
