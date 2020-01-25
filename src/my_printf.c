#include <stdint.h>
#include <stdarg.h>

#include "libc/stdio.h"
#include "libc/string.h"

#include "kernel/vga.h"
#include "kernel/serial.h"

// character buffers used for standard I/O operations
// In the future, these may be allocated differently, and each process
// may have its own set of I/O buffers.
char k_ibuf[BUFSIZ];
char k_obuf[BUFSIZ];
char k_ebuf[BUFSIZ];
char k_dbuf[BUFSIZ];

// The IO streams
// These represent the current implementation of stdin, stdout, and stderr.
// We also include the non-standard stddbg for development purposes.
FILE k_stdin  = { 1, k_ibuf, 0, BUFSIZ, 0, 0 };
FILE k_stdout = { 2, k_obuf, 0, BUFSIZ, 0, 0 };
FILE k_stderr = { 3, k_ebuf, 0, BUFSIZ, 0, 0 };
FILE k_stddbg = { 4, k_dbuf, 0, BUFSIZ, 0, 0 };


/**
 * Retrieves a pointer to standard output.
 * The resulting file object can be used as the resulting value of the
 * stdout macro.
 *
 * Returns:
 *   FILE* - stdout.
 */
FILE* k_get_iobuf(int id)
{
	switch(id)
	{
		case 1: return &k_stdin;
		case 2: return &k_stdout;
		case 3: return &k_stderr;
		case 4: return &k_stddbg;
		default: return NULL;
	}
}

// number buffer size limit
#define BUF_LIMIT 100




//--------------------------------------------------------------------------//
//                              Format Tags                                 //
//--------------------------------------------------------------------------//

// format flag bit flags
#define FMT_LEFT   0x01 /* -                               */
#define FMT_SIGN   0x02 /* +                               */
#define FMT_SPACE  0x04 /* [space]                         */
#define FMT_POINT  0x08 /* #                               */
#define FMT_ZERO   0x10 /* 0                               */
#define FMT_WIDTH  0x20 /* width is passed as argument     */
#define FMT_PREC   0x40 /* precision is passed as argument */
#define FMT_ZPREC  0x40 /* precision is zero               */

// format specifiers
#define SPEC_c 'c'
#define SPEC_s 's'
#define SPEC_d 'd'
#define SPEC_i 'i'
#define SPEC_u 'u'
#define SPEC_f 'f'
#define SPEC_e 'e'
#define SPEC_E 'E'
#define SPEC_g 'g'
#define SPEC_G 'G'
#define SPEC_o 'o'
#define SPEC_x 'x'
#define SPEC_X 'X'
#define SPEC_p 'p'
#define SPEC_n 'n'
#define SPEC_per '%'

// format length bit flags
#define LEN_h  0x01 /* h  */
#define LEN_l  0x02 /* l  */
#define LEN_ll 0x04 /* ll */
#define LEN_L  0x08 /* L  */

// format tag parser states
#define STATE_FLAGS  1
#define STATE_WIDTH  2
#define STATE_PREC   3
#define STATE_LENGTH 4
#define STATE_SPEC   5
#define STATE_DONE   6

/**
 * Determines if a character within a format tag is a valid format flag.
 * If the character is a format flag, then a flag variable is set to have
 * the value of one of the format bit flags.
 * If the character is not a valid flag value, the variable is set to 0.
 *
 * Params:
 *   c - a character within a format tag
 *   f - a variable to contain the bit flag
 */
#define is_flag(c, f) (     \
    f = c == '-' ? FMT_LEFT \
    : c == '+' ? FMT_SIGN   \
    : c == ' ' ? FMT_SPACE  \
    : c == '#' ? FMT_POINT  \
    : c == '0' ? FMT_ZERO   \
    : 0                     \
)

/**
 * Determines if a character within a format tag is a valid length value.
 * If the character is a valid length value, then a flag variable is set to
 * have the value of one of the length bit flags.
 * If the character is not a valid length value, the variable is set to 0.
 *
 * Params:
 *   c - a character within a format tag
 *   l - a variable to contain the bit flag
 */
#define is_len(c, l) (   \
    l = c == 'h' ? LEN_h \
    : c == 'l' ? LEN_l   \
    : c == 'L' ? LEN_L   \
    : 0                  \
)

/**
 * Determines if a character within a format tag is a valid specifier.
 * If the character is a valid specifier, then a variable is set with the
 * value of that specifier.
 * If the character is not a valid specifier, the variable is set to 0.
 *
 * Params:
 *   c - a character within a format tag
 *   s - a variable to contain the specifier character
 */
#define is_spec(c, s) (   \
    s = c == 'c' ? SPEC_c \
    : c == 's' ? SPEC_s   \
    : c == 'd' ? SPEC_d   \
    : c == 'i' ? SPEC_i   \
    : c == 'u' ? SPEC_u   \
    : c == 'f' ? SPEC_f   \
    : c == 'e' ? SPEC_e   \
    : c == 'E' ? SPEC_E   \
    : c == 'g' ? SPEC_g   \
    : c == 'G' ? SPEC_G   \
    : c == 'o' ? SPEC_o   \
    : c == 'x' ? SPEC_x   \
    : c == 'X' ? SPEC_X   \
    : c == 'p' ? SPEC_p   \
    : c == 'n' ? SPEC_n   \
    : c == '%' ? SPEC_per \
    : 0                   \
)

/**
 * A format tag within a format string.
 */
typedef struct ftag {
	unsigned char flags;
	size_t width;
	size_t prec;
	unsigned char len;
	char spec;
}ftag;

/**
 * Parses a format tag within a format string for printf.
 * This function is intended to be called while traversing a string.
 * The first argument will be a pointer to the current position within
 * that string. The second argument is a reference to a pointer to the
 * new position in the string.
 *
 * Params:
 *   const char* start - a pointer to the starting index in the string
 *   char** - a reference to a pointer that contains the new position
 *     in the original string.
 *
 * Returns:
 *   ftag - a format tag structure
 */
static ftag parse_format(const char* start, char** end);




//--------------------------------------------------------------------------//
//                         Floating Point Numbers                           //
//--------------------------------------------------------------------------//

#define FLOAT_SGN_BIT(n) ((n & 0x80000000) >> 31)
#define FLOAT_EXP_BIT(n) (((n & 0x7F800000) >> 23) - 0x7F)
#define FLOAT_MNT_BIT(n) ((n & 0x007FFFFF) | 0x00800000)

// maximum exponent absolute value
#define FLOAT_EXP_N 151

// maximum digits in binary number component
#define FLOAT_BIN_DIG 152


#define DOUBLE_SGN_BIT(n) ((n & 0x8000000000000000) >> 63)
#define DOUBLE_EXP_BIT(n) (((n & 0x7FF0000000000000) >> 52) - 0x3FF)
#define DOUBLE_MNT_BIT(n) ((n & 0xFFFFFFFFFFFFF) | 0x10000000000000)

// maximum exponent absolute value
#define DOUBLE_EXP_N 1200

// maximum digits in binary number component
#define DOUBLE_BIN_DIG 1200

// left and right sides of the radix point
#define SN_LEFT 0
#define SN_RIGHT 1


/**
 * An IEEE 754 single-precision floating point number.
 */
typedef struct ieee_754_float {
	uint32_t raw;  // raw, 32-bit binary data
	uint8_t sign;  // sign (0 for positive, 1 for negative)
	int8_t exp;    // exponent
	uint32_t mant; // mantissa
}ieee_754_float;

/**
 * An IEEE 754 double-precision floating point number.
 */
typedef struct ieee_754_double {
	uint64_t raw;  // raw, 64-bit binary data
	uint8_t sign;  // sign (0 for positive, 1 for negative)
	int16_t exp;   // exponent
	uint64_t mant; // mantissa
}ieee_754_double;




/**
 * Converts a float to a uint32_t by moving the raw binary data into an
 * integer register.
 * This particular implementation is intended for a 32-bit system using the
 * System V ABI on an x86 architecture.
 *
 * Params:
 *   float - a 32-bit IEEE 754 single-precision floating point number
 *
 * Returns:
 *   uint32_t - the raw binary data of the floating point number
 */
extern uint32_t extract_float_sysv32(float f);

/**
 * Converts a double to a uint64_t by moving the raw binary data into an
 * integer register.
 * This particular implementation is intended for a 32-bit system using the
 * System V ABI on an x86 architecture.
 *
 * Params:
 *   double - a 64-bit IEEE 754 double-precision floating point number
 *
 * Returns:
 *   uint32_t - the raw binary data of the floating point number
 */
extern uint64_t extract_double_sysv32(double d);

/**
 * Extracts the raw binary components of a 32-bit IEEE 754 single-precision
 * floating point number.
 *
 * Params:
 *   float - a 32-bit IEEE 754 single-precision floating point number
 *
 * Returns:
 *   ieee_754_float - the individual binary components of the number
 */
//static ieee_754_float extract_float(float f);

/**
 * Extracts the raw binary components of a 64-bit IEEE 754 double-precision
 * floating point number.
 *
 * Params:
 *   double - a 64-bit IEEE 754 double-precision floating point number
 *
 * Returns:
 *   ieee_754_double - the individual binary components of the number
 */
static ieee_754_double extract_double(double d);

/**
 * Converts a binary fraction value to decimal.
 * The first argument is a pointer to a character array containing
 * all of the digits to the right of the radix point in a binary number.
 * So for the given binary number 110.010, the input string would be "010",
 * and the expected output would be "25".
 * The second argument is a pointer to the output buffer.
 * The output buffer is assumed to be large enough to hold at least 152
 * 8-bit numbers.
 *
 * Params:
 *   const char* - a pointer to a character array of '1' and '0'
 *   uint8_t* - a pointer to the output buffer
 */
//static size_t fbin_to_fdec_frac(const char* raw, uint8_t* frac);

/**
 * Converts a binary whole number to decimal.
 * The first argument is a pointer to a character array containing
 * all of the digits to the left of the radix point in a binary number.
 * So for the given binary number 110.010, the input string would be "110",
 * and the expected output would be "6".
 * The second argument is a pointer to the output buffer.
 * The output buffer is assumed to be large enough to hold at least 152
 * 8-bit numbers.
 *
 * Params:
 *   const char* - a pointer to a character array of '1' and '0'
 *   uint8_t* - a pointer to the output buffer
 *
 * Returns:
 *   size_t - the number of values written to the output buffer
 */
//static size_t fbin_to_fdec_whole(const char* raw, uint8_t* whole);

/**
 * Converts a single-precision floating point number into two unsigned integer
 * arrays that contain the decimal digits of the left and right sides of
 * the radix point.
 *
 * Params:
 *   float - the number to expand
 *   uint8_t* - an array to contain the digits to the left of the radix point
 *   size_t* - a pointer to the number of digits to the left of the radix point
 *   uint8_t* - an array to contain the digits to the right of the radix point
 *   size_t* - a pointer to the number of digits to the right of the radix point
 */
//static void expand_float(float f, uint8_t* whole, size_t* w_res, uint8_t* frac, size_t* f_res);

/**
 * Converts a binary fraction value to decimal.
 * The first argument is a pointer to a character array containing
 * all of the digits to the right of the radix point in a binary number.
 * So for the given binary number 110.010, the input string would be "010",
 * and the expected output would be "25".
 * The second argument is a pointer to the output buffer.
 * The output buffer is assumed to be large enough to hold at least 152
 * 8-bit numbers.
 *
 * Params:
 *   const char* - a pointer to a character array of '1' and '0'
 *   uint8_t* - a pointer to the output buffer
 */
static size_t dbin_to_ddec_frac(const char* raw, uint8_t* frac);

/**
 * Converts a binary whole number to decimal.
 * The first argument is a pointer to a character array containing
 * all of the digits to the left of the radix point in a binary number.
 * So for the given binary number 110.010, the input string would be "110",
 * and the expected output would be "6".
 * The second argument is a pointer to the output buffer.
 * The output buffer is assumed to be large enough to hold at least 152
 * 8-bit numbers.
 *
 * Params:
 *   const char* - a pointer to a character array of '1' and '0'
 *   uint8_t* - a pointer to the output buffer
 *
 * Returns:
 *   size_t - the number of values written to the output buffer
 */
static size_t dbin_to_ddec_whole(const char* raw, uint8_t* whole);

/**
 * Converts a double-precision floating point number into two unsigned integer
 * arrays that contain the decimal digits of the left and right sides of
 * the radix point.
 *
 * Params:
 *   double - the number to expand
 *   uint8_t* - an array to contain the digits to the left of the radix point
 *   size_t* - a pointer to the number of digits to the left of the radix point
 *   uint8_t* - an array to contain the digits to the right of the radix point
 *   size_t* - a pointer to the number of digits to the right of the radix point
 */
static void expand_double(double d, uint8_t* whole, size_t* w_res, uint8_t* frac, size_t* f_res);

/**
 * Performs carries when rounding floating point numbers that have been
 * converted to arrays. After determining the starting position, this
 * function gradually moves left, adding 1 to each element in the arrays
 * until no elements exceed 10. If this function reaches the far left end
 * of the range of digits, then an extra digit is required to represent
 * the rounded value.
 *
 * Params:
 *   uint8_t* - an array containing digits to the left of the radix point
 *   size_t - the size of the left array
 *   uint8_t* - an array containing digits to the right of the radix point
 *   size_t - the size of the right array
 *   size_t - the starting index
 *   int - the starting side (0 for left, 1 for right)
 *
 * Returns:
 *   int - 1 if a new digit is required, otherwise 0
 */
static int carry(uint8_t* left, size_t l_size, uint8_t* right, size_t r_size, size_t ind, int side);

/**
 * Rounds a floating point number using banker's rounding.
 *
 * Params:
 *   uint8_t* - an array of digits to the left of the radix point
 *   size_t - the size of the left array
 *   uint8_t* - an array of digits to the right of the radix point
 *   size_t - the size of the right array
 *   size_t - precision of 1 or greater
 *
 * Returns:
 *   int - whether or not an extra digit should be added
 */
static int round(uint8_t* left,
	size_t l_size,
	uint8_t* right,
	size_t r_size,
	size_t prec);

/**
 * Rounds the digits to the left of the radix point using banker's rounding.
 * This is intended for use when printing scientific notation using
 * the 'E' or 'e' specifiers.
 *
 * Params:
 *   uint8_t* - an array of digits to the left of the radix point
 *   size_t - the size of the left array
 *   size_t - precision of 1 or greater
 *
 * Returns:
 *   int - whether or not an extra digit should be added
 */
static int round_left(uint8_t* left,
	size_t l_size,
	uint8_t* right,
	size_t r_size,
	size_t prec);



//--------------------------------------------------------------------------//
//                     Number to String Conversion                          //
//--------------------------------------------------------------------------//

/**
 * Reverses the order of a character array.
 * Given a length of l, only the first l characters will be reversed.
 * Any characters after that in the buffer will remain unmoved.
 *
 * Params:
 *   char* - a pointer to a character array
 *   size_t - the number of characters to reverse
 */
static void reverse(char* str, size_t len);

/**
 * Converts an integer to a NUL-terminated character array.
 * The characters are written to an output buffer.
 * The returned value is the number of characters in the output buffer
 * not including the NUL character, or 0 on failure.
 * The output buffer should allow for at least BUF_LIMIT characters.
 * The limit specified by BUF_LIMIT includes the NUL terminating character.
 *
 * Params:
 *   int - an integer
 *   char* - a character buffer
 *   ftag - a format tag from a printf format string
 *
 * Returns:
 *   size_t - the number of characters in the output buffer
 */
static size_t int_to_buffer(int n, char* buffer, ftag t);

/**
 * Converts an unsigned pointer to a NUL-terminated character array.
 * The characters are written to an output buffer.
 * The returned value is the number of characters in the output buffer
 * not including the NUL character, or 0 on failure.
 * The output buffer should allow for at least BUF_LIMIT characters.
 * The limit specified by BUF_LIMIT includes the NUL terminating character.
 *
 * Params:
 *   uintptr_t - an unsigned pointer
 *   char* - a character buffer
 *   int - capitalization flag (0 if lower case, or 1 for upper case)
 *
 * Returns:
 *   size_t - the number of characters in the output buffer
 */
static size_t uptr_to_buffer(uintptr_t n, char* buffer, int cap);

/**
 * Converts a size_t value to a NUL-terminated character array.
 * The characters are written to an output buffer.
 * The returned value is the number of characters in the output buffer
 * not including the NUL character, or 0 on failure.
 * The output buffer should allow for at least BUF_LIMIT characters.
 * The limit specified by BUF_LIMIT includes the NUL terminating character.
 *
 * Params:
 *   size_t - the unsigned, integral value to convert
 *   char* - a pointer to a chatacter array
 *
 * Returns:
 *   size_t - the number of characters in the output buffer
 */
static size_t size_to_buffer(size_t n, char* buffer);

/**
 * Converts a double-precision floating pointer number to a NUL-terminated
 * character array. The characters are written to an output buffer.
 * The returned value is the number of characters in the output buffer
 * not including the NUL character, or 0 on failure.
 * The output buffer should allow for at least BUF_LIMIT characters.
 * The limit specified by BUF_LIMIT includes the NUL terminating character.
 *
 * Params:
 *   double - the number to convert
 *   char* - a pointer to a character array
 *   ftag - a format tag
 *
 * Returns:
 *   size_t - the number of characters in the buffer
 */
static size_t double_to_buffer(double d, char* buf, ftag t);

/**
 * Converts a double-precision floating pointer number to a NUL-terminated
 * character array. The resulting character array depicts the number in
 * scientific notation. The characters are written to an output buffer.
 * The returned value is the number of characters in the output buffer
 * not including the NUL character, or 0 on failure.
 * The output buffer should allow for at least BUF_LIMIT characters.
 * The limit specified by BUF_LIMIT includes the NUL terminating character.
 *
 * Params:
 *   double - the number to convert
 *   char* - a pointer to a character array
 *   ftag - a format tag
 *
 * Returns:
 *   size_t - the number of characters in the buffer
 */
static size_t doublesn_to_buffer(double d, char* buf, ftag t);

/**
 * Converts a string of characters into an unsigned integral integer.
 * This function scans a character array looking for decimal digits and parses
 * them until it encounters a character outside of the range of
 * '0' through '9'.
 * The first argument is intended to be a string of unsigned decimal integer
 * digits that may occur within a larger string.
 * The second argument is a reference to a memory location that will receive
 * the number that resulted from the conversion.
 *
 *
 * Params:
 *   const char* - a pointer to a sequence of characters
 *   size_t* - a pointer to a size_t to store the resulting number
 *
 * Returns:
 *   size_t - the number of digits in the resulting number
 */
static size_t parse_udec(const char* str, size_t* res);




static ftag parse_format(const char* start, char** end)
{
	size_t n;           // conversion result
	size_t d;           // index change
	int state;          // current parser state
	unsigned char flag; // flag
	unsigned char len;  // length
	char spec;          // specifier
	ftag tag;           // format tag
	size_t e;           // element size

	tag.flags = 0;
	tag.width = 0;
	tag.prec = 0;
	tag.len = 0;
	tag.spec = 0;

	state = STATE_FLAGS;
	e = sizeof(char);

	while (*start != '\0' && state != STATE_DONE)
	{
		switch (state)
		{
		case STATE_FLAGS:
		{
			if (is_flag(*start, flag))
			{
                tag.flags |= flag;
                
				start++;
			}
			else
				state++;
		}
		break;

		case STATE_WIDTH:
		{
			if (*start == '*')
			{
				tag.flags |= FMT_WIDTH;
				start++;
			}
			else
			{
				d = parse_udec(start, &n);
				tag.width = n;
				start += e * d;
			}

			state++;
		}
		break;

		case STATE_PREC:
		{
			if (*start == '.')
			{
				start += e;
				if (*start == '*')
				{
					tag.flags |= FMT_PREC;
					start += e;
				}
				else
				{
					d = parse_udec(start, &n);
					tag.prec = n;
					start += e * d;
				}
			}

			state++;
		}
		break;

		case STATE_LENGTH:
		{
			if (is_len(*start, len))
			{
				if (len == LEN_l && tag.len & LEN_l)
                    tag.len |= LEN_ll;
                else
                    tag.len |= len;

				start += e;
			}
			else
				state++;
		}
		break;

		case STATE_SPEC:
			tag.spec = is_spec(*start, spec) ? spec : 0;
			state++;
			break;

		default:
			break;
		}
	}

	// Update the end position
	*end = (char*)start;

	return tag;
}




// static ieee_754_float extract_float(float f)
// {
// 	ieee_754_float comp;
// 	uint32_t raw;

// 	raw = extract_float_sysv32(f);

// 	comp.raw = raw;
// 	comp.sign = FLOAT_SGN_BIT(raw);
// 	comp.exp = FLOAT_EXP_BIT(raw);
// 	comp.mant = FLOAT_MNT_BIT(raw);

// 	// Remove the implicit 1 bit for denormalized numbers.
// 	if (((raw & 0x7F800000) >> 23) == 0)
// 	{
// 		comp.mant ^= 0x00800000;
// 	}

// 	return comp;
// }

static ieee_754_double extract_double(double d)
{
	ieee_754_double comp;
	uint64_t raw;

	raw = extract_double_sysv32(d);

	comp.raw = raw;
	comp.sign = DOUBLE_SGN_BIT(raw);
	comp.exp = DOUBLE_EXP_BIT(raw);
	comp.mant = DOUBLE_MNT_BIT(raw);

	// Remove the implicit 1 bit for denormalized numbers.
	if (((raw & 0x7FF0000000000000) >> 52) == 0)
	{
		comp.mant ^= 0x10000000000000;
	}

	return comp;
}

// static size_t fbin_to_fdec_frac(const char* raw, uint8_t* frac)
// {
// 	size_t i, j, k, c;              // indices
// 	uint8_t conv[2][FLOAT_BIN_DIG]; // binary to decimal conversion array
// 	uint8_t dec[FLOAT_BIN_DIG];     // decimal number
// 	uint8_t next;                   // next digit in conversion array
// 	uint8_t half;                   // half of the next conversion digit
// 	size_t n;                       // decimal digit counter

// 	// Prepopulate the fraction evaluator arrays with 0
// 	for (i = 0; i < FLOAT_BIN_DIG; i++)
// 	{
// 		dec[i] = 0;
// 		conv[0][i] = 0;
// 		conv[1][i] = 0;
// 	}

// 	// Give the conversion array a starting value
// 	conv[0][0] = 5;

// 	i = j = k = 0;

// 	while (*raw != '\0' && i < FLOAT_EXP_N)
// 	{
// 		// If the current binary digit is 1, add the
// 		// conversion result to the current decimal value.
// 		if (*raw == '1')
// 		{
// 			for (j = 0; j < FLOAT_BIN_DIG; j++)
// 			{
// 				dec[j] += conv[k][j];

// 				// Handle carries
// 				if (dec[j] >= 10)
// 				{
// 					for (c = j; c > 0 && dec[c] >= 10; c--)
// 					{
// 						dec[c - 1]++;
// 						dec[c] = dec[c] - 10;
// 					}
// 				}
// 			}
// 		}

// 		// Calculate the decimal equivalent of the next binary digit
// 		for (j = 0; j < FLOAT_BIN_DIG && i < FLOAT_EXP_N - 1; j++)
// 		{
// 			next = conv[k][j];
// 			half = next / 2;

// 			conv[k][j] = 0;

// 			if (half > 0 && !(next & 1))
// 				conv[k ? 0 : 1][j] += half;

// 			else if (j < FLOAT_BIN_DIG - 1)
// 			{
// 				conv[k ? 0 : 1][j] += half;
// 				conv[k ? 0 : 1][j + 1] += next > 0 ? 5 : 0;
// 			}
// 		}

// 		k = k ? 0 : 1;
// 		i++;
// 		raw++;
// 	}

// 	// Determine the number of digits in the final decimal number
// 	for (n = FLOAT_BIN_DIG - 1; dec[n] == 0 && n > 0; n--);

// 	n++;

// 	for (i = 0; i < n; i++)
// 		frac[i] = dec[i];

// 	return n;
// }

// static size_t fbin_to_fdec_whole(const char* raw, uint8_t* whole)
// {
// 	size_t i, j, k, c;              // indices
// 	uint8_t conv[2][FLOAT_BIN_DIG]; // binary to decimal conversion array
// 	uint8_t dec[FLOAT_BIN_DIG];     // decimal number
// 	uint8_t next;                   // next digit in conversion array
// 	uint8_t doub;                   // double of the next conversion digit
// 	size_t len;                     // length of the raw string
// 	size_t l;                       // length counter
// 	size_t n;                       // decimal digit counter

// 	len = strlen(raw);
// 	l = len;

// 	// Prepopulate the arrays with 0
// 	for (i = 0; i < FLOAT_BIN_DIG; i++)
// 	{
// 		dec[i] = 0;
// 		conv[0][i] = 0;
// 		conv[1][i] = 0;
// 	}

// 	// Give the conversion array a starting value
// 	conv[0][FLOAT_BIN_DIG - 1] = 1;

// 	i = j = k = n = 0;

// 	while (l > 0 && i < FLOAT_EXP_N)
// 	{
// 		// If the current binary digit is 1, add the
// 		// conversion result to the current decimal value.
// 		if (raw[l - 1] == '1')
// 		{
// 			for (j = FLOAT_BIN_DIG - 1; j > 0; j--)
// 			{
// 				dec[j] += conv[k][j];

// 				// Handle carries
// 				if (dec[j] >= 10)
// 				{
// 					for (c = j; c > 0 && dec[c] >= 10; c--)
// 					{
// 						dec[c - 1]++;
// 						dec[c] = dec[c] - 10;
// 					}
// 				}
// 			}
// 		}

// 		// Calculate the decimal equivalent of the next binary digit
// 		for (j = FLOAT_BIN_DIG - 1; j > 0 && i < FLOAT_EXP_N - 1; j--)
// 		{
// 			next = conv[k][j];
// 			doub = next * 2;

// 			conv[k][j] = 0;

// 			conv[k ? 0 : 1][j] += doub;

// 			// Handle carries
// 			if (conv[k ? 0 : 1][j] >= 10)
// 			{
// 				for (c = j; c > 0 && conv[k ? 0 : 1][c] >= 10; c--)
// 				{
// 					conv[k ? 0 : 1][c - 1]++;
// 					conv[k ? 0 : 1][c] = conv[k ? 0 : 1][c] - 10;
// 				}
// 			}
// 		}

// 		k = k ? 0 : 1;
// 		i++;
// 		l--;
// 	}

// 	// Determine the number of digits in the final decimal number
// 	for (n = 0; dec[n] == 0 && n < FLOAT_BIN_DIG; n++);

// 	for (i = n; i < FLOAT_BIN_DIG; i++)
// 		whole[i - n] = dec[i];

// 	return FLOAT_BIN_DIG - n;
// }

// static void expand_float(float f,
// 	uint8_t* whole,
// 	size_t* w_res,
// 	uint8_t* frac,
// 	size_t* f_res)
// {
// 	ieee_754_float ieeef;
// 	char mstr[25];
// 	int16_t i, j;
// 	uint8_t li;
// 	uint8_t ri;
// 	uint8_t rad;
// 	char left[FLOAT_BIN_DIG];
// 	char right[FLOAT_BIN_DIG];


// 	// Extract the binary components of the float.
// 	ieeef = extract_float(f);

// 	// Prepopulate the character arrays with '\0'.
// 	for (i = 0; i < FLOAT_BIN_DIG; i++)
// 	{
// 		left[i] = '\0';
// 		right[i] = '\0';
// 		frac[i] = 0;
// 		whole[i] = 0;
// 	}

// 	// Convert the mantissa to a NUL-terminated string.
// 	for (i = 23, j = 0; i >= 0; i--)
// 	{
// 		mstr[j++] = ((ieeef.mant & (1 << i)) >> i) ? '1' : '0';
// 	}
// 	mstr[24] = '\0';

// 	rad = li = ri = 0;

// 	// If the exponent is negative, add leading zeros '0'
// 	// to the right side array.
// 	if (ieeef.exp + 1 < 0)
// 	{
// 		rad = 1;
// 		for (i = 0; i < -(ieeef.exp + 1); i++)
// 			right[ri++] = '0';
// 	}

// 	for (i = 0; i < 160; i++)
// 	{
// 		if (i == ieeef.exp + 1)
// 			rad = 1;

// 		if (i < 24)
// 		{
// 			// Get the characters from the mantissa string.
// 			if (rad)
// 				right[ri++] = mstr[i];

// 			else
// 				left[li++] = mstr[i];
// 		}
// 		else if (i < ieeef.exp + 1)
// 		{
// 			// Since we've used all the characters from the
// 			// mantissa string, we start outputting zeros '0'.
// 			if (rad)
// 				right[ri++] = '0';

// 			else
// 				left[li++] = '0';
// 		}
// 	}

// 	*w_res = fbin_to_fdec_whole(left, whole);
// 	*f_res = fbin_to_fdec_frac(right, frac);
// }

static size_t dbin_to_ddec_frac(const char* raw, uint8_t* frac)
{
	size_t i, j, k, c;               // indices
	uint8_t conv[2][DOUBLE_BIN_DIG]; // binary to decimal conversion array
	uint8_t dec[DOUBLE_BIN_DIG];     // decimal number
	uint8_t next;                    // next digit in conversion array
	uint8_t half;                    // half of the next conversion digit
	size_t n;                        // decimal digit counter

	// Prepopulate the fraction evaluator arrays with 0
	for (i = 0; i < DOUBLE_BIN_DIG; i++)
	{
		dec[i] = 0;
		conv[0][i] = 0;
		conv[1][i] = 0;
	}

	// Give the conversion array a starting value
	conv[0][0] = 5;

	i = j = k = 0;

	while (*raw != '\0' && i < DOUBLE_EXP_N)
	{
		// If the current binary digit is 1, add the
		// conversion result to the current decimal value.
		if (*raw == '1')
		{
			for (j = 0; j < DOUBLE_BIN_DIG; j++)
			{
				dec[j] += conv[k][j];

				// Handle carries
				if (dec[j] >= 10)
				{
					for (c = j; c > 0 && dec[c] >= 10; c--)
					{
						dec[c - 1]++;
						dec[c] = dec[c] - 10;
					}
				}
			}
		}

		// Calculate the decimal equivalent of the next binary digit
		for (j = 0; j < DOUBLE_BIN_DIG && i < DOUBLE_EXP_N - 1; j++)
		{
			next = conv[k][j];
			half = next / 2;

			conv[k][j] = 0;

			if (half > 0 && !(next & 1))
				conv[k ? 0 : 1][j] += half;

			else if (j < DOUBLE_BIN_DIG - 1)
			{
				conv[k ? 0 : 1][j] += half;
				conv[k ? 0 : 1][j + 1] += next > 0 ? 5 : 0;
			}
		}

		k = k ? 0 : 1;
		i++;
		raw++;
	}

	// Determine the number of digits in the final decimal number
	for (n = DOUBLE_BIN_DIG - 1; dec[n] == 0 && n > 0; n--);

	n++;

	for (i = 0; i < n; i++)
		frac[i] = dec[i];

	return n;
}

static size_t dbin_to_ddec_whole(const char* raw, uint8_t* whole)
{
	size_t i, j, k, c;               // indices
	uint8_t conv[2][DOUBLE_BIN_DIG]; // binary to decimal conversion array
	uint8_t dec[DOUBLE_BIN_DIG];     // decimal number
	uint8_t next;                    // next digit in conversion array
	uint8_t doub;                    // double of the next conversion digit
	size_t len;                      // length of the raw string
	size_t l;                        // length counter
	size_t n;                        // decimal digit counter

	len = strlen(raw);
	l = len;

	// Prepopulate the arrays with 0
	for (i = 0; i < DOUBLE_BIN_DIG; i++)
	{
		dec[i] = 0;
		conv[0][i] = 0;
		conv[1][i] = 0;
	}

	// Give the conversion array a starting value
	conv[0][DOUBLE_BIN_DIG - 1] = 1;

	i = j = k = n = 0;

	while (l > 0 && i < DOUBLE_EXP_N)
	{
		// If the current binary digit is 1, add the
		// conversion result to the current decimal value.
		if (raw[l - 1] == '1')
		{
			for (j = DOUBLE_BIN_DIG - 1; j > 0; j--)
			{
				dec[j] += conv[k][j];

				// Handle carries
				if (dec[j] >= 10)
				{
					for (c = j; c > 0 && dec[c] >= 10; c--)
					{
						dec[c - 1]++;
						dec[c] = dec[c] - 10;
					}
				}
			}
		}

		// Calculate the decimal equivalent of the next binary digit
		for (j = DOUBLE_BIN_DIG - 1; j > 0 && i < DOUBLE_EXP_N - 1; j--)
		{
			next = conv[k][j];
			doub = next * 2;

			conv[k][j] = 0;

			conv[k ? 0 : 1][j] += doub;

			// Handle carries
			if (conv[k ? 0 : 1][j] >= 10)
			{
				for (c = j; c > 0 && conv[k ? 0 : 1][c] >= 10; c--)
				{
					conv[k ? 0 : 1][c - 1]++;
					conv[k ? 0 : 1][c] = conv[k ? 0 : 1][c] - 10;
				}
			}
		}

		k = k ? 0 : 1;
		i++;
		l--;
	}

	// Determine the number of digits in the final decimal number
	for (n = 0; dec[n] == 0 && n < DOUBLE_BIN_DIG; n++);

	for (i = n; i < DOUBLE_BIN_DIG; i++)
		whole[i - n] = dec[i];

	return DOUBLE_BIN_DIG - n;
}

static void expand_double(double d,
	uint8_t* whole,
	size_t* w_res,
	uint8_t* frac,
	size_t* f_res)
{
	ieee_754_double ieeed;
	char mstr[54];
	int16_t i, j;
	uint8_t li;
	uint8_t ri;
	uint8_t rad;
	char left[DOUBLE_BIN_DIG];
	char right[DOUBLE_BIN_DIG];


	// Extract the binary components of the float.
	ieeed = extract_double(d);

	// Prepopulate the character arrays with '\0'.
	for (i = 0; i < DOUBLE_BIN_DIG; i++)
	{
		left[i] = '\0';
		right[i] = '\0';
		frac[i] = 0;
		whole[i] = 0;
	}

	// Convert the mantissa to a NUL-terminated string.
	for (i = 52, j = 0; i >= 0; i--)
	{
		mstr[j++] = ((ieeed.mant & ((uint64_t)1 << i)) >> i) ? '1' : '0';
	}
	mstr[53] = '\0';

	rad = li = ri = 0;

	// If the exponent is negative, add leading zeros '0'
	// to the right side array.
	if (ieeed.exp + 1 < 0)
	{
		rad = 1;
		for (i = 0; i < -(ieeed.exp + 1); i++)
			right[ri++] = '0';
	}

	for (i = 0; i < 160; i++)
	{
		if (i == ieeed.exp + 1)
			rad = 1;

		if (i < 53)
		{
			// Get the characters from the mantissa string.
			if (rad)
				right[ri++] = mstr[i];

			else
				left[li++] = mstr[i];
		}
		else if (i < ieeed.exp + 1)
		{
			// Since we've used all the characters from the
			// mantissa string, we start outputting zeros '0'.
			if (rad)
				right[ri++] = '0';

			else
				left[li++] = '0';
		}
	}

	*w_res = dbin_to_ddec_whole(left, whole);
	*f_res = dbin_to_ddec_frac(right, frac);
}

static int carry(uint8_t* left,
	size_t l_size,
	uint8_t* right,
	size_t r_size,
	size_t ind,
	int side)
{
	size_t i, j;

	if (side == SN_RIGHT)
	{
		// Move through the right side, incrementing
		// the digits until they are all less than 10.
		for (i = ind; i > 0 && i < r_size && right[i] >= 10; i--)
		{
			right[i] = 0;
			right[i - 1]++;
		}

		// If we've moved through the right side and still
		// need to carry a digit, move to the left side.
		if (i == 0 && right[i] >= 10)
		{
			right[i] = 0;

			// If there are no digits in the left side,
			// the extra digit will account for the carry.
			if (l_size == 0)
				return 1;

			// Move through the left side, incrementing
			// the digits until they are all less than 10.
			for (j = l_size - 1; j > 0 && left[j] >= 10; j--)
			{
				left[j] = 0;
				left[j - 1]++;
			}

			// If we still need to carry,
			// then an extra digit is needed.
			if (j == 0 && left[j] >= 10)
			{
				left[j] = 0;
				return 1;
			}
		}
	}
	else if (side == SN_LEFT)
	{
		// Move through the left side, incrementing
		// the digits until they are all less than 10.
		for (i = ind; i > 0 && left[i] >= 10; i--)
		{
			left[i] = 0;
			left[i - 1]++;
		}

		// If we still need to carry,
		// then an extra digit is needed.
		if (i == 0 && left[i] >= 10)
		{
			left[i] = 0;
			return 1;
		}
	}

	return 0;
}

static int round(uint8_t* left,
	size_t l_size,
	uint8_t* right,
	size_t r_size,
	size_t prec)
{
	size_t i, j; // Indices


	// Only procede if there are digits available.
	if (!l_size && !r_size)
		return 0;


	// If the precision is greater than the number of
	// digits after the radix, then there's no need to round
	if (prec >= r_size)
		return 0;


	// If the precision is 0 and there are no digits to the right
	// of the radix point, then we don't need to do anything.
	if (!prec && !r_size)
		return 0;


	// If the precision is 0 and there are no digits to the left
	// of the radix point, but there are digits to the right,
	// then we simply add an extra digit to the output based
	// on the value of the first digit on the right side.
	if (!prec && !l_size)
		return right[0] > 5 ? 1 : 0;


	// If the precision is 0 and there are digits to the left of
	// the radix point, round based on the value of the first
	// digits to the right of the radix point.
	if (!prec && l_size)
	{
		i = l_size - 1;
		if (right[0] > 5 || (right[0] == 5 && (left[i] & 1)))
		{
			left[i]++;

			if (left[i] >= 10)
				return carry(left, l_size, right, r_size, i, SN_LEFT);

			else
				return 0;
		}
	}

	// If the element at the index precision - 1 of the right side
	// of the radix point meets the criteria for banker's rounding,
	// then perform the round.
	i = prec;
	j = i - 1;
	if (right[i] > 5 || (right[i] == 5 && (right[j] & 1)))
	{
		right[i] = 0;
		right[j]++;

		if (right[j] >= 10)
			return carry(left, l_size, right, r_size, j, SN_RIGHT);

		else
			return 0;
	}

	// If we made it this far, then there was nothing to do.
	return 0;
}

static int round_left(uint8_t* left,
	size_t l_size,
	uint8_t* right,
	size_t r_size,
	size_t prec)
{
	size_t i, j; // Indices

	// Only procede if there are digits available, and
	// the precision is greter than 0.
	if (!prec || !l_size)
		return 0;


	// If the precision is greater than the number of
	// digits before the radix, then there's no need to round
	if (prec >= l_size)
		return 0;


	// If the element at the index precision - 1 of the right side
	// of the radix point meets the criteria for banker's rounding,
	// then perform the round.
	if (r_size)
	{
		if (right[0] > 5 || (right[0] == 5 && (left[prec - 1] & 1)))
		{
			right[0] = 0;
			left[prec - 1]++;

			if (left[prec - 1] >= 10)
				return carry(left, l_size, NULL, 0, prec - 1, SN_LEFT);
			else
				return 0;
		}
	}
	else if (l_size > 1)
	{
		i = prec;
		j = i - 1;
		if (left[i] > 5 || (left[i] == 5 && (left[i - 1] & 1)))
		{
			left[i] = 0;
			left[j]++;

			if (left[j] >= 10)
				return carry(left, l_size, NULL, 0, j, SN_LEFT);

			else
				return 0;
		}
	}

	// If we made it this far, then there was nothing to do.
	return 0;
}




static void reverse(char* str, size_t len)
{
	size_t beg; // beginning of string
	size_t end; // end of string
	char tmp;   // temporary character storage

	for (beg = 0, end = len - 1; beg < end;)
	{
		tmp = str[beg];
		str[beg++] = str[end];
		str[end--] = tmp;
	}
}

static size_t int_to_buffer(int n, char* buffer, ftag t)
{
	size_t i;       // index
	int radix;      // base (8, 10, or 16)
	int sign;       // whether or not the integer is signed
	int cap;        // whether to use capitalized hex digits
	int r;          // remainder
	int orig;       // original integer value
	unsigned int u; // unsigned integer

	// Ensure that the buffer is not a NULL pointer.
	if (buffer == NULL)
		return 0;

	// Only allow integer formats.
	if (t.spec == SPEC_o)
		radix = 8;

	else if (t.spec == SPEC_i || t.spec == SPEC_d || t.spec == SPEC_u)
		radix = 10;

	else if (t.spec == SPEC_X || t.spec == SPEC_x || t.spec == SPEC_p)
		radix = 16;

	else return 0;


	// Save the original value of n.
	orig = n;

	// Check for capitalization.
	cap = t.spec == SPEC_X;

	// Check whether we're dealing with a signed integer.
	sign = t.spec == SPEC_d || t.spec == SPEC_i;

	// Convert the integer to an unsigned integer
	// for the purpose of evaluating the individual digits.
	u = sign ? (n < 0 ? -n : n) : (unsigned int)n;

	i = 0;

	// If the integer is 0, we only need one digit,
	// so we populate the array accordingly and return.
	if (n == 0)
	{
		buffer[i++] = '0';
	}

	while (u)
	{
		r = u % radix;
		buffer[i++] = r > 9 ? r - 10 + (cap ? 'A' : 'a') : r + '0';
		u /= radix;
	}

	// If the format tag contains the '#' flag,
	// then we add the lead '0' or "0x" depending on the radix.
	if (t.flags & FMT_POINT)
	{
		if (radix == 16)
		{
			buffer[i++] = cap ? 'X' : 'x';
			buffer[i++] = '0';
		}
		else if (radix == 8)
			buffer[i++] = '0';
	}

	// Append the negative sign if necessary.
	if (orig < 0 && sign)
		buffer[i++] = '-';

	// Terminate the string with the NUL character.
	buffer[i] = '\0';

	// The resulting string is backwards, so we need to reverse it.
	reverse(buffer, i);

	return i;
}

static size_t int64_to_buffer(int64_t n, char* buffer, ftag t)
{
	size_t i;       // index
	int radix;      // base (8, 10, or 16)
	int sign;       // whether or not the integer is signed
	int cap;        // whether to use capitalized hex digits
	int r;          // remainder
	int orig;       // original integer value
	uint64_t u;     // unsigned integer

	// Ensure that the buffer is not a NULL pointer.
	if (buffer == NULL)
		return 0;

	// Only allow integer formats.
	if (t.spec == SPEC_o)
		radix = 8;

	else if (t.spec == SPEC_i || t.spec == SPEC_d || t.spec == SPEC_u)
		radix = 10;

	else if (t.spec == SPEC_X || t.spec == SPEC_x || t.spec == SPEC_p)
		radix = 16;

	else return 0;


	// Save the original value of n.
	orig = n;

	// Check for capitalization.
	cap = t.spec == SPEC_X;

	// Check whether we're dealing with a signed integer.
	sign = t.spec == SPEC_d || t.spec == SPEC_i;

	// Convert the integer to an unsigned integer
	// for the purpose of evaluating the individual digits.
	u = sign ? (n < 0 ? -n : n) : (uint64_t)n;

	i = 0;

	// If the integer is 0, we only need one digit,
	// so we populate the array accordingly and return.
	if (n == 0)
	{
		buffer[i++] = '0';
	}

	while (u)
	{
		r = u % radix;
		buffer[i++] = r > 9 ? r - 10 + (cap ? 'A' : 'a') : r + '0';
		u /= radix;
	}

	// If the format tag contains the '#' flag,
	// then we add the lead '0' or "0x" depending on the radix.
	if (t.flags & FMT_POINT)
	{
		if (radix == 16)
		{
			buffer[i++] = cap ? 'X' : 'x';
			buffer[i++] = '0';
		}
		else if (radix == 8)
			buffer[i++] = '0';
	}

	// Append the negative sign if necessary.
	if (orig < 0 && sign)
		buffer[i++] = '-';

	// Terminate the string with the NUL character.
	buffer[i] = '\0';

	// The resulting string is backwards, so we need to reverse it.
	reverse(buffer, i);

	return i;
}

static size_t size_to_buffer(size_t n, char* buffer)
{
	size_t i; // index
	int r;    // remainder

	// Ensure that the buffer is not a NULL pointer
	if (buffer == NULL)
		return 0;

	i = 0;

	// If the integer is 0, we only need one digit,
	// so we populate the array accordingly and return.
	if (n == 0)
	{
		buffer[i++] = '0';
		buffer[i] = '\0';
		return 1;
	}

	while (n)
	{
		r = n % 10;
		buffer[i++] = r + '0';
		n /= 10;
	}

	// Terminate the string with the NUL character.
	buffer[i] = '\0';

	// The resulting string is backwards, so we need to reverse it.
	reverse(buffer, i);

	return i;
}

static size_t uptr_to_buffer(uintptr_t n, char* buffer, int cap)
{
	size_t i; // index
	int r;    // remainder

	// Ensure that the buffer is not a NULL pointer
	if (buffer == NULL)
		return 0;

	i = 0;

	// If the integer is 0, we only need one digit,
	// so we populate the array accordingly and return.
	if (n == 0)
	{
		buffer[i++] = '0';
		buffer[i] = '\0';
		return 1;
	}

	while (n)
	{
		r = n % 16;
		buffer[i++] = r > 9 ? r - 10 + (cap ? 'A' : 'a') : r + '0';
		n /= 16;
	}

	// Terminate the string with the NUL character.
	buffer[i] = '\0';

	// The resulting string is backwards, so we need to reverse it.
	reverse(buffer, i);

	return i;
}

static size_t double_to_buffer(double d, char* buf, ftag t)
{
	size_t i, j;                   // indices
	uint8_t left[DOUBLE_BIN_DIG];  // left side of radix point
	uint8_t right[DOUBLE_BIN_DIG]; // right size of radix point
	size_t l_size;                 // number of digits on the left side
	size_t r_size;                 // number of digits on the right side
	int extra;                     // whether or not to write an extra digit
	size_t ls, rs;                 // left and right significant figures

	// Default values.
	i = l_size = r_size = ls = rs = 0;

	// Default the precision to 6.
	if (t.prec == 0 && !(t.flags & FMT_ZPREC))
		t.prec = 6;

	// Expand the double into two arrays of decimal digits.
	expand_double(d, left, &l_size, right, &r_size);

	extra = round(left, l_size, right, r_size, t.prec);

	// Put the sign in the buffer if necessary.
	if (d < 0 || (t.flags & FMT_SIGN))
		buf[i++] = d < 0 ? '-' : '+';

	// Put the first digit in the buffer.
	if (extra)
		buf[i++] = '1';

	else if (l_size == 0)
		buf[i++] = '0';

	// Add the digits to the left of the radix
	// point to the buffer.
	for (j = 0; j < l_size && i < BUF_LIMIT - 1; j++)
		buf[i++] = left[j] + '0';

	// Add the radix point to the buffer.
	if (i < BUF_LIMIT - 1 && (r_size || (t.flags & FMT_POINT)))
		buf[i++] = '.';

	// Add the digits to the right of the radix
	// point to the buffer.
	for (j = 0; j < r_size && j < t.prec && i < BUF_LIMIT - 1; j++)
		buf[i++] = right[j] + '0';

	// Pad the digits after the radix point
	// with trailing zeros to match the precision.
	for (; j < t.prec && i < BUF_LIMIT - 1; j++)
		buf[i++] = '0';


// Terminate the string with the NUL character.
	buf[i] = '\0';

	return i;
}

static size_t doublesn_to_buffer(double d, char* buf, ftag t)
{
	size_t i, j;                   // indices
	size_t len;                    // conversion result size
	char sbuf[BUF_LIMIT];          // conversion buffer
	uint8_t left[DOUBLE_BIN_DIG];  // left side of radix point
	uint8_t right[DOUBLE_BIN_DIG]; // right size of radix point
	size_t l_size;                 // number of digits on the left side
	size_t r_size;                 // number of digits on the right side
	size_t exp;                    // exponent
	size_t count;                  // precision padding
	int neg;                       // whether the exponent is negative
	int extra;                     // whether to write an extra digit
	//size_t ls, rs;                 // significant figure counts

	//ls = rs = 0;

	// Default the precision to 6.
	if (t.prec == 0 && !(t.flags & FMT_ZPREC))
		t.prec = 6;

	// Expand the double into two arrays of decimal digits.
	expand_double(d, left, &l_size, right, &r_size);

	// Determine if the rounding digit is on the left or right side.
	// In this context, i is the index on the left side and j is the
	// index on the right side.
	for (i = 0; i < l_size && i < t.prec; i++);
	for (j = 0; j < r_size && j + i <= t.prec; j++);


	if (i < l_size && i > 0)
		extra = round_left(left, l_size, right, r_size, t.prec);

	else if (j < r_size)
		extra = round(left, l_size, right, r_size, j);

	else
		extra = 0;


	exp = count = 0;
	neg = 0;
	i = 0;

	// Put the sign in the buffer if necessary.
	if (d < 0 || (t.flags & FMT_SIGN))
		buf[i++] = d < 0 ? '-' : '+';

		// Put the first digit in the buffer.
	if (extra)
		buf[i++] = '1';

	else if (l_size > 0)
		buf[i++] = left[0] + '0';

	else if (r_size > 0 && i < BUF_LIMIT - 1)
	{
		neg = 1;
		exp++;
		for (j = 0; j < r_size && right[j] == 0; j++)
			exp++;

		buf[i++] = right[j] + '0';
	}
	else
		buf[i++] = '0';

	// Put the radix point in the buffer.
	if (t.prec && i < BUF_LIMIT - 1)
		buf[i++] = '.';

	// Put the rest of the left side in the buffer.
	if (l_size > 1)
	{
		for (j = 1; j < l_size && count < t.prec && i < BUF_LIMIT - 1; j++)
		{
			buf[i++] = left[j] + '0';
			exp++;
			count++;
		}
	}

	// Put the rest of the right side in the buffer.
	if (r_size > 0 && l_size > 0)
	{
		for (j = 0; j < r_size && count < t.prec && i < BUF_LIMIT - 1; j++)
		{
			buf[i++] = right[j] + '0';
			exp++;
			count++;
		}
	}
	else if (r_size > 1)
	{
		for (j = exp; j < r_size && count < t.prec && i < BUF_LIMIT - 1; j++)
		{
			buf[i++] = right[j] + '0';
			count++;
		}
	}
	else if (i < BUF_LIMIT - 1)
		buf[i++] = '0';


	// Fill the remainder of the digits with zeros
	// to meet the precision requirement.
	if (count < t.prec)
	{
		for (; count < t.prec && i < BUF_LIMIT - 1; count++)
			buf[i++] = '0';
	}


	// Put the 'E' or 'e' character in the buffer.
	if ((t.spec == SPEC_E || t.spec == SPEC_G) && i < BUF_LIMIT - 1)
		buf[i++] = 'E';

	else if (i < BUF_LIMIT - 1)
		buf[i++] = 'e';


	// Determine the value of the exponent.
	if (l_size > 1)
		exp = l_size - 1;
	else if (l_size == 1)
		exp = 0;

	if (l_size >= 1 && extra)
		exp++;
	else if (neg && extra)
	{
		if (exp > 0)
			exp--;
		else
		{
			exp++;
			neg = 0;
		}
	}

	// Convert the exponent value to a string.
	len = size_to_buffer(exp, sbuf);

	// Put the exponent sign in the buffer.
	if (i < BUF_LIMIT - 1)
		buf[i++] = neg ? '-' : '+';

	if (len < 2 && i < BUF_LIMIT - 1)
		buf[i++] = '0';

	// Put the exponent value in the buffer
	for (j = 0; j < len && i < BUF_LIMIT - 1; j++)
		buf[i++] = sbuf[j];

	// Terminate the string with the NUL character.
	buf[i] = '\0';

	return i;
}

static size_t parse_udec(const char* str, size_t* res)
{
	size_t count, n, tmp, err;

	count = n = err = 0;

	while (*str >= '0' && *str <= '9' && !err)
	{
		tmp = (size_t)(*str) - '0';

		if (n == 0 && count == 1)
		{
			err = 1;
		}
		else
		{
			n = n * 10 + tmp;

			str += sizeof(char);
			count++;
		}
	}

	*res = n;

	return count;
}




/**
 * Writes the contents of a character array to stdout using constraints
 * provided by a format tag.
 * The character array is expected to be a numerical value that has been
 * converted to a string.
 *
 * Params:
 *   char* - a pointer to a character array
 *   size_t - the length of the array
 *   ftag - the format tag
 */
static void print_num(FILE* stream, char* buf, size_t len, ftag t)
{
	int neg;     // whether or not the number is negative
	size_t i;    // index
	size_t d;    // width deifference
	size_t plen; // pointer length
	int point;   // whether the '#' flag is present
	int radix;   // the base (8, 10, or 16)

	neg = 0;
	plen = sizeof(uintptr_t) * 2;
	point = t.flags & FMT_POINT;

	if (t.spec == SPEC_o)
		radix = 8;

	else if (t.spec == SPEC_i || t.spec == SPEC_d || t.spec == SPEC_u)
		radix = 10;

	else if (t.spec == SPEC_X || t.spec == SPEC_x || t.spec == SPEC_p)
		radix = 16;

	else radix = 10;

	// Print the negative sign '-'
	if (len && buf[0] == '-')
	{
		neg = 1;
		fputc(buf[0], stream);
		t.prec++;
	}

	// Print the positive sign '+'
	if (len && buf[0] != '-' && (t.flags & FMT_SIGN))
	{
		fputc('+', stream);
		t.prec++;
		if (t.width)
			t.width--;
	}

	// Print a leading blank space if the space flag is set
	if (len && buf[0] != '-' && !(t.flags & FMT_SIGN) && (t.flags & FMT_SPACE))
	{
		fputc((t.flags & FMT_ZERO) ? '0' : ' ', stream);
		t.prec++;
		if (t.width)
			t.width--;
	}

	// Pad with leading spaces if the '-' flag is not present
	if (len < t.width && !(t.flags & FMT_LEFT))
	{
		if ((t.prec < t.width) && (t.spec != SPEC_p))
		{
			d = len > t.prec ? len : t.prec;
			for (i = 0; i < t.width - d; i++)
				fputc(' ', stream);
		}
		else if (t.spec == SPEC_p && plen < t.width)
		{
			for (i = 0; i < t.width - plen; i++)
				fputc(' ', stream);
		}
	}

	// Print the leading "0x" or "0X"
	if (t.flags & FMT_POINT)
	{
		if (t.spec == SPEC_X || t.spec == SPEC_x)
		{
			fputc(buf[0], stream);
			fputc(buf[1], stream);
			t.prec += 2;
		}
	}

	// Print leading zeros for pointers
	if (t.spec == SPEC_p && len < plen)
	{
		for (i = 0; i < plen - len; i++)
			fputc('0', stream);
	}

	// Pad with '0' for the precision
	if (len < t.prec && t.spec != SPEC_p)
	{
		for (i = len; i < t.prec; i++)
			fputc('0', stream);
	}

	// Determine the starting index for printing.
	if (neg)
		i = 1;

	else if (point && radix == 16 && t.spec != SPEC_p)
		i = 2;

	else i = 0;

	// Print the contents of the buffer.
	for (; i < len; i++)
		fputc(buf[i], stream);

	// Pad with trailing spaces if the '-' flag is present
	if (len < t.width && (t.flags & FMT_LEFT))
	{
		if ((t.prec < t.width) && (t.spec != SPEC_p))
		{
			d = len > t.prec ? len : t.prec;
			for (i = 0; i < t.width - d; i++)
				fputc(' ', stream);
		}
		else if (t.spec == SPEC_p && plen < t.width)
		{
			for (i = 0; i < t.width - plen; i++)
				fputc(' ', stream);
		}
	}
}




//--------------------------------------------------------------------------//
//                               Public API                                 //
//--------------------------------------------------------------------------//

int putc(int c, FILE* stream)
{
	return fputc(c, stream);
}

int fputc(int c, FILE* stream)
{
	// Here is where we perform a write.
    switch (stream->type)
	{
		// TODO: add logic to determine output context
		case __FILE_NO_STDOUT:
		case __FILE_NO_STDERR:
			vga_putchar((char)(c & 0xFF));
			return 1;

		case __FILE_NO_STDDBG:
			com1_write((uint8_t)(c & 0xFF));
			return 1;

		default:
			return 0;
	}
}

int putchar(int c)
{
	return fputc(c, stdout);
}

int printf(const char* fmt, ...)
{
	va_list argp;         // argument pointer
	size_t i, j;          // index
	char* end;            // updated character pointer
	char buf[BUF_LIMIT];  // string conversion buffer
	size_t len;           // string length
	int err;              // error flag

	// Values to be printed
	int n;          // integer
    int n64;        // 64-bit integer
	unsigned int u; // unsigned integer
    uint64_t u64;   // unsigned 64-bit integer
	uintptr_t p;    // unsigned pointer
	double d;       // double

	va_start(argp, fmt);

	i = j = 0;
	err = 0;
	while (*fmt != '\0' && !err)
	{
		if (*fmt == '%')
		{
			fmt++;
			ftag t = parse_format(fmt, &end);
			fmt = end;

			if (t.flags & FMT_WIDTH)
				t.width = va_arg(argp, size_t);

			if (t.flags & FMT_PREC)
				t.prec = va_arg(argp, size_t);

			if (t.spec == SPEC_c)
			{
				char c = va_arg(argp, int);
				putchar(c);
			}
			else if (t.spec == SPEC_s)
			{
				char* s = va_arg(argp, char*);
				size_t len = strlen(s);

				for (i = 0; i < len; i++)
					putchar(s[i]);
			}

			// signed decimal integers
			else if (t.spec == SPEC_d || t.spec == SPEC_i)
			{
				if (t.len & LEN_ll)
                {
                    n64 = va_arg(argp, int64_t);
                    len = int64_to_buffer(n64, buf, t);
                }
                else
                {
                    n = va_arg(argp, int);
				    len = int_to_buffer(n, buf, t);
                }

				print_num(stdout, buf, len, t);
			}

			// unsigned decimal integers
			else if (t.spec == SPEC_u)
			{
                if (t.len & LEN_ll)
                {
                    u64 = va_arg(argp, uint64_t);
                    len = int64_to_buffer(u64, buf, t);
                }
                else
                {
                    u = va_arg(argp, unsigned int);
				    len = int_to_buffer(u, buf, t);
                }

				print_num(stdout, buf, len, t);
			}

			// unsigned hexadecimal integers
			else if (t.spec == SPEC_X || t.spec == SPEC_x)
			{
				if (t.len & LEN_ll)
                {
                    u64 = va_arg(argp, int64_t);
                    len = int64_to_buffer(u64, buf, t);
                }
                else
                {
                    u = va_arg(argp, int);
				    len = int_to_buffer(u, buf, t);
                }

				print_num(stdout, buf, len, t);
			}

			// signed octal integers
			else if (t.spec == SPEC_o)
			{
				if (t.len & LEN_ll)
                {
                    u64 = va_arg(argp, int64_t);
                    len = int64_to_buffer(u64, buf, t);
                }
                else
                {
                    u = va_arg(argp, int);
				    len = int_to_buffer(u, buf, t);
                }

				print_num(stdout, buf, len, t);
			}

			// pointers
			else if (t.spec == SPEC_p)
			{
				p = va_arg(argp, uintptr_t);
				len = uptr_to_buffer(p, buf, 1);

				print_num(stdout, buf, len, t);
			}

			else if (t.spec == SPEC_f)
			{
				d = va_arg(argp, double);
				len = double_to_buffer(d, buf, t);

				print_num(stdout, buf, len, t);
			}

			else if (t.spec == SPEC_E || t.spec == SPEC_e)
			{
				d = va_arg(argp, double);
				len = doublesn_to_buffer(d, buf, t);

				print_num(stdout, buf, len, t);
			}
			else if (t.spec == SPEC_G || t.spec == SPEC_g)
			{
				// TODO: implement this
			}
			else if (t.spec == SPEC_n)
			{
				// TODO: implement this
			}
			else
			{
				// invalid specifier
				err = 1;
			}
		}
		else
		{
			putchar(*fmt);
		}

		fmt++;
	}

	va_end(argp);

	return 1;
}

int fprintf(FILE* stream, const char* fmt, ...)
{
	va_list argp;         // argument pointer
	size_t i, j;          // index
	char* end;            // updated character pointer
	char buf[BUF_LIMIT];  // string conversion buffer
	size_t len;           // string length
	int err;              // error flag

	// Values to be printed
	int n;          // integer
    int n64;        // 64-bit integer
	unsigned int u; // unsigned integer
    uint64_t u64;   // unsigned 64-bit integer
	uintptr_t p;    // unsigned pointer
	double d;       // double

	va_start(argp, fmt);

	i = j = 0;
	err = 0;
	while (*fmt != '\0' && !err)
	{
		if (*fmt == '%')
		{
			fmt++;
			ftag t = parse_format(fmt, &end);
			fmt = end;

			if (t.flags & FMT_WIDTH)
				t.width = va_arg(argp, size_t);

			if (t.flags & FMT_PREC)
				t.prec = va_arg(argp, size_t);

			if (t.spec == SPEC_c)
			{
				char c = va_arg(argp, int);
				fputc(c, stream);
			}
			else if (t.spec == SPEC_s)
			{
				char* s = va_arg(argp, char*);
				size_t len = strlen(s);

				for (i = 0; i < len; i++)
					fputc(s[i], stream);
			}

			// signed decimal integers
			else if (t.spec == SPEC_d || t.spec == SPEC_i)
			{
				if (t.len & LEN_ll)
                {
                    n64 = va_arg(argp, int64_t);
                    len = int64_to_buffer(n64, buf, t);
                }
                else
                {
                    n = va_arg(argp, int);
				    len = int_to_buffer(n, buf, t);
                }

				print_num(stream, buf, len, t);
			}

			// unsigned decimal integers
			else if (t.spec == SPEC_u)
			{
                if (t.len & LEN_ll)
                {
                    u64 = va_arg(argp, uint64_t);
                    len = int64_to_buffer(u64, buf, t);
                }
                else
                {
                    u = va_arg(argp, unsigned int);
				    len = int_to_buffer(u, buf, t);
                }

				print_num(stream, buf, len, t);
			}

			// unsigned hexadecimal integers
			else if (t.spec == SPEC_X || t.spec == SPEC_x)
			{
				if (t.len & LEN_ll)
                {
                    u64 = va_arg(argp, int64_t);
                    len = int64_to_buffer(u64, buf, t);
                }
                else
                {
                    u = va_arg(argp, int);
				    len = int_to_buffer(u, buf, t);
                }

				print_num(stream, buf, len, t);
			}

			// signed octal integers
			else if (t.spec == SPEC_o)
			{
				if (t.len & LEN_ll)
                {
                    u64 = va_arg(argp, int64_t);
                    len = int64_to_buffer(u64, buf, t);
                }
                else
                {
                    u = va_arg(argp, int);
				    len = int_to_buffer(u, buf, t);
                }

				print_num(stream, buf, len, t);
			}

			// pointers
			else if (t.spec == SPEC_p)
			{
				p = va_arg(argp, uintptr_t);
				len = uptr_to_buffer(p, buf, 1);

				print_num(stream, buf, len, t);
			}

			else if (t.spec == SPEC_f)
			{
				d = va_arg(argp, double);
				len = double_to_buffer(d, buf, t);

				print_num(stream, buf, len, t);
			}

			else if (t.spec == SPEC_E || t.spec == SPEC_e)
			{
				d = va_arg(argp, double);
				len = doublesn_to_buffer(d, buf, t);

				print_num(stream, buf, len, t);
			}
			else if (t.spec == SPEC_G || t.spec == SPEC_g)
			{
				// TODO: implement this
			}
			else if (t.spec == SPEC_n)
			{
				// TODO: implement this
			}
			else
			{
				// invalid specifier
				err = 1;
			}
		}
		else
		{
			fputc(*fmt, stream);
		}

		fmt++;
	}

	va_end(argp);

	return 1;
}
