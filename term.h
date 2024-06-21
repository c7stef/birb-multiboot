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
#define VGA_HEIGHT 50

typedef struct TermHandle
{
	uint16_t* buffer;
} TermHandle;

uint8_t vga_entry_color(enum VgaColor fg, enum VgaColor bg);
uint16_t vga_entry(unsigned char uc, uint8_t color);

void terminal_change_mode(void);
void terminal_initialize(TermHandle* term);
void terminal_put_entry(TermHandle* term, uint16_t entry, size_t x, size_t y);

#endif // TERM_H