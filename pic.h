#ifndef PIC_H
#define PIC_H

#include <stddef.h>

#define PIC_VECTOR_BASE 0x20

void pic_init(void);
void pic_send_eoi(size_t irq);

#endif // PIC_H