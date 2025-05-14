#ifndef KOI_OBJECT_H
#define KOI_OBJECT_H


#include "koi_object/string_name.hpp"
#include "koi_object/variant_reference.hpp"

#include <cstdint>
#include <unordered_map>
#include <type_traits>
#include <functional>


namespace Koi {

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

    Object() = default;
    ~Object() = default;

    Object(std::initializer_list<std::pair<const StringName, VarRef>> properties);

    const StringName& get_class_name() const;

    template<typename T>
    typename std::enable_if<!std::is_pointer<T>::value, std::pair<T, bool>>::type
    get(const StringName& property_name) const;

    template<typename T>
    typename std::enable_if<std::is_pointer<T>::value, std::pair<T, bool>>::type
    get(const StringName& property_name) const;

    template<typename T>
    bool set(const StringName& property_name, const T& value) const;
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
        auto it = _property_map.find(property_name);
        if (it != _property_map.end()) {
            return {it->second.get<T>(), true};
        } else {
            return {T(), false};
        }
    }

    template<typename T>
    typename std::enable_if<std::is_pointer<T>::value, std::pair<T, bool>>::type
    Object::get(const StringName& property_name) const {
        auto it = _property_map.find(property_name);
        if (it != _property_map.end()) {
            return {it->second.get<T>(), true};
        } else {
            return {nullptr, false};
        }
    }

    template<typename T>
    bool Object::set(const StringName& property_name, const T& value) const {
        auto it = _property_map.find(property_name);
        if (it == _property_map.end()) {
            return false;
        }

        return it->second.set(value);
    }


class MockObject : public Object {
private:
    static bool is_class_registered;
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
        if (!is_class_registered) {//fixme:: put in an application startup function instead.
            register_object_class<MockObject>("MockObject");
            register_factory_method("MockObject", []() -> Object * { return new MockObject(); });
        }
    }
};

} // Koi

#endif //KOI_OBJECT_H
