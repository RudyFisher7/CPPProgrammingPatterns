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


#ifndef KOI_VARIANT_REFERENCE_HPP
#define KOI_VARIANT_REFERENCE_HPP


#include <cstdint>
#include <typeinfo>
#include <type_traits>
#include <utility>


template<typename T>
struct is_pointer_to_pointer {
    static constexpr bool value = std::is_pointer<T>::value && std::is_pointer<typename std::remove_pointer<T>::type>::value;
};

namespace Koi {

/**
 * A class that stores a reference to another variable and basic type information.
 * The variable may be a pointer to a non-scalar type, or a scalar/fundamental type.
 * @warning This class does not own its variable, so memory management must happen outside of
 * this class.
 * @note This class is not designed to store data directly, just a reference to data stored elsewhere and information
 * about that data's type.
 */
class VarRef final {
public:
    const std::type_info& type;

private:
    void* _pointer;


public:
    VarRef();
    ~VarRef() = default;

    template<class T, typename = typename std::enable_if<!std::is_pointer<T>::value, VarRef>::type>
    VarRef(T& value) : _pointer(&value), type(typeid(value)) {
        static_assert(std::is_scalar<T>::value, "Type T must be a scalar type.");
    }

    template<class T, typename = typename std::enable_if<(std::is_pointer<T>::value && !is_pointer_to_pointer<T>::value), VarRef>::type>
    VarRef(T value) : _pointer(value), type(typeid(value)) {
        static_assert(!std::is_scalar<std::remove_pointer<T>>::value, "Type T must be a pointer to a non-scalar type.");
    }

    template<typename T>
    typename std::enable_if<!std::is_pointer<T>::value, std::pair<T, bool>>::type
    get() const {
        static_assert(std::is_default_constructible<T>::value || std::is_trivial<T>::value, "Type T must have a default constructor.");
        static_assert(std::is_scalar<T>::value, "Type T must be a scalar type.");
        if (typeid(T) == type) {
            return {*static_cast<T *>(_pointer), true};
        } else {
            return {static_cast<T>(0), false};
        }
    }

    template<typename T>
    typename std::enable_if<std::is_pointer<T>::value, std::pair<T, bool>>::type
    get() const {
        static_assert(!is_pointer_to_pointer<T>::value, "Type T must be a pointer type that doesn't point to another pointer.");
        static_assert(!std::is_scalar<std::remove_pointer<T>>::value, "Type T must be a pointer to a non-scalar type.");
        if (typeid(T) == type) {
            return {static_cast<T>(_pointer), true};
        } else {
            return {nullptr, false};
        }
    }

    template<typename T>
    bool set(const T& value) {
        if (typeid(T) != type) {
            return false;
        }

        T& ref = *static_cast<T*>(_pointer);
        ref = value;

        return true;
    }
};

} // Koi

#endif //KOI_VARIANT_REFERENCE_HPP
