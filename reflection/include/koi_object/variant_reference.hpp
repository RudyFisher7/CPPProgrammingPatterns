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

    template<class T>
    VarRef(T& value) : _pointer(&value), _type(typeid(T)) {}

    template<typename T>
    typename std::enable_if<!std::is_pointer<T>::value, T>::type
    get() {
        static_assert(std::is_default_constructible<T>::value || std::is_trivial<T>::value, "Type T must have a default constructor.");
        if (typeid(T) == _type) {
            return *static_cast<T*>(_pointer);
        } else {
            return T();
        }
    }

    template<typename T>
    typename std::enable_if<std::is_pointer<T>::value, T>::type
    get() {
        if (typeid(T) == _type) {
            return *static_cast<T*>(_pointer);
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
