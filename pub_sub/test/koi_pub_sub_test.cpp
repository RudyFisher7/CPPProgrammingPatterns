#include "koi_pub_sub/server.hpp"
#include "koi_pub_sub/models/callable.hpp"
#include "koi_pub_sub/models/data.hpp"

#include "mock_object.hpp"

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>


TEST_CASE("Callable", "[Callable]") {
    MockObject obj;
    KoiPubSub::Callable<MockObject> callable(obj, &MockObject::on_published);

    MockData data;
    data.integer = 8;
    callable(data);
}


int main(int argc, char* argv[]) {
    // your setup ...

    int result = Catch::Session().run( argc, argv );

    // your clean-up...

    return result;
}