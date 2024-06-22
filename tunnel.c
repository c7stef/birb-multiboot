#include "tunnel.h"

#define TUNNEL_DISTANCE 40
#define TUNNEL_GAP      16
#define TUNNEL_PADDING  6
#define TUNNEL_RANGE    (VGA_HEIGHT - 2 * TUNNEL_PADDING - TUNNEL_GAP)

#define TUNNEL_HEIGHT   5
#define TUNNEL_WIDTH    10

#define VELOCITY_STEP   200

static char const tunnel_head[TUNNEL_HEIGHT][TUNNEL_WIDTH] = {
    "GGGGGGGGGG",
    "G GG     G",
    "G GG     G",
    "G GG     G",
    "GGGGGGGGGG",
};

static char const tunnel_body[TUNNEL_WIDTH] = " G G    G ";

void tunnel_pool_initialize(TunnelPool* pool)
{
    pool->begin = pool->end = 0;
    pool->velocity = (Vec2) { -7 * VELOCITY_STEP, 0 };
}

static size_t tunnel_pool_next_idx(size_t idx)
{
    idx++;
    if (idx == MAX_TUNNELS)
        return 0;
    return idx;
}

static size_t tunnel_pool_prev_idx(size_t idx)
{
    if (idx == 0)
        return MAX_TUNNELS - 1;
    return idx - 1;
}

static void tunnel_pool_spawn(TunnelPool* pool)
{
    size_t idx1 = pool->end;
    pool->end = tunnel_pool_next_idx(pool->end);

    size_t y1 = TUNNEL_PADDING + rand() * TUNNEL_RANGE / RAND_MAX;

    pool->tunnels[idx1] = (Tunnel) {
        .position = vec2_pixel_to_decimal((Vec2) { VGA_WIDTH, y1 }),
        .upright = false,
        .birb_passed = false,
    };

    size_t idx2 = pool->end;
    pool->end = tunnel_pool_next_idx(pool->end);

    size_t y2 = y1 + TUNNEL_GAP;

    pool->tunnels[idx2] = (Tunnel) {
        .position = vec2_pixel_to_decimal((Vec2) { VGA_WIDTH, y2 }),
        .upright = true,
        .birb_passed = false,
    };
}

bool tunnel_check_collision_at(TunnelPool* pool, Vec2 point)
{
    for (size_t idx = pool->begin; idx != pool->end; idx = tunnel_pool_next_idx(idx)) {
        Tunnel t = pool->tunnels[idx];
        Vec2 tpos = vec2_decimal_to_pixel(t.position);

        if (!t.upright)
            tpos.y -= TUNNEL_HEIGHT - 1;
        
        // Collide with tunnel head
        if (point.x >= tpos.x && point.x < tpos.x + TUNNEL_WIDTH
            && point.y >= tpos.y && point.y < tpos.y + TUNNEL_HEIGHT)
            return true;
        
        // Collide with tunnel body
        bool collide_x =
            point.x >= tpos.x+1
            && point.x < tpos.x + TUNNEL_WIDTH-1;
    
        bool collide_y = (t.upright && point.y >= tpos.y)
            || (!t.upright && point.y <= tpos.y);
        
        if (collide_x && collide_y)
            return true;
    }

    return false;
}

static void tunnel_pool_pop(TunnelPool* pool)
{
    pool->begin = tunnel_pool_next_idx(pool->begin);
}

void tunnel_pool_update(TunnelPool* pool)
{
    if (pool->velocity.x > -15 * VELOCITY_STEP)
        pool->velocity.x--;

    for (size_t i = pool->begin; i != pool->end; i = tunnel_pool_next_idx(i)) {
        int new_x = pool->tunnels[i].position.x += pool->velocity.x / VELOCITY_STEP;
        if (decimal_to_pixel(new_x) <= -TUNNEL_WIDTH)
            tunnel_pool_pop(pool);
    }

    if (pool->begin == pool->end)
        tunnel_pool_spawn(pool);
    else {
        size_t last_idx = tunnel_pool_prev_idx(pool->end);
        int x = decimal_to_pixel(pool->tunnels[last_idx].position.x);
        if (VGA_WIDTH - x > TUNNEL_DISTANCE)
            tunnel_pool_spawn(pool);
    }
}

void tunnel_pool_render(TunnelPool* pool, Screen* screen)
{
    int body_width = strlen(tunnel_body);

    for (size_t idx = pool->begin; idx != pool->end; idx = tunnel_pool_next_idx(idx)) {
        bool upright = pool->tunnels[idx].upright;
        Vec2 pos_px = vec2_decimal_to_pixel(pool->tunnels[idx].position);

        if (!upright)
            pos_px.y -= TUNNEL_HEIGHT - 1;
        
        for (int i = 0; i < TUNNEL_HEIGHT; i++)
            for (int j = 0; j < TUNNEL_WIDTH; j++)
                screen_draw_sprite_px(screen, tunnel_head[i][j],
                    pos_px.x + j,
                    pos_px.y + i);

        int body_top = upright
            ? pos_px.y + TUNNEL_HEIGHT
            : 0;

        int body_bottom = upright
            ? (VGA_HEIGHT-1)
            : pos_px.y - 1;

        for (int y = body_top; y <= body_bottom; y++)
            for (int j = 0; j < body_width; j++)
                screen_draw_sprite_px(screen, tunnel_body[j],
                    pos_px.x + j,
                    y);
    }
}

bool tunnel_pool_birb_passed(TunnelPool* pool, Birb* birb)
{
    size_t idx = pool->begin;

    if (idx == pool->end)
        return false;
    
    if (pool->tunnels[idx].birb_passed)
        return false;
    
    if (pool->tunnels[idx].position.x < birb->position.x) {
        pool->tunnels[idx].birb_passed = true;
        pool->tunnels[tunnel_pool_next_idx(idx)].birb_passed = true;

        return true;
    }

    return false;
}

