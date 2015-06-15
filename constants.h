/*
 * constants.h
 *
 *  Created on: 10 juin 2015
 *      Author: echambon
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

// Program version
#define VERSION 1

// Max strings length
#define MAXLENGTH 255

// Project filenames
#define PROPERTIES_FILENAME 		"PROPERTIES.TXT"
#define INPUT_FILENAME				"INPUT.BIB"

// Default values
#define PROJECT_TITLE_DFLT			"List of Publications"
#define AUTHOR_NAME_DFLT			"John Smith"
#define CATEGORIES_NB				7
#define MAX_COAUTHORS				10 							// max co-authors for each paper

// OUTPUT directory
//#define OUTPUT_DIR 			"./OUTPUT/"

// Table size definition
#define YEAR_COLUMN_WIDTH_PX		50
#define YEAR_COLUMNS				9

#define CATEGORY_COLUMN_WIDTH_PX	190
#define CATEGORY_COLUMNS			3

#define AUTHORS_COLUMN_WIDTH_PX		80
#define AUTHORS_COLUMNS				3

#endif /* CONSTANTS_H_ */
