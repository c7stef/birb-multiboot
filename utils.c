#include "utils.h"
#include "screen.h"

void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);

size_t strlen(const char* str)
{
    size_t len = 0;
	while (str[len])
		len++;
	return len;
}

void strcat(char* dst, char const* src)
{
	dst += strlen(dst);
	while (*src)
		*dst++ = *src++;
	*dst = '\0';
}

void sleep(size_t ticks)
{
	volatile size_t i = 0;
	while (i < ticks * 1000)
		i++;
}

void micro_sleep(void)
{
	outb(0x80, 0);
}

noreturn void panic(char const* message)
{
    TermHandle term = { 0 };
    terminal_initialize(&term);

	Screen panic_screen = { 0 };
	uint8_t text_color = vga_entry_color(VgaColor_White, VgaColor_Red);
	char message_fatal[] = "FATAL: ";

	screen_clear(&panic_screen, VgaColor_Red);
	screen_write_string(&panic_screen, message_fatal, 1, 1, text_color);
	screen_write_string(&panic_screen, message, sizeof message_fatal, 1, text_color);

	screen_display(&panic_screen, &term);

	hang();
}

noreturn void hang(void)
{
	__asm__ volatile ("cli; hlt");
    while (1);
}
