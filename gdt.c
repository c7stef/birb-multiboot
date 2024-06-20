#include <stdint.h>
#include <stddef.h>

#include "gdt.h"

#define GDT_ENTRY_SIZE 8
#define LSB(N) ((N) & 0xFF)

typedef struct GdtEntry
{
    uintptr_t base;
    uintptr_t limit;
    uint8_t access_byte;
    uint8_t flags;
} GdtEntry;

// Make room for 4 entries
uint8_t gdt[GDT_ENTRY_SIZE * 4];

void load_gdt(uint32_t gdt, uint16_t size);

static void gdt_encode_entry(uint8_t target[static GDT_ENTRY_SIZE], GdtEntry entry)
{
    // Encode the limit
    target[0] = LSB(entry.limit);
    target[1] = LSB(entry.limit >> 8);
    target[6] = LSB(entry.limit >> 16) & 0x0F;

    // Encode the base
    target[2] = LSB(entry.base);
    target[3] = LSB(entry.base >> 8);
    target[4] = LSB(entry.base >> 16);
    target[7] = LSB(entry.base >> 24);

    // Encode the access byte
    target[5] = entry.access_byte;

    // Encode the flags
    target[6] |= entry.flags << 4;
}

void gdt_init(void)
{
    GdtEntry entries[] = {
        { // Null descriptor
            .base =         0x00000000,
            .limit =        0x00000000,
            .access_byte =  0x00,
            .flags =        0x0
        },
        { // Code segment
            .base =         0x00000000,
            .limit =        0x003FFFFF,
            .access_byte =  0x9A,
            .flags =        0xC
        },
        { // Data segment
            .base =         0x00000000,
            .limit =        0x003FFFFF,
            .access_byte =  0x92,
            .flags =        0xC
        }
    };

    size_t entries_count = sizeof entries / sizeof entries[0];

    for (size_t i = 0; i < entries_count; i++)
        gdt_encode_entry(gdt + i * GDT_ENTRY_SIZE, entries[i]);

    load_gdt((uint32_t)&gdt, (uint16_t)sizeof gdt);
}
