#pragma once

#include "../../common.h"
#include <stdbool.h>
#include <stdint.h>

#define STACK_SIZE ((BOARD_SIZE) * (BOARD_SIZE))

/**
 * @brief stack of Coordinate type elements
 *
 */
struct Stack
{
    struct Coordinate elems[STACK_SIZE];
    int8_t top;
};

void stack_init(struct Stack *stack, uint32_t size);
void push(struct Stack *stack, uint16_t x, uint16_t y);
struct Coordinate pop(struct Stack *stack);
bool is_empty(struct Stack *stack);
