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

	terminal_change_mode();
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

	birb.position = (Vec2) {40, 40};

	while (1) {
		Key key_pressed = keyboard_pop_key_pressed();

		if (key_pressed == Key_W)
			birb_jump(&birb);

		birb_update(&birb);

		terminal_clear(&term);
		birb_render(&birb, &term);

		sleep(5000);
	}
}

