#include "birb.h"

#include "tunnel.h"

#define BIRB_HEIGHT     6
#define BIRB_WIDTH      8

static char const birb_sprite[BIRB_HEIGHT][BIRB_WIDTH] = {
    "     WW ",
    "     WWB",
    "   WWWW ",
    "  WWWWW ",
    "WWWWWW  ",
    "  B B   "
};

static uint8_t const collision_mask[BIRB_HEIGHT] = {
    0b00000110,
    0b00000111,
    0b00011110,
    0b00111110,
    0b01111100,
    0b00101000   
};

static Vec2 jump_velocity = { .x = 0, .y = -16 };

#define VELOCITY_STEP 2

void birb_initialize(Birb* birb)
{
    birb->position = vec2_pixel_to_decimal((Vec2) { 5, VGA_HEIGHT/2 });
    birb->velocity = jump_velocity;
}

void birb_jump(Birb* birb)
{
    birb->velocity = jump_velocity;
}

bool birb_check_tunnel_collision(Birb* birb, TunnelPool* tunnels)
{
    Vec2 pos = vec2_decimal_to_pixel(birb->position);
    
    for (size_t i = 0; i < BIRB_HEIGHT; i++) {
        uint8_t mask = collision_mask[i];
        uint8_t bitmask = 0b10000000;

        for (size_t j = 0; j < BIRB_WIDTH; j++) {
            Vec2 pixel_pos = { pos.x + j, pos.y + i};
            if (mask & bitmask && tunnel_check_collision_at(tunnels, pixel_pos))
                return true;

            bitmask >>= 1;
        }
    }

    return false;
}

void birb_update(Birb* birb)
{
    birb->position = (Vec2) {
        birb->position.x + birb->velocity.x / VELOCITY_STEP,
        birb->position.y + birb->velocity.y / VELOCITY_STEP
    };

    birb->velocity.y++;

    // Troll physics. Increase velocity artificially to prevent birb from
    // staying still
    if (birb->velocity.y >= 0 && birb->velocity.y < VELOCITY_STEP)
        birb->velocity.y = VELOCITY_STEP;
}

void birb_render(Birb* birb, Screen* screen)
{
    Vec2 pos = vec2_decimal_to_pixel(birb->position);

    for (size_t i = 0; i < BIRB_HEIGHT; i++) {
        for (size_t j = 0; j < BIRB_WIDTH; j++) {
            char sprite_ch = birb_sprite[i][j];

            if (sprite_ch == ' ')
                continue;

            VgaColor color = vga_color_map[(size_t)sprite_ch];

            screen_draw_pixel(screen, color, pos.x + j, pos.y + i);
        }
    }
}

