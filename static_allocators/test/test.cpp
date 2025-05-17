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


#define KOI_HEAP_SIZE 16u
#include "static_allocators/allocator.h"

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>


typedef struct test_struct_t {
    int m1;
    int m2;
    float m3;
    double m4;
    char m5[7u];
} TestStruct;


TEST_CASE("1 Allocation", "[Allocator]") {
    char* ptr = (char*)koi_static_alloc(16u * koi_static_get_block_size());

    CHECK((ptr == nullptr));

    ptr = (char*)koi_static_alloc(15u * koi_static_get_block_size());

    CHECK((ptr != nullptr));

    ptr = (char*)koi_static_free(ptr);

    CHECK((ptr == nullptr));
}


TEST_CASE("2 Allocations", "[Allocator]") {
    size_t block_size = koi_static_get_block_size();
    size_t bytes_available = KOI_HEAP_SIZE * koi_static_get_block_size();
    size_t bytes = (bytes_available / 2u) - koi_static_get_block_size();

    char* ptr = (char*)koi_static_alloc(bytes);
    char* ptr2 = (char*)koi_static_alloc(bytes);

    CHECK((ptr != nullptr));
    CHECK((ptr2 != nullptr));

    ptr = (char*)koi_static_free(ptr);
    ptr2 = (char*)koi_static_free(ptr2);

    CHECK((ptr == nullptr));
    CHECK((ptr2 == nullptr));
}


TEST_CASE("TestStruct", "[Allocator]") {
    TestStruct* s1 = (TestStruct*) koi_static_alloc(sizeof(TestStruct));

    REQUIRE(s1 != nullptr);

    s1->m1 = 80;
    s1->m2 = 80;
    s1->m3 = 80.033f;
    s1->m4 = 80.033;
    s1->m5[0u] = 'A';

    INFO("TestStruct member assignments executed.");
    REQUIRE(true);
}


int main(int argc, char* argv[]) {
    // your setup ...
    koi_static_init();

    int result = Catch::Session().run(argc, argv);

    // your clean-up...

    return result;
}