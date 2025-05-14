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


#ifndef KOI_OBJECT_H
#define KOI_OBJECT_H


#include "koi_object/string_name.hpp"
#include "koi_object/variant_reference.hpp"

#include <cstdint>
#include <unordered_map>
#include <type_traits>
#include <functional>
#include <utility>


namespace Koi {

/**
 * A class that provides runtime, system-agnostic type information and indexed access and mutation of its properties and
 * provides factory methods for creating new instances to the heap at runtime.
 */
class Object {
protected:
    static std::unordered_map<const StringName, std::function<Object*()>, StringNameHash> _object_factory_methods;
    static std::unordered_map<const StringName, const std::type_info*, StringNameHash> _object_type_infos;
    static std::unordered_map<const std::type_info*, const StringName> _object_class_names;

    std::unordered_map<const StringName, VarRef, StringNameHash> _property_map;

public:
    template<class T>
    static typename std::enable_if<!std::is_pointer<T>::value, void>::type
    register_object_class(const StringName& class_name);

    static const std::type_info& get_class_type_info(const StringName& class_name);
    static void register_factory_method(const StringName& class_name, const std::function<Object*()>& method);
    static Object* create_new(const StringName& class_name);

    Object() = default;
    virtual ~Object() = default;

    Object(std::initializer_list<std::pair<const StringName, VarRef>> properties);

    const StringName& get_class_name() const;

    template<typename T>
    typename std::enable_if<!std::is_pointer<T>::value, std::pair<T, bool>>::type
    get(const StringName& property_name) const;

    template<typename T>
    typename std::enable_if<std::is_pointer<T>::value, std::pair<T, bool>>::type
    get(const StringName& property_name) const;

    template<typename T>
    bool set(const StringName& property_name, const T& value);

    bool has_property(const StringName& property_name) const;

    const std::unordered_map<const StringName, VarRef, StringNameHash>& get_property_list() const;

    //todo:: implement more types
    virtual const std::string to_json_string() const;

};


    template<class T>
    typename std::enable_if<!std::is_pointer<T>::value, void>::type
    Object::register_object_class(const StringName& class_name) {
        static_assert(std::is_base_of<Object, T>::value, "Type T must be an Object type.");
        _object_type_infos.emplace(class_name, &typeid(T));
        _object_class_names.emplace(&typeid(T), class_name);
    }

    template<typename T>
    typename std::enable_if<!std::is_pointer<T>::value, std::pair<T, bool>>::type
    Object::get(const StringName& property_name) const {
        static_assert(std::is_default_constructible<T>::value || std::is_trivial<T>::value, "Type T must have a default constructor.");
        static_assert(std::is_scalar<T>::value, "Type T must be a scalar type.");
        auto it = _property_map.find(property_name);
        if (it != _property_map.end()) {
            return it->second.get<T>();
        } else {
            return {static_cast<T>(0), false};
        }
    }

    template<typename T>
    typename std::enable_if<std::is_pointer<T>::value, std::pair<T, bool>>::type
    Object::get(const StringName& property_name) const {
        static_assert(!std::is_scalar<T>::value, "Type T must be a pointer to a non-scalar type.");
        auto it = _property_map.find(property_name);
        if (it != _property_map.end()) {
            return it->second.get<T>();
        } else {
            return {nullptr, false};
        }
    }

    template<typename T>
    bool Object::set(const StringName& property_name, const T& value) {
        auto it = _property_map.find(property_name);
        if (it == _property_map.end()) {
            return false;
        }

        return it->second.set(value);
    }

} // Koi

#endif //KOI_OBJECT_H
