#ifndef KOI_VARIANT_REFERENCE_HPP
#define KOI_VARIANT_REFERENCE_HPP


#include <cstdint>
#include <typeinfo>
#include <type_traits>


namespace Koi {

/**
 * A class that stores a reference to another variable and basic type information.
 * @warning This class does not own its variable, so memory management must happen outside of
 * this class.
 * fixme:: doesn't work with object pointers.
 */
class VarRef final {
public:
    enum Type : size_t {
        TYPE_MIN = 0,
        TYPE_NULL = TYPE_MIN,
        TYPE_BOOL,
        TYPE_CHAR,
        TYPE_UCHAR,
        TYPE_SHORT,
        TYPE_USHORT,
        TYPE_INT,
        TYPE_UINT,
        TYPE_LONG,
        TYPE_ULONG,
        TYPE_LONG_LONG,
        TYPE_ULONG_LONG,
        TYPE_FLOAT,
        TYPE_DOUBLE,
        TYPE_OBJECT,
        TYPE_SIZE,
    };

private:
    void* _pointer;

    const std::type_info& _type;

public:
    VarRef();
    ~VarRef() = default;

//    VarRef(bool& value);
//    VarRef(char& value);
//    VarRef(unsigned char& value);
//    VarRef(short& value);
//    VarRef(unsigned short& value);
//    VarRef(int& value);
//    VarRef(unsigned int& value);
//    VarRef(long& value);
//    VarRef(unsigned long& value);
//    VarRef(long long& value);
//    VarRef(unsigned long long& value);

    template<typename T>
    struct is_pointer_to_pointer {
        static constexpr bool value = std::is_pointer<T>::value && std::is_pointer<typename std::remove_pointer<T>::type>::value;
    };

    template<class T, typename = typename std::enable_if<!std::is_pointer<T>::value, VarRef>::type>
    VarRef(T& value) : _pointer(&value), _type(typeid(value)) {
        static_assert(std::is_scalar<T>::value, "Type T must be a scalar type.");
    }

    template<class T, typename = typename std::enable_if<(std::is_pointer<T>::value && !is_pointer_to_pointer<T>::value), VarRef>::type>
    VarRef(T value) : _pointer(value), _type(typeid(value)) {
        static_assert(!std::is_scalar<std::remove_pointer<T>>::value, "Type T must be a pointer to a non-scalar type.");
    }

    template<typename T>
    typename std::enable_if<!std::is_pointer<T>::value, const T&>::type
    get() {
        static_assert(std::is_default_constructible<T>::value || std::is_trivial<T>::value, "Type T must have a default constructor.");
        static_assert(std::is_scalar<T>::value, "Type T must be a scalar type.");
        if (typeid(T) == _type) {
            return *static_cast<T*>(_pointer);
        } else {
            return {};
        }
    }

    template<typename T>
    typename std::enable_if<std::is_pointer<T>::value, T>::type
    get() {
        static_assert(!is_pointer_to_pointer<T>::value, "Type T must be a pointer type that doesn't point to another pointer.");
        static_assert(!std::is_scalar<std::remove_pointer<T>>::value, "Type T must be a pointer to a non-scalar type.");
        if (typeid(T) == _type) {
            return static_cast<T>(_pointer);
        } else {
            return nullptr;
        }
    }

    template<typename T>
    bool set(const T& value) {
        if (typeid(T) != _type) {
            return false;
        }

        T& ref = *static_cast<T*>(_pointer);
        ref = value;

        return true;
    }
};

} // Koi

#endif //KOI_VARIANT_REFERENCE_HPP
