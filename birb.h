#ifndef BIRB_H
#define BIRB_H

#include "term.h"
#include "utils.h"

typedef struct Birb
{
    Vec2 position;
    Vec2 velocity;
} Birb;

void birb_initialize(Birb* birb);
void birb_jump(Birb* birb);
void birb_update(Birb* birb);
void birb_render(Birb* birb, TermHandle* term);

#endif // BIRB_H