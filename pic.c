#include <stdint.h>
#include <stdbool.h>

#include "pic.h"
#include "utils.h"

#define PIC1		    0x20
#define PIC2		    0xA0
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	    (PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	    (PIC2+1)
#define PIC_EOI		    0x20

// ICW4 will be present
#define ICW1_ICW4	0x01

// Initialization code
#define ICW1_INIT	0x10

// 8086/88 mode
#define ICW4_8086	0x01

#define outb_delay(P, V) do { outb((P), (V)); micro_sleep(); } while (false)

static void pic_remap(size_t offset_master, size_t offset_slave)
{
    uint8_t mask_m = inb(PIC1_DATA);
    uint8_t mask_s = inb(PIC2_DATA);
    
    // ICW1: Start initialization sequence
    outb_delay(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    outb_delay(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);

    // ICW2: Vector offset
    outb_delay(PIC1_DATA, offset_master);
    outb_delay(PIC2_DATA, offset_slave);

    // ICW3: Communication lines for cascading
    outb_delay(PIC1_COMMAND, 0x04);
    outb_delay(PIC2_COMMAND, 0x02);

    // ICW4: Use 8086 mode
    outb_delay(PIC1_COMMAND, ICW4_8086);
    outb_delay(PIC2_COMMAND, ICW4_8086);

    // Restore masks
    outb(PIC1_DATA, mask_m);
    outb(PIC2_DATA, mask_s);
}

void pic_set_mask(uint8_t IRQline) {
    uint16_t port;
    uint8_t value;

    if(IRQline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        IRQline -= 8;
    }

    value = inb(port) | (1 << IRQline);
    outb(port, value);        
}

void pic_clear_mask(uint8_t IRQline) {
    uint16_t port;
    uint8_t value;

    if(IRQline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        IRQline -= 8;
    }

    value = inb(port) & ~(1 << IRQline);
    outb(port, value);        
}

void pic_init(void)
{
    pic_remap(0x20, 0x28);
}

void pic_send_eoi(size_t irq)
{
    if(irq >= 8)
		outb(PIC2_COMMAND,PIC_EOI);
	
	outb(PIC1_COMMAND,PIC_EOI);
}
