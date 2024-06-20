#include "birb.h"

static char const* const birb_sprite[] = {
    "     WW ",
    "     WWB",
    "   DGGW ",
    "  DGGGW ",
    "DDWWWW  ",
    "  B B   "
};

void birb_initialize(Birb* birb) {
    birb->position = birb->velocity = (Vec2) { 0, 0 };
}

void birb_update(Birb* birb) {
    birb->position = (Vec2) {
        birb->position.x + birb->velocity.x,
        birb->position.y + birb->velocity.y
    };
}

void birb_render(Birb* birb, TermHandle* term) {
    size_t sprite_height = sizeof birb_sprite / sizeof *birb_sprite;
    size_t sprite_width = strlen(*birb_sprite);

    for (size_t i = 0; i < sprite_height; i++) {
        for (size_t j = 0; j < sprite_width; j++) {
            char sprite_ch = birb_sprite[i][j];

            if (sprite_ch == ' ')
                continue;

            VgaColor color = vga_color_map[(size_t)sprite_ch];

            terminal_draw_pixel(term, color, birb->position.x + j, birb->position.y + i);
        }
    }
}

