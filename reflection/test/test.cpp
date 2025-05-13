

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>


TEST_CASE("ToDo", "[ToDo]") {
    CHECK(true);//todo::
}


int main(int argc, char* argv[]) {
    // your setup ...

    int result = Catch::Session().run(argc, argv);

    // your clean-up...

    return result;
}