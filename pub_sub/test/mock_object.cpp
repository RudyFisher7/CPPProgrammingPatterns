#include "mock_object.hpp"


void MockObject::on_published(const Data &data) {
    const auto* mock = dynamic_cast<const MockData*>(&data);
    if (mock) {
        //do stuff
    }
}
