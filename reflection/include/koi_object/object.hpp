//
// Created by rfish on 5/13/2025.
//

#ifndef KOI_OBJECT_H
#define KOI_OBJECT_H


#include "koi_object/string_name.hpp"

#include "koi_object/variant_reference.hpp"

#include <cstdint>
#include <unordered_map>
#include <type_traits>


namespace Koi {


class Object {
protected:
    std::unordered_map<const StringName, VarRef, StringNameHash> _property_map;

public:

    Object() = default;
    ~Object() = default;

    Object(std::initializer_list<std::pair<const StringName, VarRef>> properties);

    template<typename T>
    std::tuple<T, bool> get(const StringName& property_name) const {
        static_assert(std::is_default_constructible<T>::value || std::is_trivial<T>::value, "Type T must have a default constructor.");
        auto it = _property_map.find(property_name);
        if (it != _property_map.end()) {
            return {it->second.get<T>(), true};
        } else {
            return {T(), false};
        }
    }

    template<typename T>
    bool set(const StringName& property_name, const T& value) const {
        auto it = _property_map.find(property_name);
        if (it == _property_map.end()) {
            return false;
        }

        return it->second.set(value);
    }
};


class MockObject : public Object {
public:
    int pint = 0;
    float pfloat = 0.0f;
    bool pbool = false;

    MockObject() :
            pint(0),
            pfloat(0.0f),
            pbool(false),
            Object({
                {"pint",   pint},
                {"pfloat", pfloat},
                {"pbool",  pbool}
            })
    {

    }
};

} // Koi

#endif //KOI_OBJECT_H
