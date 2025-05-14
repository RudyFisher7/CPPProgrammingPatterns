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


#ifndef KOI_PUB_SUB_TEST_MOCK_OBJECT_HPP
#define KOI_PUB_SUB_TEST_MOCK_OBJECT_HPP


#include <ostream>
#include "koi_pub_sub/callable.hpp"
#include "koi_pub_sub/models/data.hpp"


using namespace KoiPubSub;

/**
 * A mock data object for unit testing purposes.
 *
 * @warning Not all operators and constructors may be implemented. Implement them
 * as needed for unit testing the core features.
 */
class MockData : public Data {
public:
    int integer = 0;
    char character = 'A';
    bool boolean = true;
    float floating_point_number = 2.0f;
    double big_float = 3.0;
    unsigned int unsigned_integer = 9u;
    long big_integer = 122l;

    MockData& operator=(const MockData& rhs);

    void to_network_bytes(std::vector<uint8_t>& out_bytes) override;

    bool from_network_bytes(const std::vector<uint8_t>& in_bytes) override;

    bool operator==(const MockData &rhs) const;

    bool operator!=(const MockData &rhs) const;

    friend std::ostream &operator<<(std::ostream &os, const MockData &data);
};


/**
 * A mock object for unit testing purposes.
 *
 * @warning Not all operators and constructors may be implemented. Implement them
 * as needed for unit testing the core features.
 */
class MockObject {
public:
    MockData data;
    void on_published(const Data& data);
};


#endif //KOI_PUB_SUB_TEST_MOCK_OBJECT_HPP
