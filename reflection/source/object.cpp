#include "koi_object/object.hpp"

namespace Koi {

std::unordered_map<const StringName, std::function<Object*()>, StringNameHash> _object_factory_methods {
        {"Object", []() -> Object* { return new Object(); }}
};

std::unordered_map<const StringName, const std::type_info*, StringNameHash> Object::_object_type_infos {{"Object", &typeid(Object)}};
std::unordered_map<const std::type_info*, const StringName> Object::_object_class_names {{&typeid(Object), "Object"}};


const std::type_info& Object::get_class_type_info(const StringName& class_name) {
    auto it = _object_type_infos.find(class_name);
    if (it == _object_type_infos.end()) {
        return typeid(nullptr);
    }

    return *it->second;
}


void Object::register_factory_method(const StringName& class_name, const std::function<Object*()>& method) {
    _object_factory_methods.emplace(class_name, method);
}


Object::Object(std::initializer_list<std::pair<const StringName, VarRef>> properties) {
    for (const auto& property : properties) {
        _property_map.emplace(property.first, property.second);
    }
}


const StringName& Object::get_class_name() const {
    auto it = _object_class_names.find(&typeid(*this));
    if (it == _object_class_names.end()) {
        return StringName::EMPTY;
    }

    return it->second;
}


bool MockObject::is_class_registered {false};

} // Koi