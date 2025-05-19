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


#define KOI_MEMORY_POOL_SIZE 16u
#include "static_allocators/allocator.h"

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>


const size_t m5_size = 7u;
typedef struct test_struct_t {
    int m1;
    int m2;
    float m3;
    double m4;
    char m5[m5_size];
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
    size_t bytes_available = KOI_MEMORY_POOL_SIZE * koi_static_get_block_size();
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
    TestStruct values {
            80,
            80,
            80.033f,
            80.033,
            {'A'}
    };

    TestStruct* s1 = (TestStruct*) koi_static_alloc(sizeof(TestStruct));
    TestStruct* s2 = (TestStruct*) koi_static_alloc(sizeof(TestStruct));

    REQUIRE(s1 != nullptr);

    s1->m1 = values.m1;
    s1->m2 = values.m2;
    s1->m3 = values.m3;
    s1->m4 = values.m4;
    s1->m5[0u] = values.m5[0u];

    INFO("TestStruct member assignments executed.");
    REQUIRE(true);

    memcpy(s2, s1, sizeof(TestStruct));

    CHECK(s1->m1 == s2->m1);
    CHECK(s1->m2 == s2->m2);
    CHECK(s1->m3 == s2->m3);
    CHECK(s1->m4 == s2->m4);

    for (size_t i = 0u; i < m5_size; ++i) {
        CHECK(s1->m5[i] == s2->m5[i]);
    }

    s1 = (TestStruct*) koi_static_free(s1);
    REQUIRE(s1 == nullptr);
    CHECK(s2->m1 == values.m1);
    CHECK(s2->m2 == values.m2);
    CHECK(s2->m3 == values.m3);
    CHECK(s2->m4 == values.m4);

    for (size_t i = 0u; i < m5_size; ++i) {
        CHECK(s2->m5[i] == values.m5[i]);
    }
}


int main(int argc, char* argv[]) {
    // your setup ...
    koi_static_init();

    int result = Catch::Session().run(argc, argv);

    // your clean-up...

    return result;
}