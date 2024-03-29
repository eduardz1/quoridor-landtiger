#pragma once

#include <stdbool.h>
#include <stdint.h>

void draw_board(void);

void draw_main_menu(void);

void draw_single_board_menu(void);

void draw_two_board_menu(void);

void draw_waiting_for_connection(void);

void draw_color_selection_menu(void);

void refresh_walls(void);

void write_invalid_move(void);

/**
 * @brief refreshes info panel located in the top-right
 *
 * @param timer timer number
 */
void refresh_info_panel(const uint8_t timer);

void highlight_possible_moves(void);

void clear_highlighted_moves(void);

void update_player_sprite(const uint8_t new_x, const uint8_t new_y);

/**
 * @brief updates the sprite of the player move selector
 * @pre be in PLAYER_MOVE mode
 *
 * @param up offset up (negative for down)
 * @param right offset right (negative for left)
 * @param show if set to false the selector clears itself
 * @return struct Coordinate with {x, y} the current offset
 */
struct Coordinate
update_player_selector(const int8_t up, const int8_t right, bool show);

/**
 * @brief updates the sprite of the wall selector
 * @pre be in WALL_PLACEMENT mode
 *
 * @param up offset up (negative for down)
 * @param right offset right (negative for left)
 * @param show if set to false the selector clears itself
 * @return struct Coordinate with {x, y} the current offset
 */
struct Coordinate
update_wall_selector(const int8_t up, const int8_t right, bool show);

/**
 * @brief updates the sprite of the menu selector
 *
 * @param up offset up (negative for down)
 * @param right offset right (negative for left)
 * @param show if set to false the selector clears itself
 * @return struct Coordinate with x always 0 and y either 0 (top button) or 1
 * (bottom button)
 */
struct Coordinate
update_menu_selector(const int8_t up, const int8_t right, bool show);
