//
// Created by rfish on 5/13/2025.
//

#include "koi_object/object.hpp"

namespace Koi {

Object::Object(std::initializer_list<std::pair<const StringName, VarRef>> properties) {
    for (const auto& property : properties) {
        _property_map.emplace(property.first, property.second);
    }
}

} // Koi