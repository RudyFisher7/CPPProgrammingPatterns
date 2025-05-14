/*
 * MIT License
 *
 * Copyright (c) 2025 Rudy Fisher (kiyasui-hito)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


#include "koi_pub_sub/server.hpp"

bool KoiPubSub::Server::subscribe(uint64_t channel, const KoiPubSub::Callable &callable) {
    bool result = false;


    auto it = subscriptions.find(channel);
    if (it == subscriptions.end()) {
        subscriptions.emplace(channel, std::map<uint64_t, Callable>());
        it = subscriptions.find(channel);
    }

    std::map<uint64_t, Callable>& map = it->second;
    if (map.find(callable.id) == map.end()) {
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
