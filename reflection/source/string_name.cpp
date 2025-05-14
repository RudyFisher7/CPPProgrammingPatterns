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


#include "koi_object/string_name.hpp"


namespace Koi {

std::string StringName::_empty;
std::unordered_set<std::string> StringName::_interned_strings{_empty};

const StringName &StringName::EMPTY{};

StringName::StringName() : _pointer(&_empty) {

}

StringName::StringName(const char *value) {
    auto it = _interned_strings.emplace(value);
    _pointer = &(*it.first);
}

StringName::StringName(const std::string &value) {
    auto it = _interned_strings.emplace(value);
    _pointer = &(*it.first);
}

const std::string &StringName::get() const {
    if (_pointer) {
        return *_pointer;
    }

    return _empty;
}

bool StringName::operator==(const StringName &rhs) const {
    return _pointer == rhs._pointer;
}

bool StringName::operator!=(const StringName &rhs) const {
    return !(*this == rhs);
}


std::hash<const void *> StringNameHash::h{};

size_t StringNameHash::operator()(const StringName &value) const {
    return h(value._pointer);
}

}
