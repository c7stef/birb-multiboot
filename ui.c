#include <stdbool.h>

#include "ui.h"
#include "utils.h"

#define DIGIT_GAP       1
#define PANEL_PADDING   1
#define PANEL_BORDER    1
#define PANEL_MARGIN    1
#define NUM_DIGITS      3

#define DIGIT_WIDTH     3
#define DIGIT_HEIGHT    5

static char const digit_sprites[10][DIGIT_HEIGHT][DIGIT_WIDTH] = {
    {
        "WWW",
        "W W",
        "W W",
        "W W",
        "WWW",
    },
    {
        "WW ",
        " W ",
        " W ",
        " W ",
        "WWW",
    },
    {
        "WWW",
        "  W",
        "WWW",
        "W  ",
        "WWW",
    },
    {
        "WWW",
        "  W",
        " WW",
        "  W",
        "WWW",
    },
    {
        "W W",
        "W W",
        "WWW",
        "  W",
        "  W",
    },
    {
        "WWW",
        "W  ",
        "WWW",
        "  W",
        "WWW",
    },
    {
        "WWW",
        "W  ",
        "WWW",
        "W W",
        "WWW",
    },
    {
        "WWW",
        "  W",
        "  W",
        "  W",
        "  W",
    },
    {
        "WWW",
        "W W",
        "WWW",
        "W W",
        "WWW",
    },
    {
        "WWW",
        "W W",
        "WWW",
        "  W",
        "WWW",
    },
};

static void panel_render(Screen* screen, Vec2 position, Vec2 size, VgaColor border_color)
{
    for (int i = 0; i < size.y; i++) {
        for (int j = 0; j < size.x; j++) {
            bool is_margin = i == 0 || j == 0 || i == size.y-1 || j == size.x-1;
            VgaColor color = is_margin ? border_color : VgaColor_Black;
            screen_draw_pixel(screen, color, position.x + j, position.y + i);
        }
    }
}

void score_render(size_t score, Screen* screen)
{
    Vec2 panel_size = {
        .x = NUM_DIGITS * DIGIT_WIDTH
            + (NUM_DIGITS - 1) * DIGIT_GAP
            + 2 * PANEL_PADDING + 2 * PANEL_BORDER,
        .y = DIGIT_HEIGHT + 2 * PANEL_PADDING + 2 * PANEL_BORDER
    };
    
    Vec2 panel_pos = {
        .x = VGA_WIDTH - PANEL_MARGIN - panel_size.x,
        .y = 0 + PANEL_MARGIN
    };

    panel_render(screen, panel_pos, panel_size, VgaColor_Green);

    Vec2 digit_pos = {
        panel_pos.x + PANEL_BORDER + PANEL_PADDING + 2 * (DIGIT_WIDTH + DIGIT_GAP),
        panel_pos.y + PANEL_BORDER + PANEL_PADDING
    };

    do {
        for (size_t i = 0; i < DIGIT_HEIGHT; i++)
            for (size_t j = 0; j < DIGIT_WIDTH; j++)
                screen_draw_sprite_px(screen, digit_sprites[score % 10][i][j],
                    digit_pos.x + j, digit_pos.y + i);
        score /= 10;
        digit_pos.x -= DIGIT_WIDTH + DIGIT_GAP;
    } while (score != 0);
}

void restart_render(Screen* screen)
{
    static char const message_down[] = "Birb down!";
    static char const message_restart[] = "Press Return to play again.";
    size_t len_restart = sizeof message_restart - 1;

    Vec2 panel_size = {
        .x = len_restart + 2 * PANEL_PADDING + 2 * PANEL_BORDER,
        .y = 3 /* Text lines */ + 2 * PANEL_PADDING + 2 * PANEL_BORDER
    };

    Vec2 panel_position = {
        .x = (VGA_WIDTH - panel_size.x) / 2,
        .y = (VGA_HEIGHT - panel_size.y) / 2
    };

    panel_render(screen, panel_position, panel_size, VgaColor_Red);

    screen_write_string(screen, message_down,
        panel_position.x + 2, panel_position.y + 2);

    screen_write_string(screen, message_restart,
        panel_position.x + 2, panel_position.y + 4);
}
