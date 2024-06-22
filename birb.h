#ifndef BIRB_H
#define BIRB_H

#include <stdbool.h>

#include "screen.h"
#include "utils.h"

typedef struct Birb
{
    Vec2 position;
    Vec2 velocity;
} Birb;

typedef struct TunnelPool TunnelPool;

void birb_initialize(Birb* birb);
void birb_jump(Birb* birb);
bool
 birb_check_tunnel_collision(Birb* birb, TunnelPool* tunnels);
void birb_update(Birb* birb);
void birb_render(Birb* birb, Screen* screen);


#endif // BIRB_H