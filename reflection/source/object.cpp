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


#include "koi_object/object.hpp"

#include <sstream>


namespace Koi {

std::unordered_map<const StringName, std::function<Object*()>, StringNameHash> Object::_object_factory_methods {};

std::unordered_map<const StringName, const std::type_info*, StringNameHash> Object::_object_type_infos {};
std::unordered_map<const std::type_info*, const StringName> Object::_object_class_names {};


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


Object* Object::create_new(const StringName& class_name) {
    auto it = _object_factory_methods.find(class_name);
    if (it == _object_factory_methods.end()) {
        return nullptr;
    }

    return it->second();
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


bool Object::has_property(const StringName& property_name) const {
    return _property_map.find(property_name) != _property_map.end();
}


const std::unordered_map<const StringName, VarRef, StringNameHash>& Object::get_property_list() const {
    return _property_map;
}

const std::string Object::to_json_string() const {
    std::ostringstream json_stream;

    json_stream << "{";

    json_stream << "\"";
    json_stream << "class_name";
    json_stream << "\"";
    json_stream << ":";
    json_stream << "\"";
    json_stream << get_class_name().get_string();
    json_stream << "\",";

    for (const auto& it : _property_map) {
        json_stream << "\"";
        json_stream << it.first.get_string();
        json_stream << "\"";
        json_stream << ":";

        if (typeid(bool) == it.second.type) {
            json_stream << (it.second.get<bool>().first ? "true" : "false");
        } else if (typeid(int) == it.second.type) {
            json_stream << it.second.get<int>().first;
        } else if (typeid(float) == it.second.type) {
            json_stream << it.second.get<float>().first;
        } else if (typeid(char) == it.second.type) {
            json_stream << "\'";
            json_stream << it.second.get<char>().first;
            json_stream << "\'";
        } else if (typeid(std::string*) == it.second.type) {
            json_stream << "\"";
            json_stream << *(it.second.get<std::string*>().first);
            json_stream << "\"";
        }


        json_stream << ",";
    }

    if (json_stream.tellp() > 0) {
        json_stream.seekp(-1, std::ios_base::end);
    }

    json_stream << "}";

    return json_stream.str();
}

} // Koi