#ifndef KOI_PUB_SUB_CALLABLE_H
#define KOI_PUB_SUB_CALLABLE_H


#include "koi_pub_sub/models/data.hpp"

#include <functional>
#include <cstdint>


namespace KoiPubSub {

class BaseCallable {
public:
    uint64_t id;
    std::function<void(const Data&)> callable;

public:
    BaseCallable() = default;
    virtual ~BaseCallable() = default;

    BaseCallable(const BaseCallable& rhs) = default;
    BaseCallable(BaseCallable&& rhs) = default;

    BaseCallable& operator=(const BaseCallable& rhs) = default;
    BaseCallable& operator=(BaseCallable&& rhs) = default;
};


template<class T>
class Callable : public BaseCallable {
public:
    Callable(T& instance, void (T::*function)(const Data&)) {
        callable = std::bind(function, &instance, std::placeholders::_1);
    }

    ~Callable() override = default;

    Callable(const Callable& rhs) = default;
    Callable(Callable&& rhs) = default;

    Callable& operator=(const Callable& rhs) = default;
    Callable& operator=(Callable&& rhs) = default;

    void operator()(const Data&);
};

    template<class T>
    void Callable<T>::operator()(const Data &data) {
        callable(data);
    }

}

#endif //KOI_PUB_SUB_CALLABLE_H
