#include <stdbool.h>
#include <stddef.h>

#include "gdt.h"
#include "idt.h"
#include "pic.h"
#include "birb.h"
#include "term.h"
#include "keyboard.h"

void kmain(void) 
{
	TermHandle term = { 0 };

	terminal_initialize(&term);
	terminal_set_bg_color(&term, VgaColor_Black);
	terminal_clear(&term);

	gdt_init();
	idt_init();
	pic_init();
	keyboard_init();
	enable_interrupts();

	Birb birb = { 0 };
	birb_initialize(&birb);

	birb.position = (Vec2) {5, 5};

	while (1) {
		birb.position.y++;

		terminal_clear(&term);
		birb_render(&birb, &term);

		sleep(50000);
	}
}

