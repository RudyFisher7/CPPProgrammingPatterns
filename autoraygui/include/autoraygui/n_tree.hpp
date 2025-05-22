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


#ifndef AUTO_RAY_GUI_N_TREE_HPP
#define AUTO_RAY_GUI_N_TREE_HPP


#include "autoraygui/enums.hpp"
#include "autoraygui/node.hpp"

#include <array>
#include <cstdint>
#include <cmath>
#include <functional>
#include <type_traits>
#include <tuple>
#include <utility>


namespace AutoRayGui {

template<typename TNodeData, size_t size, IndexingMode indexing_mode>
class NTree {
protected:
    typedef NTree<TNodeData, size, indexing_mode> TThis;
    typedef Node<TNodeData> TThisNode;

    std::array<TThisNode, size> _arena {};

    size_t _arena_size;
    size_t _current_index;

    TThisNode* _current_parent;

public:
    NTree() : _arena_size(0u), _current_index(0u), _current_parent(nullptr) {}
    virtual ~NTree() = default;

    virtual TThis* BeginRoot() {
        _get(0u) = {
                {},
                nullptr,
                nullptr,
                nullptr,
                nullptr,
                nullptr,
        };

        _begin_root();
        return this;
    }

    virtual TThis* EndRoot() {
        _end_root();
        return this;
    }

    virtual TThis* Begin() {
        _get(_current_index) = {
                {},
                _current_parent,
                nullptr,
                nullptr,
                nullptr,
                nullptr,
        };

        _begin();
        return this;
    }

    virtual TThis* End() {
        _end();
        return this;
    }

    virtual TThis* SetData(TNodeData value) {
        _current_parent->data = value;
        return this;
    }

    virtual TThisNode* Root() {
        return &_get(0u);
    }

protected:
    virtual void _begin_root() {
        _current_parent = &_get(0u);
        _current_index = 1u;
    }

    virtual void _end_root() {
        _current_parent = nullptr;
        _arena_size = _current_index;
        _current_index = 0u;
    }

    virtual void _begin() {
        TThisNode* current = &_get(_current_index);

        // if there is a current parent
        if (_current_parent) {

            // if the parent has no children, set its first one to the current node
            if (!_current_parent->first_child) {
                _current_parent->first_child = current;
            }

            // if the parent has >= 1 children
            if (_current_parent->last_child) {
                // set the parent's last child's right sibling to the current node
                _current_parent->last_child->right_sibling = current;
            }

            // set the current node's left sibling to the parent's last child
            current->left_sibling = _current_parent->last_child;

            // set the parent's last child to the current node
            _current_parent->last_child = current;
        }

        // set the current parent to the current node for the next Begin() call
        _current_parent = current;

        ++_current_index;
    }

    virtual void _end() {
        _current_parent = _current_parent->parent;
    }

    template<IndexingMode in_indexing_mode = indexing_mode>
    inline typename std::enable_if<in_indexing_mode == INDEXING_MODE_SAFE, TThisNode&>::type
    _get(size_t index) {
        return _arena.at(index);
    }

    template<IndexingMode in_indexing_mode = indexing_mode>
    inline typename std::enable_if<in_indexing_mode == INDEXING_MODE_UNCHECKED, TThisNode&>::type
    _get(size_t index) {
        return _arena[index];
    }

    virtual TThisNode* _get_first_leaf(TThisNode* from) {
        TThisNode* current = from;
        while (current->first_child) {
            current = current->first_child;
        }

        return current;
    }

    virtual TThisNode* _get_last_sibling(TThisNode* from) {
        if (from->parent) {
            return from->parent->last_child;
        }

        return nullptr;
    }

    virtual TThisNode* _get_first_right_leaf(TThisNode* from_leaf) {
        TThisNode* result = from_leaf;

        if (from_leaf->right_sibling) {
            result = from_leaf->right_sibling;
            while (result->first_child) {
                result = result->first_child;
            }
        } else {
            TThisNode* parent_with_right_sibling = from_leaf->parent;
            while (parent_with_right_sibling && !parent_with_right_sibling->right_sibling) {
                parent_with_right_sibling = parent_with_right_sibling->parent;
            }

            if (parent_with_right_sibling) {
                TThisNode* current = parent_with_right_sibling->right_sibling;
                while (current->first_child) {
                    current = current->first_child;
                }

                result = current;
            }
        }

        return result;
    }

    virtual TThisNode* _get_left_most_parent(TThisNode* from) {
        TThisNode* result = from->parent;

        if (result && result->parent) {
            result = result->parent->first_child;
        }

        return result;
    }
};


}


#endif //AUTO_RAY_GUI_N_TREE_HPP
