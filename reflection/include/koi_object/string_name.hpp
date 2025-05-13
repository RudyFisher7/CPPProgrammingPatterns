//
// Created by rfish on 5/13/2025.
//

#ifndef KOI_STRING_NAME_H
#define KOI_STRING_NAME_H


#include <unordered_set>
#include <string>


namespace Koi {

class StringName {
protected:
    static std::string _empty;
    static std::unordered_set<std::string> _interned_strings;

    const std::string* _pointer;

public:
    StringName();
    ~StringName() = default;
    explicit StringName(const std::string& value);

    const std::string& get() const;

    bool operator==(const StringName& rhs) const;
    bool operator!=(const StringName& rhs) const;
};

};


#endif //KOI_STRING_NAME_H
