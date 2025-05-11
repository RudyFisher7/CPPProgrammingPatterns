#ifndef KOI_PUB_SUB_TEST_MOCK_OBJECT_H
#define KOI_PUB_SUB_TEST_MOCK_OBJECT_H


#include "koi_pub_sub/models/callable.hpp"
#include "koi_pub_sub/models/data.hpp"


using namespace KoiPubSub;

class MockData : public Data {
public:
    int integer = 0;
};


class MockObject {
public:
    void on_published(const Data& data);
};


#endif //KOI_PUB_SUB_TEST_MOCK_OBJECT_H
