/*
 * MIT License
 *
 * Copyright (c) 2025 Rudy Fisher (kiyasui-hito)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


/**
 * A static allocator that uses a doubly-linked free list and other metadata to manage its memory allocations.
 * It uses an array of Block structs that each store a pointer to the allocated data and its metadata.
 */


#include "static_allocators/allocator.h"

#include <stddef.h>

/**
 * Contains pointer to allocated memory if any.
 * Contains metadata for managing its memory allocations.
 * next: a pointer to the next memory block in the static heap. Should be either the next Block that has memory
 * allocated, or the next free Block.
 * previous: a pointer to the previous memory block in the static heap. Should be either the previous Block that has
 * memory allocated, or the previous free Block.
 */
typedef struct block_t {
    struct block_t* next;
    struct block_t* previous;
    size_t capacity;
    size_t index;
    size_t size;
    char* data;
} Block;


static Block heap[KOI_HEAP_SIZE];
static Block* free_list = NULL;


void koi_static_init(void) {
    heap[0u].next = &heap[KOI_HEAP_SIZE - 1u];
    heap[0u].previous = NULL;
    heap[0u].capacity = KOI_HEAP_SIZE - 1u;
    heap[0u].index = 0u;
    heap[0u].size = 0u;

    for (size_t i = 1u; i < KOI_HEAP_SIZE - 1u; ++i) {
        heap[i].next = NULL;
        heap[i].previous = NULL;
        heap[i].capacity = (KOI_HEAP_SIZE - i + 1) * sizeof(Block);
        heap[i].index = i;
        heap[i].size = 0u;
    }

    heap[KOI_HEAP_SIZE - 1u].next = NULL;
    heap[KOI_HEAP_SIZE - 1u].previous = &heap[0u];
    heap[KOI_HEAP_SIZE - 1u].capacity = 0u;
    heap[KOI_HEAP_SIZE - 1u].index = KOI_HEAP_SIZE - 1u;
    heap[KOI_HEAP_SIZE - 1u].size = 0u;

    free_list = &heap[0u];
}


void* koi_static_alloc(size_t size) {
    if (free_list == NULL) {
        return NULL;
    }

    size_t blocks_needed = (size + sizeof(Block) - 1u) / sizeof(Block);

    // there might be space later in the array, so use it if needed
    Block* result = free_list;
    while (result != NULL && result->capacity < blocks_needed) {
        result = result->next;
    }

    if (result == NULL) {
        return NULL;
    }

    result->size = blocks_needed;
    result->data = (char*)&heap[result->index + 1u];
    free_list = &heap[result->index + blocks_needed + 1u];
    result->next = free_list;
    free_list->previous = result;

    return result->data;
}


void koi_static_free(void* ptr) {
    Block* block = (Block*)((char*)ptr - offsetof(Block, data));

    // if block is before the free list
    if (block->next == free_list) {
        for (size_t i = block->index; i < free_list->index; ++i) {
            heap[i].next = &heap[i + 1u];
            heap[i].previous = &heap[i - 1u];
            heap[i].capacity = free_list->capacity + free_list->index - i;
            heap[i].index = i;
            heap[i].size = 0u;
        }

        free_list->previous = NULL;
        block->next = free_list->next;
        free_list->next = NULL;
        free_list = block;

        // if block is after free list
    } else { //todo:: implement this case
        Block* current = block;

        // if there are free blocks before this block,
        // use the
        if (current->previous != NULL && current->previous->size == 0u) {
            Block *previous = block->previous;
            while (previous->previous->size == 0u) {
                previous = previous->previous;
            }

            current = previous;
        }

        size_t end_index = block->index + block->size;
        if (current->next == NULL) {
            --end_index;
            heap[KOI_HEAP_SIZE - 1u].next = NULL;
            heap[KOI_HEAP_SIZE - 1u].previous = &heap[KOI_HEAP_SIZE - 2u];
            heap[KOI_HEAP_SIZE - 1u].capacity = 0u;
            heap[KOI_HEAP_SIZE - 1u].index = KOI_HEAP_SIZE - 1u;
            heap[KOI_HEAP_SIZE - 1u].size = 0u;
        }

        for (size_t i = current->index; i < end_index; ++i) {
            heap[i].next = &heap[i + 1u];
            heap[i].previous = &heap[i - 1u];
            heap[i].capacity = (KOI_HEAP_SIZE - i + 1) * sizeof(Block);
            heap[i].index = i;
            heap[i].size = 0u;
        }
    }
}
