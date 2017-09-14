/** @name CGI-BIN POST method data processing library (postdata.h) */
//@{

#ifndef _POSTDATA_H
#define _POSTDATA_H

#define MAX_NAME  128
#define MAX_VALUE 256

typedef struct prop_data
{
	char *key;
	char *value;
} prop_data;

// Function declarations

/**
 Convert a hexadecimal number represented by a string into
 character with specified ASCII code. Doesn't check for errors. First
 two string characters must represent a two-digit hexadecimal number.
 @param what  String representing character ASCII code in the hexadecimal
 format.
 @return Character.
*/
char x2c (char *what);

/**
 Decode special characters in a URL encoded string. Special characters are
 encoded in hexadecimal preceded by percent sign (%NN). This function
 replaces all the %NN's with characters those ASCII codes are specified
 numbers.
 @param url  URL encoded string.
 @return Same string with decoded special characters.
*/
char *unescape_url (char *url);

/**
 Decode '+' characters in a URL encoded string. This function replaces
 all the '+' characters in a string with spaces.
 @param str  URL encoded string.
 @return Same string with decoded '+' characters.
*/
char *plustospace (char *str);

/**
 Copy one string into another one till the specified separator character
 or NULL is found.
 @param buf  Destination string.
 @param ptr  Source string.
 @param stop  Separator character.
 @param newptr  Pointer to the next character after separator in the source
 string or NULL if there are no characters. This parameter is modified
 inside function.
 @return Pointer to the begining of the destination string.
*/
char *makeword (char *buf, char *ptr, const char stop, char **newptr);

/**
 Read a HTML POST method form data returned by server into a buffer.
 Buffer memory is allocated inside the function. Returns NULL if
 there are errors.
 @return Pointer to the begining of the buffer with form data.
*/
//char *read_queue (void);
char *read_queue(void);
char *read_queue_for_upload (int *len);
/**
 Process a HTML POST method form data returned by server.
 Function splits the data into key and value pairs, decodes it and
 saves into an array of prop_data type structures.
 @param result  Array of prop_data type structures.
 @return Count of processed keys.
*/
int process_queue (prop_data result[]);
void print_ok();

//@}

#endif


