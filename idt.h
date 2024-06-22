#ifndef IDT_H
#define IDT_H

#include <stddef.h>

void idt_init(void);
void disable_interrupts(void);
void enable_interrupts(void);
void register_irq_handler(size_t irq, void callback(void));

#endif // IDT_H
