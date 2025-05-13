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

    size_t number_of_bytes = KoiPubSub::Serialization::get_number_of_bytes(value64, new_value64, uint8_t(9), char('A'));
    std::cout << "Number of bytes: " << (int)number_of_bytes << std::endl;

    std::vector<uint8_t> buffer;
    KoiPubSub::Serialization::to_network_bytes(buffer, value64, new_value64, uint8_t(255), char('A'));

    std::cout << "Buffer: " << std::endl;
    for (auto& value: buffer) {
        std::cout << (int)value << std::endl;
    }

    std::tuple<uint64_t, uint64_t, uint8_t, char> t = KoiPubSub::Serialization::from_network_bytes<uint64_t, uint64_t, uint8_t, char>(buffer.data());

    uint64_t a = 0;
    uint64_t b = 0;
    uint8_t c = 0;
    char d = 'B';

    KoiPubSub::Serialization::from_network_bytes(buffer.data(), a, b, c, d);

    int v = 0;
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