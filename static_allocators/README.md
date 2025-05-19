# Static Allocators

This module implements static allocators in C99 because fun.

See unit test(s) inside test/ for usage examples.

## v0.0.1 Features
- An allocator.h provides the C API for the allocators.
- Implementations of the allocators are inside the source/ folder.
- Currently, 1 implementation exists, a (mostly) free list allocator.
- Some unit tests are implemented but many more edge cases would be covered if implementing an allocator for production use.