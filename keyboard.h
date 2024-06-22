#ifndef KEYBOARD_H
#define KEYBOARD_H

typedef enum Key {
    Key_None,
    Key_W,
    Key_Return
} Key;

void keyboard_init(void);
Key keyboard_pop_key_pressed(void);

#endif // KEYBOARD_H
