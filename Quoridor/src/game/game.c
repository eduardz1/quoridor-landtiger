#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION < 6000000)
#pragma diag_suppress 68 // integer conversion resulted in a change of sign
#endif
#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6000000)
// enumeration values 'PLAYER_MOVE' and 'WALL_PLACEMENT' not explicitly handled
// in switch
#pragma clang diagnostic ignored "-Wswitch-enum"
// implicit conversion changes signedness: 'int' to 'uint32_t' (aka 'unsigned
// int')
#pragma clang diagnostic ignored "-Wsign-conversion"
// mixing declarations and code is incompatible with standards before C99
#pragma clang diagnostic ignored "-Wdeclaration-after-statement"
#endif

#include "game.h"
#include "../CAN/CAN.h"
#include "../GLCD/GLCD.h"
#include "../RIT/RIT.h"
#include "../button_EXINT/button.h"
#include "../imgs/sprites.h"
#include "../utils/headers/dyn_array.h"
#include "../utils/headers/stack.h"
#include "graphics.h"
#include "npc.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

bool AI_enabled = false;
bool CAN_enabled = false;

bool connected = true;

enum Player opponent = WHITE;
enum Player NPC = WHITE; // TODO: make order of all this global when you can
                         // test on the board again

struct Coordinate legal_moves[MAX_NEIGHBORS] = {UINT8_MAX};
enum Player current_player = WHITE;
struct Board board = {0};
enum Mode mode = GAME_MODE_SELECTION;
enum Direction direction = VERTICAL;

struct PlayerInfo red = {RED, 3, 0, 8};
struct PlayerInfo white = {WHITE, 3, 6, 8};

uint32_t turn_id = 0; // number that is incremented each turn

void game_init(void)
{
    board.moves = new_dyn_array(0);

    draw_main_menu();
    enable_RIT(); /* start accepting inputs */
}

void select_menu_option(bool up_or_down)
{
    switch (mode)
    {
    case GAME_MODE_SELECTION:
        if (!up_or_down)
        {
            mode = SINGLE_BOARD_MENU;
            draw_single_board_menu();
        }
        else
        {
            mode = TWO_BOARDS_MENU;
            CAN_enabled = true;
            draw_two_board_menu();
        }
        break;

    case SINGLE_BOARD_MENU:
        if (up_or_down) // vs AI
        {
            mode = COLOR_SELECTION_MENU;
            AI_enabled = true;
            draw_color_selection_menu();
        }
        else
        {
            draw_board();
            srand(LPC_RIT->RICOUNTER);
            change_turn();
        }
        break;

    case TWO_BOARDS_MENU:
        if (up_or_down) AI_enabled = true;

        CAN_RxMsg.data[0] = 0xFF;
        CAN_RxMsg.data[1] = 0xFF;
        CAN_RxMsg.data[2] = 0xFF;
        CAN_RxMsg.data[3] = 0xFF;
        CAN_RxMsg.len = 4;
        CAN_RxMsg.id = 1;
        CAN_RxMsg.format = STANDARD_FORMAT;
        CAN_RxMsg.type = DATA_FRAME;
        CAN_wrMsg(1, &CAN_RxMsg);

        draw_waiting_for_connection();
        connected = false;

        mode = COLOR_SELECTION_MENU;
        break;

    case COLOR_SELECTION_MENU:

        opponent = up_or_down ? RED : WHITE;
        NPC = opponent;

        if (CAN_enabled)
        {
            NPC = opponent == RED ? WHITE : RED;
            if (opponent == RED)
            {
                CAN_RxMsg.data[0] = 0xFA;
                CAN_RxMsg.data[1] = 0xFA;
                CAN_RxMsg.data[2] = 0xFA;
                CAN_RxMsg.data[3] = 0xFA;
            }
            else
            {
                CAN_RxMsg.data[0] = 0xFC;
                CAN_RxMsg.data[1] = 0xFC;
                CAN_RxMsg.data[2] = 0xFC;
                CAN_RxMsg.data[3] = 0xFC;
            }

            CAN_RxMsg.len = 4;
            CAN_RxMsg.id = 1;
            CAN_RxMsg.format = STANDARD_FORMAT;
            CAN_RxMsg.type = DATA_FRAME;
            CAN_wrMsg(1, &CAN_RxMsg);
        }
        draw_board();
        srand(LPC_RIT->RICOUNTER);
        change_turn();
        break;

    default: break;
    }
}

void change_turn(void)
{
    turn_id++;
    current_player = current_player == RED ? WHITE : RED;
    mode = PLAYER_MOVE; // reset to default
    clear_highlighted_moves();
    LCD_draw_rectangle(2, 9, 2 + 8 * 21, 9 + 8 + 4 + 8, TABLE_COLOR);
    calculate_possible_moves(legal_moves, current_player);
    highlight_possible_moves();
    reset_timer();

    if (current_player == opponent)
    {
        if (AI_enabled && !CAN_enabled)
        {
            AI_move();
            change_turn();
        }
        else if (!AI_enabled && CAN_enabled)
        {
            // wait for CAN message
            return;
        }
    }
    else if (CAN_enabled && AI_enabled)
    {
        AI_move();
        change_turn();
    }
}

void calculate_possible_moves(struct Coordinate moves[MAX_NEIGHBORS],
                              const enum Player player)
{
    uint8_t x = player == RED ? red.x : white.x,
            y = player == RED ? red.y : white.y, i = 0;
    struct Coordinate possible_moves[MAX_NEIGHBORS] = {0};

    // check left
    if (x > 0 && !board.board[x][y].walls.left)
    {
        if (board.board[x - 1][y].player_id != NONE)
        {
            if (x - 1 > 0 && !board.board[x - 1][y].walls.left)
            { // jump over left
                possible_moves[i].x = x - 2;
                possible_moves[i++].y = y;
            }
            else
            { // TODO: check if you can move diagonally even though you can jump
                if (y > 0 && !board.board[x - 1][y].walls.top)
                { // up and left
                    possible_moves[i].x = x - 1;
                    possible_moves[i++].y = y - 1;
                }
                if (y < BOARD_SIZE - 1 && !board.board[x - 1][y].walls.bottom)
                { // down and left
                    possible_moves[i].x = x - 1;
                    possible_moves[i++].y = y + 1;
                }
            }
        }
        else
        { // left
            possible_moves[i].x = x - 1;
            possible_moves[i++].y = y;
        }
    }

    // check right
    if (x < BOARD_SIZE - 1 && !board.board[x][y].walls.right)
    {
        if (board.board[x + 1][y].player_id != NONE)
        {
            if (x + 1 < BOARD_SIZE - 1 && !board.board[x + 1][y].walls.right)
            { // jump over right
                possible_moves[i].x = x + 2;
                possible_moves[i++].y = y;
            }
            else
            {
                if (y > 0 && !board.board[x + 1][y].walls.top)
                { // up and right
                    possible_moves[i].x = x + 1;
                    possible_moves[i++].y = y - 1;
                }
                if (y < BOARD_SIZE - 1 && !board.board[x + 1][y].walls.bottom)
                { // down and right
                    possible_moves[i].x = x + 1;
                    possible_moves[i++].y = y + 1;
                }
            }
        }
        else
        { // right
            possible_moves[i].x = x + 1;
            possible_moves[i++].y = y;
        }
    }

    // check top
    if (y > 0 && !board.board[x][y].walls.top)
    {
        if (board.board[x][y - 1].player_id != NONE)
        {
            if (y - 1 > 0 && !board.board[x][y - 1].walls.top)
            { // jump over top
                possible_moves[i].x = x;
                possible_moves[i++].y = y - 2;
            }
            else
            {
                if (x > 0 && !board.board[x][y - 1].walls.left)
                { // up and left
                    possible_moves[i].x = x - 1;
                    possible_moves[i++].y = y - 1;
                }
                if (x < BOARD_SIZE - 1 && !board.board[x][y - 1].walls.right)
                { // up and right
                    possible_moves[i].x = x + 1;
                    possible_moves[i++].y = y - 1;
                }
            }
        }
        else
        { // top
            possible_moves[i].x = x;
            possible_moves[i++].y = y - 1;
        }
    }

    // check bottom
    if (y < BOARD_SIZE - 1 && !board.board[x][y].walls.bottom)
    {
        if (board.board[x][y + 1].player_id != NONE)
        {
            if (y + 1 < BOARD_SIZE - 1 && !board.board[x][y + 1].walls.bottom)
            { // jump over bottom
                possible_moves[i].x = x;
                possible_moves[i++].y = y + 2;
            }
            else
            {
                if (x > 0 && !board.board[x][y + 1].walls.left)
                { // down and left
                    possible_moves[i].x = x - 1;
                    possible_moves[i++].y = y + 1;
                }
                if (x < BOARD_SIZE - 1 && !board.board[x][y + 1].walls.right)
                { // down and right
                    possible_moves[i].x = x + 1;
                    possible_moves[i++].y = y + 1;
                }
            }
        }
        else
        { // bottom
            possible_moves[i].x = x;
            possible_moves[i++].y = y + 1;
        }
    }

    for (; i < ARRAY_SIZE(possible_moves); i++)
    {
        possible_moves[i].x = UINT8_MAX;
        possible_moves[i].y = UINT8_MAX;
    }

    memcpy(moves, possible_moves, sizeof(possible_moves));
}

union Move move_player(const uint8_t x, const uint8_t y)
{
    union Move move = {0};
    move.x = x;
    move.y = y;
    move.player_id = current_player;

    for (uint8_t i = 0; i < ARRAY_SIZE(legal_moves); i++)
    {
        if (legal_moves[i].x != move.x || legal_moves[i].y != move.y) continue;

        board
            .board[current_player == RED ? red.x : white.x]
                  [current_player == RED ? red.y : white.y]
            .player_id = NONE;
        board.board[move.x][move.y].player_id = current_player;

        clear_highlighted_moves();
        update_player_sprite(move.x, move.y);

        if (current_player == RED)
        {
            red.x = move.x;
            red.y = move.y;
        }
        else
        {
            white.x = move.x;
            white.y = move.y;
        }

        dyn_array_push(board.moves, move.as_uint32_t);
        if (CAN_enabled && current_player != opponent)
        {
            CAN_TxMsg.data[0] = move.bytes[0];
            CAN_TxMsg.data[1] = move.bytes[1];
            CAN_TxMsg.data[2] = move.bytes[2];
            CAN_TxMsg.data[3] = move.bytes[3];
            CAN_TxMsg.len = 4;
            CAN_TxMsg.id = 1;
            CAN_TxMsg.format = STANDARD_FORMAT;
            CAN_TxMsg.type = DATA_FRAME;
            CAN_wrMsg(1, &CAN_TxMsg);
        }

        if (red.y == BOARD_SIZE - 1)
        {
            LCD_write_text(48, MAX_Y - 27, "RED wins!", Black, TRANSPARENT, 2);
            exit(0);
        }
        else if (white.y == 0)
        {
            LCD_write_text(
                32, MAX_Y - 27, "WHITE wins!", Black, TRANSPARENT, 2);
            exit(0);
        }

        return move;
    }

    move.as_uint32_t = -1;
    return move; // DO NOT MERGE THE PRECEDING LINE WITH THIS ONE
}

bool is_wall_between(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
    return (x1 == x2 && y1 < y2 && board.board[x1][y1].walls.bottom) ||
           (x1 == x2 && y1 > y2 && board.board[x1][y1].walls.top) ||
           (y1 == y2 && x1 < x2 && board.board[x1][y1].walls.right) ||
           (y1 == y2 && x1 > x2 && board.board[x1][y1].walls.left);
}

bool find_path(uint8_t x, uint8_t y, const uint8_t winning_y)
{
    struct Stack stack;
    bool visited[BOARD_SIZE][BOARD_SIZE] = {false};

    stack_init(&stack, BOARD_SIZE * BOARD_SIZE);
    push(&stack, x, y); // push starting position
    visited[x][y] = true;

    while (!is_empty(&stack))
    {
        struct Coordinate coordinate = pop(&stack);
        x = coordinate.x;
        y = coordinate.y;

        if (y == winning_y) return true;

        // neighbors to visit
        uint8_t neighbors[][2] = {
            {    x, y - 1},
            {    x, y + 1},
            {x + 1,     y},
            {x - 1,     y}
        };

        for (uint8_t i = 0; i < ARRAY_SIZE(neighbors); i++)
        {
            uint8_t new_x = neighbors[i][0];
            uint8_t new_y = neighbors[i][1];

            if (new_x < BOARD_SIZE && new_y < BOARD_SIZE &&
                !visited[new_x][new_y] && !is_wall_between(x, y, new_x, new_y))
            {
                push(&stack, new_x, new_y);
                visited[new_x][new_y] = true;
            }
        }
    }

    return false;
}

bool is_not_trapped(const enum Player player)
{
    uint8_t x = player == RED ? red.x : white.x;
    uint8_t y = player == RED ? red.y : white.y;

    return find_path(x, y, player == RED ? BOARD_SIZE - 1 : 0);
}

bool is_wall_clipping(const uint8_t x,
                      const uint8_t y,
                      const enum Direction dir)
{
    switch (dir)
    {
    case HORIZONTAL:
        // check overlap
        if (board.board[x][y].walls.bottom ||
            board.board[x + 1][y].walls.bottom)
            return true;

        // check intersection
        if (y < BOARD_SIZE - 1 && board.board[x][y + 1].walls.right &&
            board.board[x][y].walls.right)
            return true;
        break;

    case VERTICAL:
        // check overlap
        if (board.board[x][y].walls.right || board.board[x][y + 1].walls.right)
            return true;

        // check intersection
        if (x < BOARD_SIZE - 1 && board.board[x][y].walls.bottom &&
            board.board[x + 1][y].walls.bottom)
            return true;
        break;
    }

    return false;
}

bool is_wall_valid(const uint8_t x, const uint8_t y, const enum Direction dir)
{
    if (x > BOARD_SIZE - 1 || y > BOARD_SIZE - 1) return false;

    return is_not_trapped(RED) && is_not_trapped(WHITE);
}

void write_invalid_move(void)
{
    LCD_write_text(24, MAX_Y - 27, "Invalid Move", Black, TRANSPARENT, 2);
#ifndef SIMULATOR
    delay_ms(1500); // FIXME: busy waiting
#endif
    LCD_draw_rectangle(24, MAX_Y - 27, 12 * 16 + 24, MAX_Y - 11, TABLE_COLOR);
}

void backup_walls(const uint8_t x,
                  const uint8_t y,
                  uint8_t *backup,
                  const struct Coordinate *neighbors,
                  size_t size_of_neighbors)
{
    for (uint8_t i = 0; i < size_of_neighbors; i++)
    {
        backup[i] =
            board.board[x + neighbors[i].x][y + neighbors[i].y].walls.as_uint8_t;
    }
}

void rollback_walls(uint8_t x,
                    uint8_t y,
                    uint8_t *backup,
                    const struct Coordinate *neighbors,
                    size_t size_of_neighbors)
{
    for (uint8_t i = 0; i < size_of_neighbors; i++)
    {
        board.board[x + neighbors[i].x][y + neighbors[i].y].walls.as_uint8_t =
            backup[i];
    }
}

union Move
place_wall(const uint8_t x, const uint8_t y, const enum Direction dir)
{
    struct PlayerInfo *player = current_player == RED ? &red : &white;
    const struct Sprite *wall_sprite = dir == HORIZONTAL ? &wall_horizontal :
                                                           &wall_vertical;
    union Move move = {.x = x,
                       .y = y,
                       .direction = dir,
                       .type = WALL_PLACEMENT,
                       .player_id = current_player};

    // wall placement affects 4 cells
    const struct Coordinate neighbors[] = {
        {0, 0},
        {1, 0},
        {0, 1},
        {1, 1}
    };
    uint8_t walls_backup[ARRAY_SIZE(neighbors)];

    if (player->wall_count == 0 || is_wall_clipping(x, y, dir))
    {
        move.as_uint32_t = -1;
        return move; // DO NOT MERGE THE PRECEDING LINE WITH THIS ONE
    }

    player->wall_count--;

    backup_walls(
        move.x, move.y, walls_backup, neighbors, ARRAY_SIZE(neighbors));

    place_tmp_wall(dir, move.x, move.y);

    if (!is_wall_valid(move.x, move.y, dir))
    {
        rollback_walls(
            move.x, move.y, walls_backup, neighbors, ARRAY_SIZE(neighbors));

        move.as_uint32_t = -1;
        return move; // DO NOT MERGE THE PRECEDING LINE WITH THIS ONE
    }

    wall_sprite->draw(board.board[move.x][move.y].x + // add cell height offset
                          (dir == HORIZONTAL ? 0 : empty_square.width),
                      board.board[move.x][move.y].y + // add cell width offset
                          (dir == HORIZONTAL ? empty_square.height : 0));

    dyn_array_push(board.moves, move.as_uint32_t);
    if (CAN_enabled && current_player != opponent)
    {
        CAN_TxMsg.data[0] = move.bytes[0];
        CAN_TxMsg.data[1] = move.bytes[1];
        CAN_TxMsg.data[2] = move.bytes[2];
        CAN_TxMsg.data[3] = move.bytes[3];
        CAN_TxMsg.len = 4;
        CAN_TxMsg.id = 1;
        CAN_TxMsg.format = STANDARD_FORMAT;
        CAN_TxMsg.type = DATA_FRAME;
        CAN_wrMsg(1, &CAN_TxMsg);
    }
    return move;
}

void place_tmp_wall(const enum Direction dir, const uint8_t x, const uint8_t y)
{
    switch (dir)
    {
    case HORIZONTAL:
        board.board[x][y].walls.bottom = true;
        board.board[x + 1][y].walls.bottom = true;
        board.board[x][y + 1].walls.top = true;
        board.board[x + 1][y + 1].walls.top = true;
        break;
    case VERTICAL:
        board.board[x][y].walls.right = true;
        board.board[x + 1][y].walls.left = true;
        board.board[x][y + 1].walls.right = true;
        board.board[x + 1][y + 1].walls.left = true;
        break;
    }
}
