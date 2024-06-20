#ifndef TERM_H
#define TERM_H

#include <stdint.h>
#include <stddef.h>

/* Hardware text mode color constants. */
typedef enum VgaColor {
	VgaColor_Black = 0,
	VgaColor_Blue = 1,
	VgaColor_Green = 2,
	VgaColor_Cyan = 3,
	VgaColor_Red = 4,
	VgaColor_Magenta = 5,
	VgaColor_Brown = 6,
	VgaColor_LightGrey = 7,
	VgaColor_DarkGrey = 8,
	VgaColor_LightBlue = 9,
	VgaColor_LightGreen = 10,
	VgaColor_LightCyan = 11,
	VgaColor_LightRed = 12,
	VgaColor_LightMagenta = 13,
	VgaColor_LightBrown = 14,
	VgaColor_White = 15,
} VgaColor;

extern VgaColor vga_color_map[];

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

typedef struct TermHandle
{
	size_t row;
	size_t col;
	VgaColor color_fg;
	VgaColor color_bg;
	uint16_t* buffer;
} TermHandle;

uint8_t vga_entry_color(enum VgaColor fg, enum VgaColor bg);
uint16_t vga_entry(unsigned char uc, uint8_t color);
void terminal_setcolor(TermHandle* term, uint8_t color);
void terminal_putentryat(TermHandle* term, uint16_t entry, size_t x, size_t y);
void terminal_putchar(TermHandle* term, char c);
void terminal_initialize(TermHandle* term);
void terminal_clear(TermHandle* term);
void terminal_write(TermHandle* term, const char* data, size_t size);
void terminal_writestring(TermHandle* term, const char* data);
void terminal_draw_pixel(TermHandle* term, VgaColor color, size_t x, size_t y);
void terminal_move_to(TermHandle* term, size_t x, size_t y);
void terminal_set_fg_color(TermHandle* term, VgaColor fg);
void terminal_set_bg_color(TermHandle* term, VgaColor bg);

#endif // TERM_H