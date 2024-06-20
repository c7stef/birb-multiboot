#include <stddef.h>

#include "term.h"
#include "utils.h"
#include "int32.h"

VgaColor vga_color_map[] = {
	[' '] = VgaColor_Black,
	['B'] = VgaColor_Brown,
	['W'] = VgaColor_White,
	['D'] = VgaColor_DarkGrey,
	['G'] = VgaColor_LightGrey,
};

void terminal_change_mode(void)
{
	regs16_t regs;
	regs.ax = 0x1112;
	int32(0x10, &regs);
}

uint8_t vga_entry_color(enum VgaColor fg, enum VgaColor bg) 
{
	return fg | bg << 4;
}

uint16_t vga_entry(unsigned char uc, uint8_t color) 
{
	return (uint16_t) uc | (uint16_t) color << 8;
}

static uint8_t terminal_get_color(TermHandle* term)
{
	return vga_entry_color(term->color_fg, term->color_bg);
}

void terminal_putentryat(TermHandle* term, uint16_t entry, size_t x, size_t y) 
{
	size_t const index = y * VGA_WIDTH + x;
	term->buffer[index] = entry;
}

void terminal_putchar(TermHandle* term, char c) 
{
	terminal_putentryat(term, vga_entry(c, terminal_get_color(term)), term->col, term->row);

	if (++term->col == VGA_WIDTH) {
		term->col = 0;
		if (++term->row == VGA_HEIGHT)
			term->row = 0;
	}
}

void terminal_initialize(TermHandle* term) 
{
	*term = (TermHandle) {
		.row = 0,
		.col = 0,
		.color_fg = VgaColor_LightGrey,
		.color_bg = VgaColor_Black,
		.buffer = (uint16_t*) 0xB8000
	};
}

void terminal_clear(TermHandle* term) {
	uint16_t entry = vga_entry(' ', vga_entry_color(term->color_fg, term->color_bg));

	for (size_t y = 0; y < VGA_HEIGHT; y++)
		for (size_t x = 0; x < VGA_WIDTH; x++)
			terminal_putentryat(term, entry, x, y);
}

void terminal_write(TermHandle* term, const char* data, size_t size) 
{
	for (size_t i = 0; i < size; i++)
		terminal_putchar(term, data[i]);
}

void terminal_writestring(TermHandle* term, const char* data) 
{
	terminal_write(term, data, strlen(data));
}

void terminal_draw_pixel(TermHandle* term, VgaColor color, size_t x, size_t y)
{
	uint16_t entry = vga_entry(' ', vga_entry_color(VgaColor_Black, color));
	
	terminal_putentryat(term, entry, x	, y);
}

void terminal_move_to(TermHandle* term, size_t x, size_t y)
{
	term->row = y;
	term->col = x;
}

void terminal_set_fg_color(TermHandle* term, VgaColor fg)
{
	term->color_fg = fg;
}

void terminal_set_bg_color(TermHandle* term, VgaColor bg)
{
	term->color_bg = bg;
}
