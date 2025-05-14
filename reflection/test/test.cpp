
#include "koi_object/string_name.hpp"
#include "koi_object/variant_reference.hpp"


#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>

#include <array>


TEST_CASE("StringName", "[StringName]") {
    SECTION("Equality") {
        Koi::StringName name1{"name"};
        Koi::StringName name2{"name"};
        Koi::StringName name3{"nale"};
        Koi::StringName name4{"namee"};

        CHECK(name1 == name2);
        CHECK_FALSE(name1 == name3);
        CHECK_FALSE(name1 == name4);
        CHECK_FALSE(name2 == name3);

        CHECK(name1.get() == name2.get());
        CHECK_FALSE(name1.get() == name3.get());
        CHECK_FALSE(name1.get() == name4.get());
        CHECK_FALSE(name2.get() == name3.get());
    }

    SECTION("Inequality") {
        Koi::StringName name1{"name"};
        Koi::StringName name2{"name"};
        Koi::StringName name3{"nale"};
        Koi::StringName name4{"namee"};

        CHECK_FALSE(name1 != name2);
        CHECK(name1 != name3);
        CHECK(name1 != name4);
        CHECK(name2 != name3);

        CHECK_FALSE(name1.get() != name2.get());
        CHECK(name1.get() != name3.get());
        CHECK(name1.get() != name4.get());
        CHECK(name2.get() != name3.get());
    }
}


TEST_CASE("VarRef", "[VarRef]") {
    SECTION("Equality", "[int]") {
        int one = 11;
        int* one_raw = &one;
        Koi::VarRef one_ref(one);
        Koi::VarRef one_ptr_ref(&one);

        CHECK(one == one_ref.get<int>());
        CHECK(one == one_ref.get<int>());
        CHECK(one == one_ptr_ref.get<int>());
        CHECK(one_raw == one_ref.get<int*>());
        CHECK(one_raw == one_ptr_ref.get<int*>());
        CHECK(one == one_ptr_ref.get<int>());
    }

    SECTION("Mutation", "[int]") {
        int two = 90;
        int one = 11;
        int* one_raw = &one;
        Koi::VarRef one_ref(one);
        Koi::VarRef one_ptr_ref(&one);

        one = two;

        CHECK(two == one_ref.get<int>());
        CHECK(two == one_ref.get<int>());
        CHECK(two == one_ptr_ref.get<int>());
        CHECK(one_raw == one_ref.get<int*>());
        CHECK(one_raw == one_ptr_ref.get<int*>());
        CHECK(two == one_ptr_ref.get<int>());

        bool one_ref_set_result = one_ref.set(two);

        REQUIRE(one_ref_set_result);
        CHECK(two == one_ref.get<int>());
        CHECK(two == one_ref.get<int>());
        CHECK(two == one_ptr_ref.get<int>());
        CHECK(one_raw == one_ref.get<int*>());
        CHECK(one_raw == one_ptr_ref.get<int*>());
        CHECK(two == one_ptr_ref.get<int>());

        one_ref_set_result = one_ref.set(9.0f);

        REQUIRE_FALSE(one_ref_set_result);
        CHECK(two == one_ref.get<int>());
        CHECK(two == one_ref.get<int>());
        CHECK(two == one_ptr_ref.get<int>());
        CHECK(one_raw == one_ref.get<int*>());
        CHECK(one_raw == one_ptr_ref.get<int*>());
        CHECK(two == one_ptr_ref.get<int>());
    }

    SECTION("Equality", "[double]") {
        double one = 11;
        double* one_raw = &one;
        Koi::VarRef one_ref(one);
        Koi::VarRef one_ptr_ref(&one);

        CHECK(one == one_ref.get<double>());
        CHECK(one == one_ref.get<double>());
        CHECK(one == one_ptr_ref.get<double>());
        CHECK(one_raw == one_ref.get<double*>());
        CHECK(one_raw == one_ptr_ref.get<double*>());
        CHECK(one == one_ptr_ref.get<double>());
    }

    SECTION("Mutation", "[double]") {
        double two = 90;
        double one = 11;
        double* one_raw = &one;
        Koi::VarRef one_ref(one);
        Koi::VarRef one_ptr_ref(&one);

        one = two;

        CHECK(two == one_ref.get<double>());
        CHECK(two == one_ref.get<double>());
        CHECK(two == one_ptr_ref.get<double>());
        CHECK(one_raw == one_ref.get<double*>());
        CHECK(one_raw == one_ptr_ref.get<double*>());
        CHECK(two == one_ptr_ref.get<double>());

        bool one_ref_set_result = one_ref.set(two);

        REQUIRE(one_ref_set_result);
        CHECK(two == one_ref.get<double>());
        CHECK(two == one_ref.get<double>());
        CHECK(two == one_ptr_ref.get<double>());
        CHECK(one_raw == one_ref.get<double*>());
        CHECK(one_raw == one_ptr_ref.get<double*>());
        CHECK(two == one_ptr_ref.get<double>());

        one_ref_set_result = one_ref.set(9.0f);

        REQUIRE_FALSE(one_ref_set_result);
        CHECK(two == one_ref.get<double>());
        CHECK(two == one_ref.get<double>());
        CHECK(two == one_ptr_ref.get<double>());
        CHECK(one_raw == one_ref.get<double*>());
        CHECK(one_raw == one_ptr_ref.get<double*>());
        CHECK(two == one_ptr_ref.get<double>());
    }
}


int main(int argc, char* argv[]) {
    // your setup ...

    int result = Catch::Session().run(argc, argv);

    // your clean-up...

    return result;
}