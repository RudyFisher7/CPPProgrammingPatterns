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
 * It uses an array of Block structs that each store a pointer to the allocated data and its own metadata.
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
 * Contains a pointer to allocated memory, if any, and metadata necessary for managing (de)allocations.
 * next: a pointer to the next memory block in the static memory pool. Should be either the next Block that has memory
 * allocated, or the next free Block.
 * previous: a pointer to the previous memory block in the static memory pool. Should be either the previous Block that has
 * memory allocated, or the previous free Block.
 */
typedef struct block_t {
    struct block_t* next;
    struct block_t* previous;
    size_t index;
    size_t capacity;
    size_t size;
    char* data;
} Block;



static Block memory_pool[KOI_MEMORY_POOL_SIZE];
static Block* free_list = NULL;
static Block* start = NULL;


size_t koi_static_get_block_size(void) {
    return sizeof(Block);
}


void koi_static_init(void) {
    memory_pool[0u].next = NULL;
    memory_pool[0u].previous = NULL;
    memory_pool[0u].capacity = KOI_MEMORY_POOL_SIZE - 1u;
    memory_pool[0u].index = 0u;
    memory_pool[0u].size = 0u;

    free_list = &memory_pool[0u];
    start = &memory_pool[0u];
}


void* koi_static_alloc(size_t size) {
    // if the memory pool is full, fail
    if (free_list == NULL || size == 0u) {
        return NULL;
    }

    // get the number of blocks needed, rounding the bytes needed up to the nearest division of sizeof(Block)
    size_t blocks_needed = (size + sizeof(Block) - 1u) / sizeof(Block);

    // if there aren't enough blocks in this section of the memory pool, search for a section later in the memory pool to use
    Block* result = free_list;
    while (result != NULL && result->capacity < blocks_needed) {
        result = result->next;
    }

    // if there wasn't enough space later in the memory pool, fail
    if (result == NULL) {
        return NULL;
    }

    size_t next_index = result->index + blocks_needed + 1u;
    if (next_index < KOI_MEMORY_POOL_SIZE) {
        Block *old_next = result->next;
        Block *new_next = &memory_pool[next_index];

        // if there is some space after this allocation and before the next section (or the end of the memory pool)
        if (new_next != NULL && new_next != old_next) {
            new_next->index = next_index;
            new_next->next = old_next;
            result->next = new_next;
            result->next->previous = result;
            new_next->size = 0u;
            new_next->capacity = result->capacity - new_next->index;
        }

        if (result == free_list) {
            free_list = &memory_pool[result->index + blocks_needed + 1u];
        }
    } else {
        free_list = NULL;
    }

    result->size = blocks_needed;
    result->data = (char*)&memory_pool[result->index + 1u];
    result->capacity = 0u;
    memset(result->data, '\0', size);

    return result->data;
}


void* koi_static_free(void* ptr) {
    if (ptr == NULL) {
        return NULL;
    }

    Block* block = start;
    while (block && block->data != ptr) {
        block = block->next;
    }

    if (block->next == NULL && block->previous == NULL) {
        block->capacity = block->size;
        block->size = 0u;
        free_list = block;
        return NULL;
    }

    size_t new_capacity = block->size;

    // if the next block has free space, merge this block with it for a contiguous section
    if (block->next != NULL && block->next->capacity > 0u) {
        // add the next block's capacity, +1 because we can use the next block as part of the next memory allocation
        new_capacity += block->next->capacity + 1u;
        block->next = block->next->next;
    }

    // if the previous block has free space, merge it with this block as well
    if (block->previous != NULL && block->previous->capacity > 0u) {
        Block *previous = block->previous;

        // add the previous block's capacity, +1 because we can use this block as part of the next memory allocation
        new_capacity += previous->capacity + 1u;

        previous->next = block->next;
        block = previous;
    }

    // clean the block
    block->capacity = new_capacity;
    block->size = 0u;
    block->data = NULL;

    // update the free list to be the earliest in the memory pool
    if (free_list == NULL || block->index < free_list->index) {
        free_list = block;
    }

    return NULL;
}
