//
// Created by rfish on 5/14/2025.
//

#ifndef KOI_STRING_NAMES_HPP
#define KOI_STRING_NAMES_HPP


#include "koi_object/string_name.hpp"


namespace Koi {

// Common StringNames for reuse at the bottom of this struct.
struct StringNames {
private:
    StringNames() = default;
    ~StringNames() = default;

public:
    static StringNames& get_singleton() {
        static StringNames _instance;
        return _instance;
    }

    StringNames(const StringNames& rhs) = delete;
    StringNames(StringNames&& rhs) = delete;
    StringNames& operator=(const StringNames& rhs) = delete;
    StringNames& operator=(StringNames&& rhs) = delete;


    const Koi::StringName OBJECT{"Object"};
};

}

#endif //KOI_STRING_NAMES_HPP
