#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION < 6000000)
#pragma diag_suppress 68 // integer conversion resulted in a change of sign
#endif

#include "graphics.h"
#include "../GLCD/GLCD.h"
#include "../imgs/sprites.h"
#include "game.h"
#include <stdint.h>
#include <stdio.h>

void draw_main_menu(void)
{
    LCD_Clear(TABLE_COLOR);
    LCD_write_text(40, 40, "Select the", Black, TRANSPARENT, 2);
    LCD_write_text(48, 40 + 2 + 16, "GAME MODE", Black, TRANSPARENT, 2);

    empty_square_wide_wide_transparent.draw(
        (MAX_X - empty_square_wide_wide_transparent.width) / 2,
        40 + 40 + 2 + 16 + 40);
    LCD_write_text(
        72, 40 + 40 + 2 + 16 + 40 + 10, "SINGLE BOARD", Black, TRANSPARENT, 1);

    empty_square_wide_wide_transparent.draw(
        (MAX_X - empty_square_wide_wide_transparent.width) / 2,
        40 + 40 + 2 + 16 + 40 + 32 + 2);
    LCD_write_text(84,
                   40 + 40 + 2 + 16 + 40 + 32 + 2 + 10,
                   "TWO BOARD",
                   Black,
                   TRANSPARENT,
                   1);
}

void draw_single_board_menu(void)
{
    LCD_draw_full_width_rectangle(40, 40 + 2 + 16 + 16, TABLE_COLOR);

    LCD_write_text(40, 40, "Single Board: select", Black, TRANSPARENT, 1);
    LCD_write_text(
        36, 40 + 2 + 8, "the opposite opponent", Black, TRANSPARENT, 1);

    LCD_write_text(
        72, 40 + 40 + 2 + 16 + 40 + 10, "    HUMAN   ", Black, TABLE_COLOR, 1);

    LCD_write_text(72,
                   40 + 40 + 2 + 16 + 40 + 32 + 2 + 10,
                   "     NPC    ",
                   Black,
                   TABLE_COLOR,
                   1);
}

void draw_two_board_menu(void)
{
    LCD_draw_full_width_rectangle(40, 40 + 2 + 16 + 16, TABLE_COLOR);

    LCD_write_text(48, 40, "Two Boards: select", Black, TRANSPARENT, 1);
    LCD_write_text(76, 40 + 2 + 8, "your player", Black, TRANSPARENT, 1);

    LCD_write_text(
        72, 40 + 40 + 2 + 16 + 40 + 10, "    HUMAN   ", Black, TABLE_COLOR, 1);

    LCD_write_text(72,
                   40 + 40 + 2 + 16 + 40 + 32 + 2 + 10,
                   "     NPC    ",
                   Black,
                   TABLE_COLOR,
                   1);
}

void draw_waiting_for_connection(void)
{
    LCD_draw_full_width_rectangle(40, 40 + 2 + 16 + 16, TABLE_COLOR);
    LCD_write_text(16, 40, "Waiting for connection...", Black, TRANSPARENT, 1);
}

void draw_color_selection_menu(void)
{
    LCD_draw_full_width_rectangle(40, 40 + 2 + 16 + 16, TABLE_COLOR);

    LCD_write_text(52, 40, "Choose your color", Black, TRANSPARENT, 1);

    LCD_draw_rectangle(72,
                       40 + 40 + 2 + 16 + 40 + 10,
                       168,
                       40 + 40 + 2 + 16 + 40 + 10 + 8,
                       TABLE_COLOR);
    LCD_draw_rectangle(72,
                       40 + 40 + 2 + 16 + 40 + 10 + 32 + 2,
                       168,
                       40 + 40 + 2 + 16 + 40 + 10 + 8 + 32 + 2,
                       TABLE_COLOR);

    player_red.draw(109, 40 + 40 + 2 + 16 + 40 + 5);
    player_white.draw(109, 40 + 40 + 2 + 16 + 40 + 32 + 2 + 5);
}

void draw_board(void)
{

    uint16_t start_x = 0, start_y = TOP_PADDING - BLOCK_PADDING, x, y;

    /* Draw background */
    LCD_draw_full_width_rectangle(0, start_y, TABLE_COLOR);
    LCD_DrawLine(start_x, start_y, MAX_X, start_y, Black);
    LCD_draw_full_width_rectangle(start_y + 1, MAX_Y - start_y, BOARD_COLOR);
    LCD_DrawLine(start_x, MAX_Y - start_y, MAX_X, MAX_Y - start_y, Black);
    LCD_draw_full_width_rectangle(MAX_Y - start_y + 1, MAX_Y, TABLE_COLOR);

    start_x = BLOCK_PADDING;
    start_y = BLOCK_PADDING + TOP_PADDING;

    /* Draws the empty squares */
    for (x = 0; x < BOARD_SIZE; x++)
    {
        for (y = 0; y < BOARD_SIZE; y++)
        {
            empty_square.draw(start_x, start_y);

            board.board[x][y].player_id = NONE;
            board.board[x][y].walls.as_uint8_t = 0;
            board.board[x][y].x = start_x;
            board.board[x][y].y = start_y;

            start_y += empty_square.height + BLOCK_PADDING;
        }
        start_y = BLOCK_PADDING + TOP_PADDING;
        start_x += empty_square.width + BLOCK_PADDING;
    }

    start_x = board.board[BOARD_SIZE >> 1][0].x + PLAYER_PADDING;
    start_y = board.board[BOARD_SIZE >> 1][0].y + PLAYER_PADDING;

    player_red.draw(start_x, start_y);
    player_white.draw(start_x, MAX_Y - start_y - player_red.height);

    board.board[BOARD_SIZE >> 1][0].player_id = RED;
    board.board[BOARD_SIZE >> 1][BOARD_SIZE - 1].player_id = WHITE;

    refresh_info_panel(0);
}

void refresh_info_panel(const uint8_t timer)
{
    static bool drawn = false;
    static enum Player last_player = WHITE;
    char time_str[3];
    char wall_str[3];

    if (drawn == false)
    {
        empty_square_wide_transparent.draw(174, 3);
        highlighted_square_wide_red.draw(174 + 2, 3 + 2);
        drawn = true;
    }

    if (last_player != current_player)
    {
        last_player = current_player;
        (last_player == RED ?
             highlighted_square_wide_red.draw :
             highlighted_square_wide_white.draw)(174 + 2, 3 + 2);

        LCD_write_text(178, 9, "time   ", Black, TRANSPARENT, 1);
        LCD_write_text(178, 9 + 8 + 4, "wall   ", Black, TRANSPARENT, 1);
    }

    snprintf(time_str, sizeof(time_str), "%2d", timer);
    snprintf(wall_str,
             sizeof(wall_str),
             "%2d",
             last_player == RED ? red.wall_count : white.wall_count);
    LCD_write_text(178 + 8 * 5, 9, time_str, Black, TABLE_COLOR, 1);
    LCD_write_text(178 + 8 * 5, 9 + 8 + 4, wall_str, Black, TABLE_COLOR, 1);
}

void refresh_walls(void)
{ // TODO: fix hard coded number with const
    uint16_t y, x;
    const uint16_t top = board.board[0][0].y - BLOCK_PADDING;
    const uint16_t bot = board.board[0][BOARD_SIZE - 1].y + 32 + BLOCK_PADDING;
    const uint16_t left = board.board[0][0].x - BLOCK_PADDING;
    const uint16_t right =
        board.board[BOARD_SIZE - 1][0].x + 32 + BLOCK_PADDING;

    for (y = top; y < bot; y += 32 + BLOCK_PADDING)
        LCD_draw_full_width_rectangle(y, y + 2, BOARD_COLOR);

    for (x = left; x < right; x += 32 + BLOCK_PADDING)
        LCD_draw_rectangle(x, top, x + 2, bot, BOARD_COLOR);

    for (x = 0; x < BOARD_SIZE; x++)
    {
        for (y = 0; y < BOARD_SIZE; y++)
        {
            if (board.board[x][y].walls.right == true)
                wall_vertical_half.draw(board.board[x][y].x + 32,
                                        board.board[x][y].y - 1);
            if (board.board[x][y].walls.left == true)
                wall_vertical_half.draw(board.board[x][y].x - 2,
                                        board.board[x][y].y - 1);
            if (board.board[x][y].walls.top == true)
                wall_horizontal_half.draw(board.board[x][y].x - 1,
                                          board.board[x][y].y - 2);
            if (board.board[x][y].walls.bottom == true)
                wall_horizontal_half.draw(board.board[x][y].x - 1,
                                          board.board[x][y].y + 32);
        }
    }
}

void highlight_possible_moves(void)
{
    uint8_t i;
    uint16_t start_x, start_y;
    const uint8_t HIGHLIGHT_PADDING =
        (uint8_t)(empty_square.width - highlighted_square.width) >> 1;

    for (i = 0; i < 5; i++)
    {
        if (legal_moves[i].x == UINT8_MAX) continue;

        start_x = board.board[legal_moves[i].x][legal_moves[i].y].x +
                  HIGHLIGHT_PADDING;
        start_y = board.board[legal_moves[i].x][legal_moves[i].y].y +
                  HIGHLIGHT_PADDING;

        highlighted_square.draw(start_x, start_y);
    }
}

void clear_highlighted_moves(void)
{
    uint8_t i;
    uint16_t x, y, start_x, start_y;
    const uint8_t HIGHLIGHT_PADDING =
        (uint8_t)(empty_square.width - highlighted_square.width) >> 1;

    for (i = 0; i < 5; i++)
    {
        if (legal_moves[i].x == UINT8_MAX) break;

        x = legal_moves[i].x;
        y = legal_moves[i].y;

        start_x = board.board[x][y].x + HIGHLIGHT_PADDING;
        start_y = board.board[x][y].y + HIGHLIGHT_PADDING;

        highlighted_square_cell_color.draw(start_x, start_y);
    }
}

/**
 * @pre the move is valid
 *
 */
void update_player_sprite(const uint8_t new_x, const uint8_t new_y)
{
    const struct Sprite *player;
    uint16_t old_x, old_y;

    if (current_player == RED)
    {
        old_x = board.board[red.x][red.y].x;
        old_y = board.board[red.x][red.y].y;
        player = &player_red;
    }
    else
    {
        old_x = board.board[white.x][white.y].x;
        old_y = board.board[white.x][white.y].y;
        player = &player_white;
    }

    empty_square.draw(old_x, old_y);
    player->draw(board.board[new_x][new_y].x + PLAYER_PADDING,
                 board.board[new_x][new_y].y + PLAYER_PADDING);
}

struct Coordinate
update_player_selector(const int8_t up, const int8_t right, bool show)
{
    static int16_t x = 0, y = 0;
    static bool flag_change_turn = true;
    static uint32_t last_turn = 0;

    uint16_t start_x, start_y;
    const struct Sprite *color;

    if (last_turn != turn_id) flag_change_turn = true;

    if (flag_change_turn)
    {
        last_turn = turn_id;

        x = current_player == RED ? red.x : white.x;
        y = current_player == RED ? red.y : white.y;

        flag_change_turn = false;
    }

    start_x = board.board[x][y].x + PLAYER_SELECTOR_PADDING;
    start_y = board.board[x][y].y + PLAYER_SELECTOR_PADDING;

    switch (board.board[x][y].player_id)
    {
    case NONE: color = &player_selector_cell_color; break;
    case RED: color = &player_selector_red_inner_color; break;
    case WHITE: color = &player_selector_white_inner_color; break;
    }

    color->draw(start_x, start_y);

    if (!show)
        return (struct Coordinate){(uint8_t)x, (uint8_t)y}; // clear selector
                                                            // only

    x += right;
    y += up;

    CLAMP(x, 0, BOARD_SIZE - 1)
    CLAMP(y, 0, BOARD_SIZE - 1)

    start_x = board.board[x][y].x + PLAYER_SELECTOR_PADDING;
    start_y = board.board[x][y].y + PLAYER_SELECTOR_PADDING;

    player_selector.draw(start_x, start_y);

    return (struct Coordinate){(uint8_t)x, (uint8_t)y};
}

struct Coordinate
update_wall_selector(const int8_t up, const int8_t right, bool show)
{
    static int16_t x = 0, y = 0;
    static bool flag_change_turn = true;
    static uint32_t last_turn = 0;

    uint16_t start_x, start_y;

    if (last_turn != turn_id) flag_change_turn = true;

    if (flag_change_turn)
    {
        last_turn = turn_id;

        /* place wall in the middle of the board */
        x = BOARD_SIZE >> 1;
        y = BOARD_SIZE >> 1;

        direction = VERTICAL;

        flag_change_turn = false;
    }

    refresh_walls(); // TODO: can cache the last position like the player
                     // selector but it's more complicated, on board it's speedy
                     // enough, low priority

    if (!show)
        return (struct Coordinate){(uint8_t)x, (uint8_t)y}; // clear selector
                                                            // only

    x += right;
    y += up;

    CLAMP(x, 0, BOARD_SIZE - 2) // wall is 2 cells wide
    CLAMP(y, 0, BOARD_SIZE - 2)

    start_x = board.board[x][y].x;
    start_y = board.board[x][y].y;

    if (direction == HORIZONTAL)
    {
        start_y += empty_square.width;
        wall_horizontal_selector.draw(start_x, start_y);
    }
    else
    {
        start_x += empty_square.width;
        wall_vertical_selector.draw(start_x, start_y);
    }

    return (struct Coordinate){(uint8_t)x, (uint8_t)y};
}

struct Coordinate
update_menu_selector(const int8_t up, const int8_t right, bool show)
{
    static int8_t last_y = 0;
    (void)right;

    highlighted_square_wide_wide_table_color.draw(
        (MAX_X - highlighted_square_wide_wide.width) / 2,
        last_y == 0 ? 40 + 40 + 2 + 16 + 40 + 2 :
                      40 + 40 + 2 + 16 + 40 + 32 + 2 + 2);

    if (!show) return (struct Coordinate){0, (uint8_t)last_y};

    last_y += up;

    CLAMP(last_y, 0, 1)

    highlighted_square_wide_wide.draw(
        (MAX_X - highlighted_square_wide_wide.width) / 2,
        last_y == 0 ? 40 + 40 + 2 + 16 + 40 + 2 :
                      40 + 40 + 2 + 16 + 40 + 32 + 2 + 2);

    return (struct Coordinate){0, (uint8_t)last_y};
}
