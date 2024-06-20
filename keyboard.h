#ifndef KEYBOARD_H
#define KEYBOARD_H

typedef enum Key {
    Key_None,
    Key_W,
} Key;

void keyboard_init(void);
Key keyboard_pop_key_pressed(void);
Key keyboard_pop_key_released(void);

#endif // KEYBOARD_H
