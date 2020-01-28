#ifndef JEP_K_CTYPE_H
#define JEP_K_CTYPE_H

/**
 * Checks if a character, c, is a decimal digit, uppercase letter, or lower
 * case letter. The result is true if either isalpha or isdigit would return
 * true.
 * 
 * Params:
 *   int - a character
 * 
 * Returns:
 *   a non-zero value if true, otherwise 0
 */
int isalnum(int c);

/**
 * Checks if a character, c, is an alphabetic letter.
 * 
 * Params:
 *   int - a character
 * 
 * Returns:
 *   a non-zero value if true, otherwise 0
 */
int isalpha(int c);

/**
 * Checks if a character, c, is a control character.
 * A control character is a character that does not occupy a printing position
 * on a display.
 * 
 * Params:
 *   int - a character
 * 
 * Returns:
 *   a non-zero value if true, otherwise 0
 */
int iscntrl(int c);

/**
 * Checks if a character, c, is a decimal digit.
 * Decimal digits are any of the following:
 * 0 1 2 3 4 5 6 7 8 9
 * 
 * Params:
 *   int - a character
 * 
 * Returns:
 *   a non-zero value if true, otherwise 0
 */
int isdigit(int c);

/**
 * Checks if a character, c, has a graphical representation.
 * 
 * Params:
 *   int - a character
 * 
 * Returns:
 *   a non-zero value if true, otherwise 0
 */
int isgraph(int c);

/**
 * Checks if a character, c, is a lowercase letter.
 * 
 * Params:
 *   int - a character
 * 
 * Returns:
 *   a non-zero value if true, otherwise 0
 */
int islower(int c);

/**
 * Checks if a character, c, is a printable character.
 * A printable character is a character that occupies a printing position
 * on a display.
 * 
 * Params:
 *   int - a character
 * 
 * Returns:
 *   a non-zero value if true, otherwise 0
 */
int isprint(int c);

/**
 * Checks if a character, c, is a punctuation character.
 * 
 * Params:
 *   int - a character
 * 
 * Returns:
 *   a non-zero value if true, otherwise 0
 */
int ispunct(int c);

/**
 * Checks if a character, c, is a white space character.
 * 
 * Params:
 *   int - a character
 * 
 * Returns:
 *   a non-zero value if true, otherwise 0
 */
int isspace(int c);

/**
 * Checks if a character, c, is a uppercase letter.
 * 
 * Params:
 *   int - a character
 * 
 * Returns:
 *   a non-zero value if true, otherwise 0
 */
int isupper(int c);

/**
 * Checks if a character, c, is a hexadecimal digit.
 * Hexadecimal digits are any of the following:
 * 0 1 2 3 4 5 6 7 8 9 a b c d e f A B C D E F
 * 
 * Params:
 *   int - a character
 * 
 * Returns:
 *   a non-zero value if true, otherwise 0
 */
int isxdigit(int c);

/**
 * Converts a character, c, to its lowercase equivalent.
 * If no such conversion is possible, c is unchanged.
 * 
 * Params:
 *   int - a character
 * 
 * Returns:
 *   the lowercase version of the character, other the unchanged character
 */
int tolower(int c);

/**
 * Converts a character, c, to its uppercase equivalent.
 * If no such conversion is possible, c is unchanged.
 * 
 * Params:
 *   int - a character
 * 
 * Returns:
 *   the uppercase version of the character, other the unchanged character
 */
int toupper(int c);

#endif
