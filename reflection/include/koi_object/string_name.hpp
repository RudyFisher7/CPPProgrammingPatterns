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


#ifndef KOI_STRING_NAME_HPP
#define KOI_STRING_NAME_HPP

#include "koi_object/string_name_registry.hpp"

#include <cmath>
#include <string>


namespace Koi {

/**
 * A string type that provides 0(1) equality/hash operations.
 * @note For performance gain to be seen, the application's source code should reuse StringName instances as much as
 * possible, as constructing new instances is expensive 0(2N).
 */
class StringName {
protected:
    static std::string _empty;

    const std::string* _pointer;

public:
    StringName();
    ~StringName() = default;
    StringName(char* value);
    StringName(std::string& value);

    const std::string& get_string() const;

    bool operator==(const StringName& rhs) const;
    bool operator!=(const StringName& rhs) const;

    friend struct StringNameHash;
};

    struct StringNameHash {
    private:
        static std::hash<const void*> h;
    public:
        size_t operator()(const StringName& value) const;
    };

};


#endif //KOI_STRING_NAME_HPP
