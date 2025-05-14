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
