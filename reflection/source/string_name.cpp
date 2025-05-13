//
// Created by rfish on 5/13/2025.
//

#include "koi_object/string_name.hpp"

std::string Koi::StringName::_empty;
std::unordered_set<std::string> Koi::StringName::_interned_strings {_empty};

Koi::StringName::StringName() : _pointer(&_empty) {

}

Koi::StringName::StringName(const char* value) {
    auto it = _interned_strings.emplace(value);
    _pointer = &(*it.first);
}

Koi::StringName::StringName(const std::string &value) {
    auto it = _interned_strings.emplace(value);
    _pointer = &(*it.first);
}

const std::string &Koi::StringName::get() const {
    if (_pointer) {
        return *_pointer;
    }

    return _empty;
}

bool Koi::StringName::operator==(const Koi::StringName &rhs) const {
    return _pointer == rhs._pointer;
}

bool Koi::StringName::operator!=(const Koi::StringName &rhs) const {
    return !(*this == rhs);
}


std::hash<const void*> Koi::StringNameHash::h{};
size_t Koi::StringNameHash::operator()(const Koi::StringName& value) const {
    return h(value._pointer);
}
