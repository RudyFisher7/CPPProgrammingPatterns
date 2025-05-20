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


#include "autoraygui/n_tree.hpp"

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>


class TestIntNTree final : public AutoRayGui::NTree<int, 32u, AutoRayGui::INDEXING_MODE_SAFE> {
public:
    TestIntNTree() = default;
    ~TestIntNTree() override = default;

    TestIntNTree* Begin() override {
        _begin();
        return this;
    }

    TestIntNTree* End() override {
        _end();
        return this;
    }

    TestIntNTree* SetData(int value) {
        _current_parent->data = value;
        return this;
    }
};


TEST_CASE("NTree Traversal", "[NTree]") {
    TestIntNTree tree;

    tree.Begin()->SetData(1)
        ->Begin()->SetData(2)
            ->Begin()->SetData(3)
            ->End()
        ->End()
    ->End();

    CHECK(true);
}


int main(int argc, char* argv[]) {
    // your setup ...

    int result = Catch::Session().run(argc, argv);

    // your clean-up...

    return result;
}