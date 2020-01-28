#ifndef JEP_K_STDIO_H
#define JEP_K_STDIO_H

#include <stdarg.h>

/**
 * The buffer size used by setbuf.
 */
#define BUFSIZ 512

/**
 * Signifies the end of a file.
 * This value can also be used as an error code for other file operations.
 */
#define EOF -1

/**
 * The null pointer constant.
 * When used as a pointer, this value points to nothing.
 * When cast as an integer, it should effectively be 0.
 */
//#define NULL ((void*)0)




struct k_iobuf {
    int type;
    char* buffer;
    int count;
    int cap;
    int pos;
    char flags;
};

typedef struct k_iobuf FILE;



/**
 * Retrieves a pointer to the standard I/O streams.
 * 
 * Params:
 *   int - the file identifier of an I/O stream
 * 
 * Returns:
 *   FILE* - a pointer to an I/O stream
 */
FILE* k_get_iobuf(int id);

#define __FILE_NO_STDIN  1
#define __FILE_NO_STDOUT 2
#define __FILE_NO_STDERR 3
#define __FILE_NO_STDDBG 4

#define stdin k_get_iobuf(__FILE_NO_STDIN)
#define stdout k_get_iobuf(__FILE_NO_STDOUT)
#define stderr k_get_iobuf(__FILE_NO_STDERR)
#define stddbg k_get_iobuf(__FILE_NO_STDDBG)




/**
 * Writes a character to an output stream.
 * On success, the character written is returned.
 * On failure, EOF is returned and the error indicator is set on stdout.
 *
 * This function may be implemented as a macro, so the expression that
 * evaluates to the stream should not have any side effects.
 *
 * Params:
 *   int - a character to write
 *   FILE* - an output stream
 *
 * Returns:
 *   int - the character written or EOF on failure
 */
int putc(int c, FILE* stream);

/**
 * Writes a character to an output stream.
 * On success, the character written is returned.
 * On failure, EOF is returned and the error indicator is set on stdout.
 *
 * Params:
 *   int - a character to write
 *   FILE* - an output stream
 *
 * Returns:
 *   int - the character written or EOF on failure
 */
int fputc(int c, FILE* stream);

/**
 * Writes a character to standard output.
 * On success, the character written is returned.
 * On failure, EOF is returned and the error indicator is set on stdout.
 *
 * Params:
 *   int - a character
 *
 * Returns:
 *   int - the character written or EOF on failure
 */
int putchar(int c);

/**
 * Writes a formatted string of characters to standard output.
 * This function is like calling fprintf and passing stdout as the first
 * argument.
 *  
 * Params:
 *   const char* - a pointer to a string
 *   ... - variadic arguments
 *
 * Returns:
 *   int - the number of characters written, or -1 on failure
 */
int printf(const char* fmt, ...);

/**
 * Writes a formatted string of characters to a file.
 * This function is like calling vfprintf, but passing variadic arguments
 * instead of an initialized va_list.
 *  
 * Params:
 *   FILE* - a FILE pointer
 *   const char* - a pointer to a string
 *   ... - veriadic arguments
 *
 * Returns:
 *   int - the number of characters written, or -1 on failure
 */
int fprintf(FILE* stream, const char* fmt, ...);

/**
 * Writes a formatted string of characters to a file.
 * The first argument is a FILE pointer, indicating the destination of the
 * output data.
 * The second argument is the format string, which contains characters data
 * potentially containing format tags that act as placeholders for data that
 * will be converted to characters.
 * The third arguments is a list of arguments.
 * The argument list is expected to have been initialized by va_start before
 * calling this function, and it is expected to be terminated by va_end
 * after this function returns.
 * 
 * Upon success, this function returns the number of characters written.
 * On failure, it returns -1.
 *
 * Format tags have the following structure:
 * %<flags><width><precision><length><specifier>
 *
 * <flags> can be one of the following values:
 *   - left-justify
 *   + prepends a sign onto the result
 *   [space] if no sign will be written, prepends a blank space
 *   # forces preceding 0, 0x or 0X for integers, and decimal point for floats
 *   0 left pads the result with zeros instead of spaces
 *
 * <width> can be one of the following values:
 *   [number] minimum number of characters to be printed
 *   * the width is not in the format tag, rather it is an integer passed
 *     as a preceding argument to the function.
 *
 * <precision> can be one of the following values:
 *   .[number] for integers, this is the minimum number of digits to be
 *     written. For e, E, and f, it's the number of digits to be written
 *     after then decimal point. For g and G, it's maximum number of
 *     significant digits to be printed. For s, it's the maximum number of
 *     characters to be printed.
 *   .* the precision is not in the format tag, rather it is an integer passed
 *     as a preceding argument to the function.
 *
 * <length> can be one of the following values:
 *   h  the argument is interpreted as short int or unsigned short int
 *   l  the argument is interpreted as a long int or unsigned long int
 *      or as a wide character or wide character string
 *   ll the argument is interpreted as a long long int or unsigned long long
 *      int
 *   L  the argument is interpreted as a long double
 *
 * Potential format specifiers:
 *   c character
 *   d signed decimal integer
 *   i signed decimal integer
 *   e scientific notation using 'e' character
 *   E scientific notation using 'E' character
 *   f decimal floating point
 *   g uses shorter of e or f (not implemented)
 *   G uses shorter of E or f (not implemented)
 *   o signed octal
 *   s string of characters
 *   u unsigned decimal integer
 *   x unsigned hexadecimal integer (lower case letter)
 *   X unsigned hexadecimal integer (capital letters)
 *   p pointer address
 *   n nothing printed
 *   % the '%' character
 *
 * Params:
 *   FILE* - a FILE pointer
 *   const char* - a pointer to a string
 *   va_list - a list of arguments to be converted to strings
 *
 * Returns:
 *   int - the number of characters written, or -1 on failure
 */
int vfprintf(FILE* stream, const char* fmt, va_list arg);

#endif
