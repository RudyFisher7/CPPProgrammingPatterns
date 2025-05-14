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


#include "mock_object.hpp"
#include "koi_pub_sub/serialization/serialization.hpp"

#include <iostream>


MockData &MockData::operator=(const MockData &rhs) {
    if (this != &rhs) {
        integer = rhs.integer;
        character = rhs.character;
        boolean = rhs.boolean;
        floating_point_number = rhs.floating_point_number;
        big_float = rhs.big_float;
        unsigned_integer = rhs.unsigned_integer;
        big_integer = rhs.big_integer;
    }

    return *this;
}

void MockData::to_network_bytes(std::vector<uint8_t>& out_bytes) {
    Serialization::to_network_bytes(
            out_bytes,
            integer,
            character,
            boolean,
            floating_point_number,
            big_float,
            unsigned_integer,
            big_integer
    );
}

bool MockData::from_network_bytes(const std::vector<uint8_t>& in_bytes) {
    return Serialization::from_network_bytes(
            in_bytes.data(),
            in_bytes.data() + in_bytes.size(),
            integer,
            character,
            boolean,
            floating_point_number,
            big_float,
            unsigned_integer,
            big_integer
    );
}

bool MockData::operator==(const MockData &rhs) const {
    return integer == rhs.integer
           && character == rhs.character
           && boolean == rhs.boolean
           && floating_point_number == rhs.floating_point_number
           && big_float == rhs.big_float
           && unsigned_integer == rhs.unsigned_integer
           && big_integer == rhs.big_integer;
}

bool MockData::operator!=(const MockData &rhs) const {
    return !(rhs == *this);
}

std::ostream &operator<<(std::ostream &os, const MockData &data) {
    os << "MockData: integer: " << data.integer << " character: " << data.character
       << " boolean: " << data.boolean << " floating_point_number: " << data.floating_point_number << " big_float: "
       << data.big_float << " unsigned_integer: " << data.unsigned_integer << " big_integer: " << data.big_integer;
    return os;
}


void MockObject::on_published(const Data &in_data) {
    const auto* mock = dynamic_cast<const MockData*>(&in_data);
    if (mock) {
        data = *mock;
    }
}
