//
// Created by rfish on 5/13/2025.
//

#ifndef KOI_OBJECT_H
#define KOI_OBJECT_H


#include "koi_object/string_name.hpp"

#include <cstdint>
#include <unordered_map>
#include <utility>
#include <typeinfo>


namespace Koi {

template<typename ... TProperties>
struct ObjectPropertyIndexer {
    static constexpr auto make() {
        return std::index_sequence_for<TProperties...>{};
    }
};


template<typename ... TProperties>
class Object {
protected:
    std::tuple<TProperties&...> _properties;
//    std::unordered_map<StringName, size_t> _property_map
public:

//    Object() : _proper

    explicit Object(TProperties&... properties) : _properties(properties...) {}

    template<typename T>
    const T& get(const StringName& property_name) const {

    }

};


class MockObject : public Object<int, float, bool> {
public:
    int pint = 0;
    float pfloat = 0.0f;
    bool pbool = false;

    MockObject() : pint(0), pfloat(0.0f), pbool(false), Object<int, float, bool>(pint, pfloat, pbool) {

    }
};

} // Koi

#endif //KOI_OBJECT_H
