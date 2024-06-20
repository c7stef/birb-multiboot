#include "keyboard.h"
#include "idt.h"
#include "utils.h"

#define KBD_IRQ 1
#define KBD_PORT 0x60

// One-element queue.
static Key current_key_pressed = Key_None;
static Key current_key_released = Key_None;

static void keyboard_irq_handler(void)
{
    uint8_t key = inb(KBD_PORT);

    if (key == 0x11)
        current_key_pressed = Key_W;
    if (key == 0x91)
        current_key_released = Key_W;
}

Key keyboard_pop_key_pressed(void)
{
    Key key = current_key_pressed;
    current_key_pressed = Key_None;
    return key;
}

Key keyboard_pop_key_released(void)
{
    Key key = current_key_released;
    current_key_released = Key_None;
    return key;
}

void keyboard_init(void)
{
    register_irq_handler(KBD_IRQ, keyboard_irq_handler);
}
