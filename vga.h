#ifndef VGA_H
#define VGA_H

/**
 * VGA text mode interface.
 * This is very heavily based on, and mostly copied from the OSDev wiki.
 */

#include <stdint.h>

// Hardware text mode color constants.
enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

/**
 * Sets the cursor position for the VGA context.
 *
 * Params:
 *   uint8_t - the column position
 *   uint8_t - the row position
 */
void vga_set_cursor(uint8_t x, uint8_t y);

/**
 * Builds an 8-bit byte containing the foreground and background colors of
 * a single character.
 * TODO: this function can probably be a macro
 *
 * Params:
 *   enum vga_color - the foreground color
 *   enum vga_color - the background color
 *
 * Returns:
 *   uint8_t - an 8-bit unsigned integer
 */
uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg);

/**
 * Builds a single VGA entry containing the character data and color.
 *
 * Params:
 *   unsigned char - the chatacter to write
 *   uint8_t - the color information
 */
uint16_t vga_entry(unsigned char uc, uint8_t color);

/**
 * Initializes the VGA context.
 */
void vga_init();

/**
 * Sets the color data for the current VGA context.
 *
 * Params:
 *   uint8_t - the foreground and background color
 */
void vga_set_color(uint8_t color);

/**
 * Makes an entry at a certain position in the current VGA output context.
 *
 * Params:
 *   char - a the character to write
 *   uint8_t - the color data
 *   size_t - the column position
 *   size_t - the row position
 */
void vga_put(char c, uint8_t color, size_t x, size_t y);

/**
 * Writes a single character to the current VGA output context.
 *
 * Params:
 *   char - a character
 */
void vga_putchar(char c);

/**
 * Writes a sequence of character data to the current VGA output context.
 *
 * Params:
 *   const char* - a pointer to an array of character data
 *   size_t - the number of characters in the data
 */
void vga_write(const char* data, size_t n);

/**
 * Writes a NUL-terminated character sequence to the current VGA output
 * context.
 *
 * Params:
 *   const char* - a pointer to a NUL-terminated character array
 */
void vga_writes(const char* data);

#endif
