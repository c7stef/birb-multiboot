#ifndef SCREEN_H
#define SCREEN_H

#include "term.h"

typedef struct Screen {
    uint16_t pixels[VGA_HEIGHT][VGA_WIDTH];
} Screen;

void screen_display(Screen* screen, TermHandle* term);
void screen_clear(Screen* screen, VgaColor color);
void screen_draw_pixel(Screen* screen, VgaColor color, size_t x, size_t y);
void screen_draw_sprite_px(Screen* screen, char ch, size_t x, size_t y);
void screen_put_entry(Screen* screen, uint16_t entry, size_t x, size_t y);
void screen_write_string(Screen* screen, char const* string, size_t x, size_t y, uint8_t color_entry);

#define __screen_write_string(SCR, STR, X, Y, COLOR, ...) screen_write_string(SCR, STR, X, Y, COLOR)
#define screen_write_string(SCR, STR, X, ...)               \
    __screen_write_string(SCR, STR, X, __VA_ARGS__,         \
    vga_entry_color(VgaColor_White, VgaColor_Black),        \
    /* ISO C99: at least one arg in "..." */ 0)

#endif // SCREEN_H
