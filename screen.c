#include "screen.h"

void screen_display(Screen* screen, TermHandle* term)
{
    for (size_t y = 0; y < VGA_HEIGHT; y++)
        for (size_t x = 0; x < VGA_WIDTH; x++)
            terminal_put_entry(term, screen->pixels[y][x], x, y);
}

void screen_clear(Screen* screen, VgaColor color)
{
    for (size_t y = 0; y < VGA_HEIGHT; y++)
        for (size_t x = 0; x < VGA_WIDTH; x++)
            screen_draw_pixel(screen, color, x, y);
}

void screen_draw_pixel(Screen* screen, VgaColor color, size_t x, size_t y)
{
    if (x < VGA_WIDTH && y < VGA_HEIGHT)
        screen->pixels[y][x] = vga_entry(' ', vga_entry_color(color, color));
}

void screen_draw_sprite_px(Screen* screen, char ch, size_t x, size_t y)
{
    if (ch == ' ')
        return;
    
    VgaColor color = vga_color_map[(size_t)ch];

    screen_draw_pixel(screen, color, x, y);
}

void screen_put_entry(Screen* screen, uint16_t entry, size_t x, size_t y)
{
    if (x < VGA_WIDTH && y < VGA_HEIGHT)
        screen->pixels[y][x] = entry;
}

void screen_write_string(Screen* screen, char const* string, size_t x, size_t y, uint8_t color_entry)
{
    while (*string)
        screen_put_entry(screen, vga_entry(*string++, color_entry), x++, y);
}

