/*
 * MIT License
 *
 * Copyright (c) 2025 Rudy Fisher (kiyasui-hito)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


#include "koi_object/string_name.hpp"
#include "koi_object/string_names.hpp"
#include "koi_object/variant_reference.hpp"
#include "koi_object/object.hpp"
#include "mock_object.hpp"


#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>

#include <array>
#include <cmath>
#include <limits>
#include <iostream>


bool are_equal_approx(double a, double b) {
    return std::fabs(a - b) < std::numeric_limits<double>::epsilon();
}

bool are_equal_approx(float a, float b) {
    return std::fabs(a - b) < std::numeric_limits<float>::epsilon();
}


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

        CHECK(name1.get_string() == name2.get_string());
        CHECK_FALSE(name1.get_string() == name3.get_string());
        CHECK_FALSE(name1.get_string() == name4.get_string());
        CHECK_FALSE(name2.get_string() == name3.get_string());
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

        CHECK_FALSE(name1.get_string() != name2.get_string());
        CHECK(name1.get_string() != name3.get_string());
        CHECK(name1.get_string() != name4.get_string());
        CHECK(name2.get_string() != name3.get_string());
    }
}


TEST_CASE("VarRef", "[VarRef]") {
    SECTION("Type", "[int]") {
        int one = 11;
        int two = 8;
        Koi::VarRef one_ref(one);

        CHECK(typeid(two) == one_ref.get_type());

        // Usage example: here is where you would draw the appropriate control to edit the property in a GUI based on
        // the type.
        int three = 0;
        if (typeid(int) == one_ref.get_type()) {
            three = 1;
        } else if (typeid(float) == one_ref.get_type()) {
            three = 2;
        } else if (typeid(char) == one_ref.get_type()) {
            three = 3;
        }

        CHECK(three == 1);
    }

    SECTION("Equality", "[int]") {
        int one = 11;
        int* one_raw = &one;
        Koi::VarRef one_ref(one);

        CHECK(one == one_ref.get<int>().first);
        CHECK_FALSE(one == one_ref.get<float>().first);
        CHECK(*one_raw == one_ref.get<int>().first);
        CHECK(0 == one_ref.get<short>().first);
        CHECK(0u == one_ref.get<unsigned int>().first);
        CHECK(are_equal_approx(0.0f, one_ref.get<float>().first));
        CHECK(are_equal_approx(0.0, one_ref.get<double>().first));
        CHECK('\0' == one_ref.get<char>().first);
        CHECK(nullptr == one_ref.get<MockObject*>().first);
        CHECK_FALSE(one_ref.get<short>().second);
        CHECK_FALSE(one_ref.get<unsigned int>().second);
        CHECK_FALSE(one_ref.get<float>().second);
        CHECK_FALSE(one_ref.get<double>().second);
        CHECK_FALSE(one_ref.get<MockObject*>().second);
    }

    SECTION("Mutation", "[int]") {
        int two = 90;
        int one = 11;
        int* one_raw = &one;
        Koi::VarRef one_ref(one);

        one = two;

        CHECK(two == one_ref.get<int>().first);

        bool one_ref_set_result = one_ref.set(two);

        REQUIRE(one_ref_set_result);
        CHECK(two == one_ref.get<int>().first);

        one_ref_set_result = one_ref.set(9.0f);

        REQUIRE_FALSE(one_ref_set_result);
        CHECK(two == one_ref.get<int>().first);
    }

    SECTION("Equality", "[double]") {
        double one = 11;
        double* one_raw = &one;
        Koi::VarRef one_ref(one);

        CHECK(one == one_ref.get<double>().first);
    }

    SECTION("Mutation", "[double]") {
        double two = 90;
        double one = 11;
        double* one_raw = &one;
        Koi::VarRef one_ref(one);

        one = two;

        CHECK(two == one_ref.get<double>().first);

        bool one_ref_set_result = one_ref.set(two);

        REQUIRE(one_ref_set_result);
        CHECK(two == one_ref.get<double>().first);

        one_ref_set_result = one_ref.set(9.0f);

        REQUIRE_FALSE(one_ref_set_result);
        CHECK(two == one_ref.get<double>().first);
    }

    SECTION("Equality", "[std::array]") {
        std::array<float, 4> one {1.1f, 2.2f, 3.3f, 4.4f};
        std::array<float, 4>* one_raw = &one;
        Koi::VarRef one_ptr_ref(&one);

        CHECK(one == *one_ptr_ref.get<std::array<float, 4>*>().first);
        CHECK(one_raw == one_ptr_ref.get<std::array<float, 4>*>().first);
    }
}


TEST_CASE("Object", "[Object]") {
    SECTION("Creat new") {
        Koi::Object* one = Koi::Object::create_new("MockObject");
        auto* one_mock = dynamic_cast<MockObject*>(one);

        CHECK(one_mock != nullptr);
        CHECK(one == one_mock);

        one_mock = nullptr;

        delete one;
    }

    SECTION("Creat new nullptr") {
        Koi::Object* one = Koi::Object::create_new("NonExistentObject");
        auto* one_mock = dynamic_cast<MockObject*>(one);

        CHECK(one_mock == nullptr);
        CHECK(one == nullptr);

        delete one;
    }

    SECTION("Class names and type infos") {
        Koi::Object one;
        MockObject two;
        MockObject three;

        CHECK_FALSE(one.get_class_name() == two.get_class_name());
        CHECK_FALSE(Koi::Object::get_class_type_info(one.get_class_name()) == Koi::Object::get_class_type_info(two.get_class_name()));

        CHECK(two.get_class_name() == three.get_class_name());
        CHECK(Koi::Object::get_class_type_info(two.get_class_name()) == Koi::Object::get_class_type_info(three.get_class_name()));
    }

    SECTION("Has property") {
        MockObject one;

        CHECK(one.has_property("pint"));
        CHECK(one.has_property("pfloat"));
        CHECK(one.has_property("pbool"));
        CHECK_FALSE(one.has_property("lint"));
    }

    SECTION("Get property") {
        MockObject one;

        int pint = one.get<int>("pint").first;

        CHECK(pint == one.pint);

        std::pair<int, bool> pfloat = one.get<int>("pfloat");

        CHECK_FALSE(pfloat.second);
        CHECK(pfloat.first == 0);
    }

    SECTION("Set property") {
        MockObject one;

        int pint = 80;

        REQUIRE(one.set("pint", pint));
        CHECK(pint == one.pint);
        CHECK(pint == one.get<int>("pint").first);

        float pfloat = 99.8f;

        REQUIRE_FALSE(one.set("pint", pfloat));
        CHECK_FALSE(are_equal_approx(pfloat, one.get<float>("pfloat").first));
        CHECK_FALSE(are_equal_approx(pfloat, one.pfloat));
    }

    SECTION("Get property list") {
        MockObject one;

        const std::unordered_map<const Koi::StringName, Koi::VarRef, Koi::StringNameHash> list = one.get_property_list();

        // Usage example: use this list in your GUI to edit the object's properties.
        // Associate the StringNames with the values that should be set and use the set<T>().
        for (const auto& it: list) {
            if (typeid(int) == it.second.get_type()) {
                one.set<int>(it.first, 8);
            } else if (typeid(float) == it.second.get_type()) {
                one.set<float>(it.first, 8.88f);
            } else if (typeid(char) == it.second.get_type()) {
                one.set<char>(it.first, 'V');
            }
        }

        CHECK(one.pint == list.find("pint")->second.get<int>().first);
        CHECK(one.pbool == list.find("pbool")->second.get<bool>().first);
        CHECK(are_equal_approx(one.pfloat, list.find("pfloat")->second.get<float>().first));
        CHECK(list.size() == 3u);
    }
}


int main(int argc, char* argv[]) {
    // your setup ...

    // Register classes and factory methods at the beginning of the application.
    Koi::Object::register_object_class<Koi::Object>(Koi::StringNames::get_singleton().OBJECT);
    Koi::Object::register_factory_method(Koi::StringNames::get_singleton().OBJECT, []() -> Koi::Object* { return new Koi::Object(); });
    Koi::Object::register_object_class<MockObject>("MockObject");
    Koi::Object::register_factory_method("MockObject", []() -> Koi::Object* { return new MockObject(); });

    MockObject mockObject;
    mockObject.pint = 90;
    std::cout << mockObject.to_json_string() << std::endl;

    int result = Catch::Session().run(argc, argv);

    // your clean-up...

    return result;
}