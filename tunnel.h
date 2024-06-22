#ifndef TUNNEL_H
#define TUNNEL_H

#include <stdbool.h>

#include "screen.h"
#include "birb.h"
#include "utils.h"

#define MAX_TUNNELS 16

typedef struct Tunnel {
    Vec2 position;
    bool upright;
    bool birb_passed;
} Tunnel;

typedef struct TunnelPool {
    Tunnel tunnels[MAX_TUNNELS];
    size_t begin;
    size_t end;
    Vec2 velocity;
} TunnelPool;

void tunnel_pool_initialize(TunnelPool* pool);
bool tunnel_check_collision_at(TunnelPool* pool, Vec2 point);
void tunnel_pool_update(TunnelPool* pool);
void tunnel_pool_render(TunnelPool* pool, Screen* screen);
bool tunnel_pool_birb_passed(TunnelPool* pool, Birb* birb);

#endif // TUNNEL_H
