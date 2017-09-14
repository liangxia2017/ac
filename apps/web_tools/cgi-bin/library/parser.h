#ifndef _PARSER_H
#define _PARSER_H

typedef struct stringlst
{
	char str[128];
	char *changeto;
	struct stringlst *next;
	char local_buf[64];
} stringlist;


/**
 Load file and print its contents to stdout.
 @param file_name  String with file name.
 @return 0 - on error, 1 - otherwise.
*/
int parse_load (char *file_name);

/**
 Add search and replace strings to a list.
 @param start  Pointer to pointer to the start of the list of strings.
 @param string  Search string.
 @param changeto  Replace string.
 @return Void.
*/
void add_str (stringlist **start, char *string, char *changeto);

/**
 Delete a string from a list. This function deletes all strings that
 match the specified string in a list of strings.
 @param start  Pointer to pointer to the start of the list of strings.
 @param string  String.
 @return Void.
*/
void delete_string (stringlist **start, char *string);

/**
 Search for the specified keyword in a list.
 @param s  String.
 @param klst  Pointer to the stringlist type structure.
 @return Pointer to the string in the list if keyword is found,
 NULL - otherwise.
*/
stringlist *find_keyword(char *s, stringlist *klst);

/**
 Change keyword in a string with string from a list.
 @param s  String with keyword.
 @param klst  Pointer to the stringlist type structure.
 @return Void.
*/
void change_keyword(char *s, stringlist *klst);

/**
 Read file, replace all keywords with strings from a list and print
 results to stdout.
 @param filename  String with file name.
 @param start  Pointer to pointer to the start of the list of strings.
 @return Void.
*/
void print_html (char *filename, stringlist **start);

int print_list(stringlist *klst);
void free_list(stringlist **start);

#endif


