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

    TestIntNTree* BeginRoot() override {
        _begin_root();
        return this;
    }

    TestIntNTree* EndRoot() override {
        _end_root();
        return this;
    }

    TestIntNTree* Begin() override {
        _begin();
        return this;
    }

    TestIntNTree* End() override {
        _end();
        return this;
    }

    TestIntNTree* SetData(int value) override {
        _current_parent->data = value;
        return this;
    }
};


TEST_CASE("NTree 1 Child", "[NTree]") {
    int v0 = 1;
    int v1 = 2;
    int v2 = 3;

    TestIntNTree tree;

    tree.BeginRoot()->SetData(v0)
        ->Begin()->SetData(v1)
            ->Begin()->SetData(v2)
            ->End()
        ->End()
    ->EndRoot();

    AutoRayGui::Node<int>* root = tree.Root();
    REQUIRE(root != nullptr);

    AutoRayGui::Node<int>* child0 = root->first_child;
    REQUIRE(child0 != nullptr);

    AutoRayGui::Node<int>* child1 = child0->first_child;
    REQUIRE(child1 != nullptr);

    // check relationships
    CHECK(root->first_child == root->last_child);
    CHECK(root->left_sibling == nullptr);
    CHECK(root->right_sibling == nullptr);
    CHECK(child0->first_child == child0->last_child);
    CHECK(child0->parent == root);
    CHECK(child0->left_sibling == nullptr);
    CHECK(child0->right_sibling == nullptr);
    CHECK(child1->first_child == child1->last_child);
    CHECK(child1->first_child == nullptr);
    CHECK(child1->parent == child0);
    CHECK(child1->left_sibling == nullptr);
    CHECK(child1->right_sibling == nullptr);

    // check data
    CHECK(root->data == v0);
    CHECK(root->first_child->data == v1);
    CHECK(root->last_child->data == v1);
    CHECK(child0->first_child->data == v2);
    CHECK(child0->last_child->data == v2);
}


TEST_CASE("NTree 2-4 Children", "[NTree]") {
    int v0 = 1;
    int v1 = 2;
    int v2 = 3;
    int v3 = 4;
    int v4 = 5;
    int v5 = 6;
    int v6 = 7;
    int v7 = 8;
    int v8 = 9;
    int v9 = 10;

    TestIntNTree tree;

    /* Build the Tree */
    {
        tree.BeginRoot()->SetData(v0)
            ->Begin()->SetData(v1)
                ->Begin()->SetData(v2)
                ->End()
                ->Begin()->SetData(v3)
                ->End()
                ->Begin()->SetData(v4)
                ->End()
            ->End()
            ->Begin()->SetData(v5)
                ->Begin()->SetData(v6)
                ->End()
                ->Begin()->SetData(v7)
                ->End()
                ->Begin()->SetData(v8)
                ->End()
                ->Begin()->SetData(v9)
                ->End()
            ->End()
        ->EndRoot();
    }

    /* Layer 1 */
    {
        AutoRayGui::Node<int> *root = tree.Root();
        REQUIRE(root != nullptr);

        AutoRayGui::Node<int> *child0 = root->first_child;
        REQUIRE(child0 != nullptr);

        AutoRayGui::Node<int> *child1 = root->last_child;
        REQUIRE(child1 != nullptr);

        // check relationships
        {
            CHECK(root->first_child != root->last_child);
            CHECK(root->left_sibling == nullptr);
            CHECK(root->right_sibling == nullptr);
            CHECK(child0->first_child != child0->last_child);
            CHECK(child0->parent == root);
            CHECK(child0->left_sibling == nullptr);
            CHECK(child0->right_sibling == child1);
            CHECK(child0->first_child != child0->last_child);
            CHECK(child0->first_child != nullptr);
            CHECK(child1->parent == root);
            CHECK(child1->left_sibling == child0);
            CHECK(child1->right_sibling == nullptr);
            CHECK(child1->first_child != child1->last_child);
            CHECK(child1->first_child != nullptr);
            CHECK(child0 == child1->left_sibling);
            CHECK(child1 == child0->right_sibling);
        }

        // check data
        CHECK(root->data == v0);
        CHECK(root->first_child->data == v1);
        CHECK(root->last_child->data == v5);
    }

    /* Layer 2: First Child */
    {
        AutoRayGui::Node<int> *parent = tree.Root()->first_child;
        REQUIRE(parent != nullptr);

        AutoRayGui::Node<int> *child0 = parent->first_child;
        REQUIRE(child0 != nullptr);

        AutoRayGui::Node<int> *child1 = child0->right_sibling;
        REQUIRE(child1 != nullptr);

        AutoRayGui::Node<int> *child2 = child1->right_sibling;
        REQUIRE(child2 != nullptr);

        // check relationships
        {
            CHECK(parent == child0->parent);
            CHECK(parent == child1->parent);
            CHECK(parent == child2->parent);

            CHECK(child0 != child1);
            CHECK(child0 != child2);
            CHECK(child1 != child0);
            CHECK(child1 != child2);
            CHECK(child2 != child0);
            CHECK(child2 != child1);

            CHECK(child0->left_sibling == nullptr);
            CHECK(child0->right_sibling == child1);
            CHECK(child0->first_child == nullptr);
            CHECK(child0->last_child == nullptr);
            CHECK(child1->left_sibling == child0);
            CHECK(child1->right_sibling == child2);
            CHECK(child1->first_child == nullptr);
            CHECK(child1->last_child == nullptr);
            CHECK(child2->left_sibling == child1);
            CHECK(child2->right_sibling == nullptr);
            CHECK(child2->first_child == nullptr);
            CHECK(child2->last_child == nullptr);
        }

        // check data
        CHECK(parent->data == v1);
        CHECK(child0->data == v2);
        CHECK(child1->data == v3);
        CHECK(child2->data == v4);
    }

    /* Layer 2: Last Child */
    {
        AutoRayGui::Node<int> *parent = tree.Root()->last_child;
        REQUIRE(parent != nullptr);

        AutoRayGui::Node<int> *child0 = parent->first_child;
        REQUIRE(child0 != nullptr);

        AutoRayGui::Node<int> *child1 = child0->right_sibling;
        REQUIRE(child1 != nullptr);

        AutoRayGui::Node<int> *child2 = child1->right_sibling;
        REQUIRE(child2 != nullptr);

        AutoRayGui::Node<int> *child3 = child2->right_sibling;
        REQUIRE(child3 != nullptr);

        // check relationships
        {
            CHECK(parent == child0->parent);
            CHECK(parent == child1->parent);
            CHECK(parent == child2->parent);
            CHECK(parent == child3->parent);

            CHECK(child0 != child1);
            CHECK(child0 != child2);
            CHECK(child0 != child3);
            CHECK(child1 != child0);
            CHECK(child1 != child2);
            CHECK(child1 != child3);
            CHECK(child2 != child0);
            CHECK(child2 != child1);
            CHECK(child2 != child3);

            CHECK(child0->left_sibling == nullptr);
            CHECK(child0->right_sibling == child1);
            CHECK(child0->first_child == nullptr);
            CHECK(child0->last_child == nullptr);
            CHECK(child1->left_sibling == child0);
            CHECK(child1->right_sibling == child2);
            CHECK(child1->first_child == nullptr);
            CHECK(child1->last_child == nullptr);
            CHECK(child2->left_sibling == child1);
            CHECK(child2->right_sibling == child3);
            CHECK(child2->first_child == nullptr);
            CHECK(child2->last_child == nullptr);
            CHECK(child3->left_sibling == child2);
            CHECK(child3->right_sibling == nullptr);
            CHECK(child3->first_child == nullptr);
            CHECK(child3->last_child == nullptr);
        }

        // check data
        CHECK(parent->data == v5);
        CHECK(child0->data == v6);
        CHECK(child1->data == v7);
        CHECK(child2->data == v8);
        CHECK(child3->data == v9);
    }
}


int main(int argc, char* argv[]) {
    // your setup ...

    int result = Catch::Session().run(argc, argv);

    // your clean-up...

    return result;
}