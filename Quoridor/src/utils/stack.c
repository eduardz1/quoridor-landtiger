#include "headers/stack.h"
#include <stdint.h>

void stack_init(struct Stack *stack, uint32_t size)
{
    stack->top = -1;
}

void push(struct Stack *stack, uint16_t x, uint16_t y)
{
    if (stack->top >= STACK_SIZE) return; // stack full

    stack->elems[++stack->top].x = x;
    stack->elems[stack->top].y = y;
}

struct Coordinate pop(struct Stack *stack)
{
    return (stack->top >= 0) ? stack->elems[stack->top--] :
                               (struct Coordinate){0}; // default to 0
}

bool is_empty(struct Stack *stack)
{
    return stack->top == -1;
}
