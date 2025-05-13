#ifndef KOI_PUB_SUB_CALLABLE_H
#define KOI_PUB_SUB_CALLABLE_H


#include "koi_pub_sub/models/data.hpp"

#include <functional>
#include <cstdint>


namespace KoiPubSub {

class Callable {
public:
    uint64_t id;
    std::function<void(const Data &)> callable;

public:
    template<class T>
    Callable(T &instance, void (T::*function)(const Data &)) {
        callable = std::bind(function, &instance, std::placeholders::_1);
    }

    virtual ~Callable() = default;

    Callable(const Callable &rhs) = default;

    Callable(Callable &&rhs) = default;

    Callable &operator=(const Callable &rhs) = default;

    Callable &operator=(Callable &&rhs) = default;
};

};

#endif //KOI_PUB_SUB_CALLABLE_H
