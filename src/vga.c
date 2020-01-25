#include "kernel/vga.h"
#include "kernel/port.h"

#include "libc/string.h"

// VGA terminal dimensions
#define WIDTH 80
#define HEIGHT 25

// Beginning of video memory for color monitors
#define BUFSTART 0xB8000

/**
 * Builds an 8-bit byte containing the foreground and background colors of
 * a single character.
 * The high four bits are the background color, and the low four bits
 * are the foreground color.
 *
 * Params:
 *   enum vga_color - the foreground color
 *   enum vga_color - the background color
 *
 * Returns:
 *   uint8_t - an 8-bit unsigned integer
 */
#define make_color(f, b) (f | b << 4)

/**
 * Builds a single VGA entry containing the character data and color.
 *
 * Params:
 *   unsigned ch - the chatacter to write
 *   uint8_t co - the color information
 *
 * Returns:
 *   uint16_t - VGA cell entry data
 */
#define make_entry(ch, co) ((uint16_t)ch | (uint16_t)co << 8)

static size_t row;
static size_t col;
static uint8_t color;
static uint16_t* buffer;

void k_vga_init(void)
{
	row = 0;
	col = 0;
	color = make_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	buffer = (uint16_t*)BUFSTART;

	for (size_t y = 0; y < HEIGHT; y++) {

		for (size_t x = 0; x < WIDTH; x++) {

			const size_t index = y * WIDTH + x;
			buffer[index] = make_entry(' ', color);
		}
	}

	// Enable the cursor
	//k_outb(0x3D4, 0x0A);
	//k_outb(0x3D5, 0x00);
	//k_outb(0x3D4, 0x0B);
	//k_outb(0x3D5, 0x0F);
}





void vga_set_cursor(uint8_t x, uint8_t y)
{
	uint16_t pos = y * WIDTH + x;
	k_outb(0x3D4, 0x0F);
	k_outb(0x3D5, (uint8_t)(pos & 0xFF));
	k_outb(0x3D4, 0x0E);
	k_outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}



void vga_set_color(uint8_t c)
{
	color = c;
}

void vga_put(char c, uint8_t color, size_t x, size_t y)
{
	const size_t index = y * WIDTH + x;
	buffer[index] = make_entry(c, color);
}

void vga_putchar(char c)
{
	// Handle newlines.
	if (c == '\n')
	{
		if (row < HEIGHT - 1)
		{
			row++;
			col = 0;
		}

		return;
	}

	// Handle carriage returns.
	if (c == '\r')
	{
		col = 0;

		return;
	}

	vga_put(c, color, col, row);

	if (++col == WIDTH) {

		col = 0;

		if (++row == HEIGHT)
			row = 0;
	}
}

void vga_write(const char* data, size_t size)
{
	for (size_t i = 0; i < size; i++)
		vga_putchar(data[i]);
}

void vga_writes(const char* data)
{
	vga_write(data, strlen(data));
}
