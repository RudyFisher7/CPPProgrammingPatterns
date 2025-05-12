#include "koi_pub_sub/server.hpp"
#include "koi_pub_sub/callable.hpp"
#include "koi_pub_sub/models/data.hpp"
#include "koi_pub_sub/serialization/serialization.hpp"

#include "mock_object.hpp"

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>

#include <array>
#include <iostream>

#if (WIN32 || WIN64)

#endif


TEST_CASE("Endianness Check", "[Serialization]") {
    std::cout << KoiPubSub::Serialization::is_little_endian() << std::endl;
}


TEST_CASE("primitive_to_network_bytes()", "[Serialization]") {
    uint64_t value64 = 100u;

    std::array<uint8_t, sizeof(uint64_t)> arr {};
    uint8_t* result = KoiPubSub::Serialization::primitive_to_network_bytes<uint64_t>(value64, arr.data());

    CHECK(result == arr.data() + sizeof(uint64_t));

    uint64_t new_value64 = KoiPubSub::Serialization::network_bytes_to_primitive<uint64_t>(arr.data());

    CHECK(value64 == new_value64);


    for (auto& value: arr) {
        std::cout << (int)value << std::endl;
    }
}


TEST_CASE("Callable", "[Callable]") {
    MockObject obj;
    KoiPubSub::Callable callable(obj, &MockObject::on_published);

    MockData data;
    data.integer = 8;
    callable.callable(data);
}


int main(int argc, char* argv[]) {
    // your setup ...

    int result = Catch::Session().run( argc, argv );

    // your clean-up...

    return result;
}