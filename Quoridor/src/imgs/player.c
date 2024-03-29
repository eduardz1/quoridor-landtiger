#include "../GLCD/GLCD.h"
#include "sprites.h"

/**
 * @brief base sprite color is red
 *
 */
static const uint16_t player_data[22 * 22] = {
    0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xd105, 0xd105,
    0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xfb7f, 0xfb7f, 0xfb7f,
    0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f,
    0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105,
    0xd105, 0xd105, 0xd105, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f,
    0xfb7f, 0xfb7f, 0xfb7f, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xfb7f,
    0xfb7f, 0xfb7f, 0xfb7f, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xfb7f,
    0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xd105, 0xd105, 0xd105,
    0xd105, 0xfb7f, 0xfb7f, 0xfb7f, 0xd105, 0xd105, 0xfb7f, 0xfb7f, 0xfb7f,
    0xd105, 0xd105, 0xd105, 0xd105, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f,
    0xd105, 0xd105, 0xd105, 0xfb7f, 0xfb7f, 0xfb7f, 0xd105, 0xd105, 0xd105,
    0xd105, 0xd105, 0xd105, 0xfb7f, 0xfb7f, 0xfb7f, 0xd105, 0xd105, 0xd105,
    0xfb7f, 0xfb7f, 0xfb7f, 0xd105, 0xd105, 0xd105, 0xfb7f, 0xfb7f, 0xd105,
    0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105,
    0xfb7f, 0xfb7f, 0xd105, 0xd105, 0xd105, 0xfb7f, 0xfb7f, 0xd105, 0xd105,
    0xd105, 0xfb7f, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105,
    0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xfb7f, 0xd105, 0xd105, 0xd105,
    0xfb7f, 0xd105, 0xd105, 0xd105, 0xfb7f, 0xfb7f, 0xd105, 0xd105, 0xd105,
    0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105,
    0xfb7f, 0xfb7f, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xfb7f,
    0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105,
    0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xfb7f, 0xd105, 0xd105, 0xd105,
    0xd105, 0xd105, 0xfb7f, 0xfb7f, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105,
    0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105,
    0xfb7f, 0xfb7f, 0xd105, 0xd105, 0xd105, 0xd105, 0xfb7f, 0xd105, 0xd105,
    0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105,
    0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xfb7f, 0xd105, 0xd105, 0xd105,
    0xd105, 0xfb7f, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105,
    0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105,
    0xfb7f, 0xd105, 0xd105, 0xd105, 0xd105, 0xfb7f, 0xfb7f, 0xd105, 0xd105,
    0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105,
    0xd105, 0xd105, 0xd105, 0xfb7f, 0xfb7f, 0xd105, 0xd105, 0xd105, 0xd105,
    0xd105, 0xfb7f, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105,
    0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xfb7f, 0xd105,
    0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xfb7f, 0xfb7f, 0xd105, 0xd105,
    0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105,
    0xd105, 0xfb7f, 0xfb7f, 0xd105, 0xd105, 0xd105, 0xfb7f, 0xd105, 0xd105,
    0xd105, 0xfb7f, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105,
    0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xfb7f, 0xd105, 0xd105, 0xd105,
    0xfb7f, 0xfb7f, 0xd105, 0xd105, 0xd105, 0xfb7f, 0xfb7f, 0xd105, 0xd105,
    0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xfb7f,
    0xfb7f, 0xd105, 0xd105, 0xd105, 0xfb7f, 0xfb7f, 0xfb7f, 0xd105, 0xd105,
    0xd105, 0xfb7f, 0xfb7f, 0xfb7f, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105,
    0xd105, 0xfb7f, 0xfb7f, 0xfb7f, 0xd105, 0xd105, 0xd105, 0xfb7f, 0xfb7f,
    0xfb7f, 0xfb7f, 0xfb7f, 0xd105, 0xd105, 0xd105, 0xd105, 0xfb7f, 0xfb7f,
    0xfb7f, 0xd105, 0xd105, 0xfb7f, 0xfb7f, 0xfb7f, 0xd105, 0xd105, 0xd105,
    0xd105, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xd105,
    0xd105, 0xd105, 0xd105, 0xd105, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xd105,
    0xd105, 0xd105, 0xd105, 0xd105, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f,
    0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105,
    0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xfb7f, 0xfb7f,
    0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f,
    0xfb7f, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105, 0xd105,
    0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f,
};

static const uint16_t player_selector_data[18 * 18] = {
    0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfee0, 0xfee0,
    0xfee0, 0xfee0, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f,
    0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfee0, 0xfee0, 0xfee0, 0xfb7f,
    0xfb7f, 0xfee0, 0xfee0, 0xfee0, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f,
    0xfb7f, 0xfb7f, 0xfb7f, 0xfee0, 0xfee0, 0xfee0, 0xfb7f, 0xfb7f, 0xfb7f,
    0xfb7f, 0xfb7f, 0xfb7f, 0xfee0, 0xfee0, 0xfee0, 0xfb7f, 0xfb7f, 0xfb7f,
    0xfb7f, 0xfb7f, 0xfee0, 0xfee0, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f,
    0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfee0, 0xfee0, 0xfb7f, 0xfb7f,
    0xfb7f, 0xfb7f, 0xfee0, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f,
    0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfee0, 0xfb7f, 0xfb7f,
    0xfb7f, 0xfee0, 0xfee0, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f,
    0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfee0, 0xfee0, 0xfb7f,
    0xfb7f, 0xfee0, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f,
    0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfee0, 0xfb7f,
    0xfee0, 0xfee0, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f,
    0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfee0, 0xfee0,
    0xfee0, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f,
    0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfee0,
    0xfee0, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f,
    0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfee0,
    0xfee0, 0xfee0, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f,
    0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfee0, 0xfee0,
    0xfb7f, 0xfee0, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f,
    0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfee0, 0xfb7f,
    0xfb7f, 0xfee0, 0xfee0, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f,
    0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfee0, 0xfee0, 0xfb7f,
    0xfb7f, 0xfb7f, 0xfee0, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f,
    0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfee0, 0xfb7f, 0xfb7f,
    0xfb7f, 0xfb7f, 0xfee0, 0xfee0, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f,
    0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfee0, 0xfee0, 0xfb7f, 0xfb7f,
    0xfb7f, 0xfb7f, 0xfb7f, 0xfee0, 0xfee0, 0xfee0, 0xfb7f, 0xfb7f, 0xfb7f,
    0xfb7f, 0xfb7f, 0xfb7f, 0xfee0, 0xfee0, 0xfee0, 0xfb7f, 0xfb7f, 0xfb7f,
    0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfee0, 0xfee0, 0xfee0, 0xfb7f,
    0xfb7f, 0xfee0, 0xfee0, 0xfee0, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f,
    0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfee0, 0xfee0,
    0xfee0, 0xfee0, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f, 0xfb7f,
};

static void draw_player_white(uint16_t x, uint16_t y)
{
    LCD_draw_image_conditional(
        x, y, x + 22, y + 22, RED_PLAYER_COLOR, WHITE_PLAYER_COLOR, player_data);
    player_selector_white_inner_color.draw(x + 2, y + 2);
}

static void draw_player_red(uint16_t x, uint16_t y)
{
    LCD_draw_image(x, y, x + 22, y + 22, player_data);
    player_selector_red_inner_color.draw(x + 2, y + 2);
}

static void draw_player_selector(uint16_t x, uint16_t y)
{
    LCD_draw_image(x, y, x + 18, y + 18, player_selector_data);
}

static void draw_player_selector_cell_color(uint16_t x, uint16_t y)
{
    LCD_draw_image_conditional(
        x, y, x + 18, y + 18, HIGHLIGHT_COLOR, CELL_COLOR, player_selector_data);
}

static void draw_player_selector_red_inner_color(uint16_t x, uint16_t y)
{
    LCD_draw_image_conditional(x,
                               y,
                               x + 18,
                               y + 18,
                               HIGHLIGHT_COLOR,
                               RED_PLAYER_COLOR_INNER,
                               player_selector_data);
}

static void draw_player_selector_white_inner_color(uint16_t x, uint16_t y)
{
    LCD_draw_image_conditional(x,
                               y,
                               x + 18,
                               y + 18,
                               HIGHLIGHT_COLOR,
                               WHITE_PLAYER_COLOR_INNER,
                               player_selector_data);
}

const struct Sprite player_red = {
    .draw = draw_player_red,
    .width = 22,
    .height = 22,
};

const struct Sprite player_white = {
    .draw = draw_player_white,
    .width = 22,
    .height = 22,
};

const struct Sprite player_selector = {
    .draw = draw_player_selector, .width = 18, .height = 18};

const struct Sprite player_selector_cell_color = {
    .draw = draw_player_selector_cell_color, .width = 18, .height = 18};

const struct Sprite player_selector_red_inner_color = {
    .draw = draw_player_selector_red_inner_color, .width = 18, .height = 18};

const struct Sprite player_selector_white_inner_color = {
    .draw = draw_player_selector_white_inner_color, .width = 18, .height = 18};
