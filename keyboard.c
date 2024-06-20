#include "keyboard.h"
#include "idt.h"

#define KBD_IRQ 1

static void keyboard_irq_handler(void)
{
    
}

void keyboard_init(void)
{
    register_irq_handler(KBD_IRQ, keyboard_irq_handler);
}
