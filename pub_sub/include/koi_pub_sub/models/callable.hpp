#ifndef KOI_PUB_SUB_CALLABLE_H
#define KOI_PUB_SUB_CALLABLE_H


#include "koi_pub_sub/models/data.hpp"

#include <functional>
#include <cstdint>




namespace KoiPubSub {

struct Callable {
    uint64_t id;
    std::function<void(const Data&)> callable;
};
}

#endif //KOI_PUB_SUB_CALLABLE_H
