#ifndef TIMER_H
#define TIMER_H

#include <stdbool.h>

void timer_init(void);
bool timer_check_tick(void);
void timer_sleep_ticks(int ticks);

#endif // TIMER_H
