#ifndef KOI_PUB_SUB_DATA_H
#define KOI_PUB_SUB_DATA_H


#include <cstdint>
#include <vector>


namespace KoiPubSub {

class Data {
public:
    Data() = default;
    virtual ~Data() = default;

    virtual void to_network_bytes(std::vector<uint8_t>& out_bytes) = 0;
    virtual bool from_network_bytes(const std::vector<uint8_t>& in_bytes) = 0;
};

}


#endif //KOI_PUB_SUB_DATA_H
