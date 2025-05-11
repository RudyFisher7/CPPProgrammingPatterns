#include "koi_pub_sub/server.hpp"

bool KoiPubSub::Server::subscribe(uint64_t channel, const KoiPubSub::Callable &callable) {
    bool result = false;

    std::map<uint64_t, Callable> map;

    auto it = subscriptions.find(channel);
    if (it == subscriptions.end()) {
        subscriptions.emplace(channel, map);
    } else {
        map = it->second;
    }

    if (map.find(callable.id) != map.end()) {
        map.emplace(callable.id, callable);
        result = true;
    }

    return result;
}

bool KoiPubSub::Server::unsubscribe(uint64_t channel, uint64_t callable_id) {
    bool result = false;

    auto it = subscriptions.find(channel);
    if (it != subscriptions.end()) {
        std::map<uint64_t, Callable> map = it->second;

        if (map.find(callable_id) != map.end()) {
            map.erase(callable_id);
            result = true;
        }
    }

    return result;
}

int KoiPubSub::Server::publish(uint64_t channel, const KoiPubSub::Data &data) {
    int result = 0;

    auto it = subscriptions.find(channel);
    if (it != subscriptions.end()) {
        std::map<uint64_t, Callable> map = it->second;

        for (const auto& pair : map) {
            pair.second.callable(data);
        }
    }

    return result;
}
