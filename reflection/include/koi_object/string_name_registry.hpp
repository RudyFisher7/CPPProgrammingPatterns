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


#ifndef KOI_STRING_NAME_REGISTRY_HPP
#define KOI_STRING_NAME_REGISTRY_HPP


#include <unordered_set>
#include <string>
#include <mutex>


namespace Koi {

class StringNameRegistry final {
private:
    static std::unordered_set<std::string> _interned_strings;
    static std::mutex _interned_strings_mutex;
public:
    static StringNameRegistry& get_singleton();

    StringNameRegistry(const StringNameRegistry& rhs) = delete;
    StringNameRegistry(StringNameRegistry&& rhs) = delete;
    StringNameRegistry& operator=(const StringNameRegistry& rhs) = delete;
    StringNameRegistry& operator=(StringNameRegistry&& rhs) = delete;

    const std::string* register_name(std::string& value);

private:
    StringNameRegistry() = default;
    ~StringNameRegistry() = default;
};

} // Koi

#endif //KOI_STRING_NAME_REGISTRY_HPP
