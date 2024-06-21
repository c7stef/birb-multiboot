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

	// [CURSED BLACK MAGIC] SET FOOT IN VM86, CONJURE BIOS
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

void terminal_put_entry(TermHandle* term, uint16_t entry, size_t x, size_t y) 
{
	size_t const index = y * VGA_WIDTH + x;
	term->buffer[index] = entry;
}

void terminal_initialize(TermHandle* term) 
{
	*term = (TermHandle) {
		.buffer = (uint16_t*) 0xB8000
	};
}
