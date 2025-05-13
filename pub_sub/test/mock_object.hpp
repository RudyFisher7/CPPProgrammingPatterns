#ifndef KOI_PUB_SUB_TEST_MOCK_OBJECT_H
#define KOI_PUB_SUB_TEST_MOCK_OBJECT_H


#include <ostream>
#include "koi_pub_sub/callable.hpp"
#include "koi_pub_sub/models/data.hpp"


using namespace KoiPubSub;

/**
 * A mock data object for unit testing purposes.
 *
 * @warning Not all operators and constructors may be implemented. Implement them
 * as needed for unit testing the core features.
 */
class MockData : public Data {
public:
    int integer = 0;
    char character = 'A';
    bool boolean = true;
    float floating_point_number = 2.0f;
    double big_float = 3.0;
    unsigned int unsigned_integer = 9u;
    long big_integer = 122l;

    MockData& operator=(const MockData& rhs);

    void to_network_bytes(std::vector<uint8_t>& out_bytes) override;

    bool from_network_bytes(const std::vector<uint8_t>& in_bytes) override;

    bool operator==(const MockData &rhs) const;

    bool operator!=(const MockData &rhs) const;

    friend std::ostream &operator<<(std::ostream &os, const MockData &data);
};


/**
 * A mock object for unit testing purposes.
 *
 * @warning Not all operators and constructors may be implemented. Implement them
 * as needed for unit testing the core features.
 */
class MockObject {
public:
    MockData data;
    void on_published(const Data& data);
};


#endif //KOI_PUB_SUB_TEST_MOCK_OBJECT_H
