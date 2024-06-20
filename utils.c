#include "utils.h"

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

	terminal_set_bg_color(&term, VgaColor_Red);
    terminal_clear(&term);

    terminal_move_to(&term, 1, 1);
	terminal_set_fg_color(&term, VgaColor_White);
    terminal_writestring(&term, "FATAL: ");
    terminal_writestring(&term, message);

	hang();
}

noreturn void hang(void)
{
	__asm__ volatile ("cli; hlt");
    while (1);
}
