#include "koi_pub_sub/server.hpp"
#include "koi_pub_sub/callable.hpp"
#include "koi_pub_sub/models/data.hpp"
#include "koi_pub_sub/serialization/serialization.hpp"

#include "mock_object.hpp"

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>

#include <array>
#include <iostream>


TEST_CASE("Serialize primitives", "[Serialization]") {
    uint64_t value64 = 100u;

    std::array<uint8_t, sizeof(uint64_t)> arr {};
    uint8_t* result = KoiPubSub::Serialization::primitive_to_network_bytes<uint64_t>(value64, arr.data());

    CHECK(result == arr.data() + sizeof(uint64_t));

    auto new_value64 = KoiPubSub::Serialization::network_bytes_to_primitive<uint64_t>(arr.data());

    CHECK(value64 == new_value64);
}


TEST_CASE("Get number of bytes to serialize", "[Serialization]") {
    size_t number_of_bytes = KoiPubSub::Serialization::get_number_of_bytes(uint64_t(9), uint64_t(9), uint8_t(9), char('A'));

    CHECK(number_of_bytes == 18u);
}


TEST_CASE("Data serialization", "[Data]") {
    MockData data;
    data.big_integer = 100l;
    data.boolean = false;

    MockData data2;

    std::vector<uint8_t> bytes;
    data.to_network_bytes(bytes);

    data2.from_network_bytes(bytes);

    CHECK(data == data2);
}


TEST_CASE("Callable", "[Callable]") {
    MockObject obj;
    KoiPubSub::Callable callable(obj, &MockObject::on_published);

    MockData data;
    data.integer = 8;
    callable.callable(data);
}


TEST_CASE("Server", "[Server]") {
    KoiPubSub::Server server;
    MockObject obj;
    KoiPubSub::Callable callable(obj, &MockObject::on_published);
    MockData data;

    data.integer = 8;
    data.big_float = 80.0;

    REQUIRE(server.subscribe(0u, callable));
    REQUIRE((server.publish(0u, data) == 0));

    CHECK(obj.data == data);
}


int main(int argc, char* argv[]) {
    // Just check the endianness of the system and display it.
    std::cout << "This system is little endian: " << (KoiPubSub::Serialization::is_little_endian() ? "true" : "false") << std::endl;
    // your setup ...

    int result = Catch::Session().run(argc, argv);

    // your clean-up...

    return result;
}