#include <stdbool.h>
#include <stddef.h>

#include "gdt.h"
#include "idt.h"
#include "pic.h"
#include "birb.h"
#include "term.h"
#include "keyboard.h"
#include "screen.h"
#include "tunnel.h"
#include "ui.h"
#include "timer.h"

void kmain(void)
{
	TermHandle term = { 0 };

	terminal_change_mode();
	terminal_initialize(&term);

	gdt_init();
	idt_init();
	pic_init();
	keyboard_init();
	enable_interrupts();
	timer_init();

	Birb birb = { 0 };
	birb_initialize(&birb);

	TunnelPool tunnels = { 0 };
	tunnel_pool_initialize(&tunnels);

	Screen screen = { 0 };

	size_t score = 0;
	size_t dead = false;

	while (1) {
		Key key_pressed = keyboard_pop_key_pressed();

		if (dead) {
			if (key_pressed == Key_Return) {
				dead = false;
				score = 0;

				birb_initialize(&birb);
				tunnel_pool_initialize(&tunnels);
			}
		} else {
			if (key_pressed == Key_W)
				birb_jump(&birb);
			
			if (tunnel_pool_birb_passed(&tunnels, &birb))
				score++;

			if (birb_check_tunnel_collision(&birb, &tunnels)) {
				dead = true;
				continue;
			}

			birb_update(&birb);
			tunnel_pool_update(&tunnels);
		}

		screen_clear(&screen, VgaColor_Black);
		birb_render(&birb, &screen);
		tunnel_pool_render(&tunnels, &screen);
		score_render(score, &screen);

		if (dead)
			restart_render(&screen);
		
		screen_display(&screen, &term);

		timer_sleep_ticks(1);
	}
}

