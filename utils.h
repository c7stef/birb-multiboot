#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>
#include <stdnoreturn.h>

#include "term.h"

#define RAND_MAX 32767
#define DECIMAL_UNIT 10

typedef struct Vec2 {
    int x;
    int y;
} Vec2;

Vec2 vec2_decimal_to_pixel(Vec2 decimal);
Vec2 vec2_pixel_to_decimal(Vec2 pixel);
int decimal_to_pixel(int decimal);
int pixel_to_decimal(int pixel);

size_t strlen(const char* str);
void strcat(char* dst, char const* src);
void sleep(size_t ticks);
void micro_sleep(void);

int rand(void);
void srand(unsigned int seed);

noreturn void panic(char const* message);
noreturn void hang(void);

inline void outb(uint16_t port, uint8_t val)
{
    __asm__ volatile ( "outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    __asm__ volatile ( "inb %w1, %b0"
                   : "=a"(ret)
                   : "Nd"(port)
                   : "memory");
    return ret;
}

#endif // UTILS_H