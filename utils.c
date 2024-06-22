#include "utils.h"
#include "screen.h"

// Declarations so compiler emits symbols for inline functions
void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);

Vec2 vec2_decimal_to_pixel(Vec2 decimal)
{
	return (Vec2) {
		decimal_to_pixel(decimal.x),
		decimal_to_pixel(decimal.y)
	};
}

Vec2 vec2_pixel_to_decimal(Vec2 pixel)
{
	return (Vec2) {
		pixel_to_decimal(pixel.x),
		pixel_to_decimal(pixel.y)
	};
}

int decimal_to_pixel(int decimal)
{
	return decimal / DECIMAL_UNIT;
}

int pixel_to_decimal(int pixel)
{
	return pixel * DECIMAL_UNIT;
}


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

static unsigned long next = 1;

int rand(void)
{
    next = next * 1103515245 + 12345;
    return (unsigned int) (next / 65536) % 32768;
}

void srand(unsigned int seed)
{
    next = seed;
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
