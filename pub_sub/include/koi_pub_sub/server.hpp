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


#ifndef KOI_PUB_SUB_SERVER_HPP
#define KOI_PUB_SUB_SERVER_HPP

#include "koi_pub_sub/callable.hpp"
#include "koi_pub_sub/models/data.hpp"

#include <cstdint>
#include <functional>
#include <map>


namespace KoiPubSub {

class Server {
protected:
    // std::map<channel, std::map<callable_id, function>>
    std::map<uint64_t, std::map<uint64_t, Callable>> subscriptions;

public:
    Server() = default;
    virtual ~Server() = default;

    virtual bool subscribe(uint64_t channel, const Callable& callable);
    virtual bool unsubscribe(uint64_t channel, uint64_t callable_id);

    virtual int publish(uint64_t channel, const Data& data);
};

};


#endif //KOI_PUB_SUB_SERVER_HPP
