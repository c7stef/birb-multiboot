#include <stdatomic.h>

#include "timer.h"

#include "idt.h"
#include "utils.h"

#define PIT_IRQ         0
#define PIT_PORT        0x40

// Chosen by fair dice roll
#define PIT_RELOAD      0x100
#define TICK_SIGNALS    90

atomic_int counter = 0;

static void pit_irq_handler(void)
{
    counter++;
}

void timer_init(void)
{
    disable_interrupts();
    outb(PIT_PORT, PIT_RELOAD & 0xFF);
    outb(PIT_PORT, (PIT_RELOAD >> 8) & 0xFF);

    counter = 0;
    register_irq_handler(PIT_IRQ, pit_irq_handler);

    enable_interrupts();
}

bool timer_check_tick(void)
{
    if (counter > TICK_SIGNALS) {
        counter = 0;
        return true;
    }

    return false;
}

void timer_sleep_ticks(int ticks)
{
    while (ticks)
        ticks -= timer_check_tick();
}

