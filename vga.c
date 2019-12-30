#include "vga.h"
#include "string.h"
#include "port.h"

// VGA terminal dimensions
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

// Beginning of video memory for color monitors
#define VGA_BUFFER_START 0xB8000

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
#define vga_make_color(f, b) (f | b << 4)

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
#define vga_make_entry(ch, co) ((uint16_t)ch | (uint16_t)co << 8)

static size_t vga_row;
static size_t vga_col;
static uint8_t vga_color;
static uint16_t* vga_buffer;

void vga_set_cursor(uint8_t x, uint8_t y)
{
	uint16_t pos = y * VGA_WIDTH + x;
	k_outb(0x3D4, 0x0F);
	k_outb(0x3D5, (uint8_t)(pos & 0xFF));
	k_outb(0x3D4, 0x0E);
	k_outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void vga_init(void)
{
	vga_row = 0;
	vga_col = 0;
	vga_color = vga_make_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	vga_buffer = (uint16_t*)VGA_BUFFER_START;

	for (size_t y = 0; y < VGA_HEIGHT; y++) {

		for (size_t x = 0; x < VGA_WIDTH; x++) {

			const size_t index = y * VGA_WIDTH + x;
			vga_buffer[index] = vga_make_entry(' ', vga_color);
		}
	}

	// Enable the cursor
	k_outb(0x3D4, 0x0A);
	k_outb(0x3D5, 0x00);
	k_outb(0x3D4, 0x0B);
	k_outb(0x3D5, 0x0F);
}

void vga_set_color(uint8_t color)
{
	vga_color = color;
}

void vga_put(char c, uint8_t color, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	vga_buffer[index] = vga_make_entry(c, color);
}

void vga_putchar(char c)
{
	if (c == 0x0A)
	{
		if (vga_row < VGA_HEIGHT - 1)
		{
			vga_row++;
			vga_col = 0;
		}

		return;
	}

	vga_put(c, vga_color, vga_col, vga_row);

	if (++vga_col == VGA_WIDTH) {

		vga_col = 0;

		if (++vga_row == VGA_HEIGHT)
			vga_row = 0;
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
