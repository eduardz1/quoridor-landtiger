#pragma once

#include "comparable.h"
#include <stdint.h>

#define PARENT(i) ((i - 1) / 2)
#define LEFT(i) (2 * i + 1)
#define RIGHT(i) (2 * i + 2)

#define MIN_HEAP_SIZE ((BOARD_SIZE) * (BOARD_SIZE))

struct MinHeap
{
    void *data[MIN_HEAP_SIZE];
    uint8_t size;
    Comp comp;
};

void new_min_heap(struct MinHeap *heap, const Comp comp);

void min_heap_insert(struct MinHeap *heap, void *value);

void *min_heap_extract(struct MinHeap *heap);

void min_heap_update(struct MinHeap *heap, const uint8_t i, void *value);

void bottom_heapify(struct MinHeap *heap, uint8_t i);

void top_heapify(struct MinHeap *heap, uint8_t i);
