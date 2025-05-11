#ifndef KOI_PUB_SUB_SERVER_H
#define KOI_PUB_SUB_SERVER_H

#include "koi_pub_sub/models/callable.hpp"
#include "koi_pub_sub/models/data.hpp"

#include <cstdint>
#include <functional>
#include <map>


namespace KoiPubSub {

class Server {
protected:
    // std::map<channel, std::map<callable_id, function>>
    std::map<uint64_t, std::map<uint64_t, BaseCallable>> subscriptions;

public:
    Server() = default;
    virtual ~Server() = default;

    virtual bool subscribe(uint64_t channel, const BaseCallable& callable);
    virtual bool unsubscribe(uint64_t channel, uint64_t callable_id);

    virtual int publish(uint64_t channel, const Data& data);
};

};


#endif //KOI_PUB_SUB_SERVER_H
