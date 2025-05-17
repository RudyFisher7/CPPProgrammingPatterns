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


#ifndef STATIC_ALLOCATORS_FREE_LIST_ALLOCATOR_H
#define STATIC_ALLOCATORS_FREE_LIST_ALLOCATOR_H




#ifdef __cplusplus
#include <cstdlib>
extern "C" {
#else
#include <stdlib.h>
#endif

#ifndef KOI_HEAP_SIZE
#define KOI_HEAP_SIZE 16u
#endif

/**
 * Must be updated if internal Block struct is changed.
 */
#ifndef KOI_HEAP_BLOCK_SIZE
#define KOI_HEAP_BLOCK_SIZE 64u
#endif

/**
 * Initializes the static memory pool for use.
 */
extern void koi_static_init(void);

/**
 * Allocates the number of bytes to the memory pool, if enough exists.
 * @param size The number of bytes to allocate. If 0, does nothing.
 * @return A pointer to the first byte in memory if successful, or NULL if couldn't allocate.
 */
extern void* koi_static_alloc(size_t size);

/**
 * Frees the memory allocated starting at the given pointer.
 * @param ptr The pointer at the first byte of allocated memory that needs to be freed. If NULL, does nothing.
 */
extern void koi_static_free(void* ptr);

#ifdef __cplusplus
};
#endif


#endif //STATIC_ALLOCATORS_FREE_LIST_ALLOCATOR_H
