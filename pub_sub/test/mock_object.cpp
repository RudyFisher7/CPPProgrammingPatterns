#include "mock_object.hpp"

#include <iostream>


void MockObject::on_published(const Data &data) {
    const auto* mock = dynamic_cast<const MockData*>(&data);
    if (mock) {
        std::cout << mock->integer << std::endl;
    }
}
