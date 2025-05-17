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

#ifdef __cplusplus
#include <cstddef>
#include <cstring>
#else
#include <stddef.h>
#include <string.h>
#endif


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
    char padding[16u];
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
        heap[i].capacity = KOI_HEAP_SIZE - i + 1u;
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
    // if the heap is full, fail
    if (free_list == NULL || size == 0u) {
        return NULL;
    }

    // get the number of blocks needed, rounding the bytes needed up to the nearest division of sizeof(Block)
    size_t bytes_needed = size + sizeof(Block) - 1u;
    size_t blocks_needed = bytes_needed / sizeof(Block);

    // if there aren't enough blocks in this section of the heap, search for a section later in the heap to use
    Block* result = free_list;
    while (result != NULL && result->capacity < blocks_needed) {
        result = result->next;
    }

    // if there wasn't enough space later in the heap, fail
    if (result == NULL) {
        return NULL;
    }

    result->size = blocks_needed;
    result->data = (char*)&heap[result->index + 1u];
    result->next = &heap[result->index + result->size + 1u];
    result->next->previous = result;
    result->capacity = 0u;
    memset(result->data, '\0', bytes_needed);

    if (result == free_list) {
        free_list = &heap[result->index + blocks_needed + 1u];
    }

    return result->data;
}


void koi_static_free(void* ptr) {
    if (ptr == NULL) {
        return;
    }

    Block* block = (Block*)((char*)ptr - offsetof(Block, data));

    // if block is just before the free list
    if (block->next == free_list) {
        free_list = block;
    }

    // if the next block has free space, merge this block with it for a contiguous section
    if (block->next->capacity > 0u) {
        block->next = block->next->next;
    }

    // if there are free blocks before this block
    if (block->previous != NULL && block->previous->capacity > 0u) {
        Block *current = block->previous;

        // get the earliest free block in the contiguous section of the heap
        while (current->previous != NULL && current->previous->capacity > 0u) {
            current = current->previous;
        }

        current->next = block->next;
        block = current;
    }

    // clean the block
    block->capacity = block->next->capacity + block->next->index - block->index - 1u;
    block->size = 0u;
    block->data = NULL;

    // finally, clean up the blocks in this section
    for (size_t i = block->index + 1u; i < block->next->index; ++i) {
        heap[i].next = NULL;
        heap[i].previous = NULL;
        heap[i].capacity = block->next->capacity + block->capacity - i;
        heap[i].index = i;
        heap[i].size = 0u;
    }
}
