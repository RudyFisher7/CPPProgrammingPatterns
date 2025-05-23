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


#ifndef AUTO_RAY_GUI_GUI_GRAPH_HPP
#define AUTO_RAY_GUI_GUI_GRAPH_HPP


#include "autoraygui/n_tree.hpp"

#include "autoraygui/enums.hpp"
#include "autoraygui/node.hpp"
#include "autoraygui/node_data.hpp"
#include "autoraygui/layout.hpp"
#include "autoraygui/control.hpp"
#include "std_extensions/extensions.hpp"

#include <raylib.h>
#include <raymath.h>

#include <array>
#include <cfloat>
#include <cstdint>
#include <cmath>
#include <functional>
#include <type_traits>
#include <tuple>
#include <utility>


namespace AutoRayGui {

typedef Node<NodeData> GuiNode;

template<size_t size, IndexingMode indexing_mode>
class GuiTree : public NTree<NodeData, size, indexing_mode> {
private:
    typedef GuiTree<size, indexing_mode> TGuiThis;

protected:
    NodeData _default_node_data;

public:
    GuiTree() :
        _default_node_data {
        {
                {0.0f, 0.0f, 0.0f, 0.0f},
                {0.0f, 0.0f, 0.0f, 0.0f},
                {0.0f, 0.0f, 0.0f, 0.0f},
                {0.0f, 0.0f},
                {FLT_MAX, FLT_MAX},
                0.0f,
                {SIZE_FLAGS_FIT, SIZE_FLAGS_FIT},
                {CHILD_ALIGNMENT_BEGIN, CHILD_ALIGNMENT_BEGIN},
                CHILD_LAYOUT_AXIS_X,
        },
        {&draw_passthrough},
    } {};

    ~GuiTree() override = default;

    explicit GuiTree(NodeData  in_default_data) : _default_node_data(std::move(in_default_data)) {};

    TGuiThis* BeginRoot() override {
        this->_get(0u) = {
                _default_node_data,
                nullptr,
                nullptr,
                nullptr,
                nullptr,
                nullptr,
        };

        this->_begin_root();
        return this;
    }

    TGuiThis* EndRoot() override {
        this->_end_root();
        return this;
    }

    TGuiThis* Begin() override {
        this->_get(this->_current_index) = {
                _default_node_data,
                this->_current_parent,
                nullptr,
                nullptr,
                nullptr,
                nullptr,
        };

        this->_begin();
        return this;
    }

    TGuiThis* End() override {
        this->_end();
        return this;
    }

    TGuiThis* SetData(NodeData value) override {
        this->_current_parent->data = std::move(value);
        return this;
    }

    TGuiThis* SetBounds(const Rectangle& value) {
        this->_current_parent->data.layout.bounds = value;
        return this;
    }

    TGuiThis* SetPosition(const Vector2& value) {
        this->_current_parent->data.layout.bounds.x = value.x;
        this->_current_parent->data.layout.bounds.y = value.y;
        return this;
    }

    TGuiThis* SetDimensions(const Vector2& value) {
        this->_current_parent->data.layout.bounds.width = value.x;
        this->_current_parent->data.layout.bounds.height = value.y;
        return this;
    }

    TGuiThis* SetMargins(const Vector4& value) {
        this->_current_parent->data.layout.margins = value;
        return this;
    }

    TGuiThis* SetMarginsAll(float value) {
        this->_current_parent->data.layout.margins = {value, value, value, value};
        return this;
    }

    TGuiThis* SetMarginsX(const Vector2& value) {
        this->_current_parent->data.layout.margins.y = value.y;
        this->_current_parent->data.layout.margins.w = value.x;
        return this;
    }

    TGuiThis* SetMarginsY(const Vector2& value) {
        this->_current_parent->data.layout.margins.x = value.x;
        this->_current_parent->data.layout.margins.z = value.y;
        return this;
    }

    TGuiThis* SetPadding(const Vector4& value) {
        this->_current_parent->data.layout.padding = value;
        return this;
    }

    TGuiThis* SetPaddingAll(float value) {
        this->_current_parent->data.layout.padding = {value, value, value, value};
        return this;
    }

    TGuiThis* SetPaddingX(const Vector2& value) {
        this->_current_parent->data.layout.padding.y = value.y;
        this->_current_parent->data.layout.padding.w = value.x;
        return this;
    }

    TGuiThis* SetPaddingY(const Vector2& value) {
        this->_current_parent->data.layout.padding.x = value.x;
        this->_current_parent->data.layout.padding.z = value.y;
        return this;
    }

    TGuiThis* SetMinSize(const Vector2& value) {
        this->_current_parent->data.layout.min_size = value;
        return this;
    }

    TGuiThis* SetMaxSize(const Vector2& value) {
        this->_current_parent->data.layout.max_size = value;
        return this;
    }

    TGuiThis* SetChildSpacing(float value) {
        this->_current_parent->data.layout.child_spacing = value;
        return this;
    }

    TGuiThis* SetSizeFlags(Vector2UInt8 value) {
        this->_current_parent->data.layout.size_flags = value;
        return this;
    }

    TGuiThis* SetSizeFlags(uint8_t value) {
        this->_current_parent->data.layout.size_flags = {value, value};
        return this;
    }

    TGuiThis* SetSizeFlagX(uint8_t value) {
        this->_current_parent->data.layout.size_flags.x = value;
        return this;
    }

    TGuiThis* SetSizeFlagY(uint8_t value) {
        this->_current_parent->data.layout.size_flags.y = value;
        return this;
    }

    TGuiThis* SetChildAlignment(Vector2UInt8 value) {
        this->_current_parent->data.layout.child_alignment = value;
        return this;
    }

    TGuiThis* SetChildLayoutAxis(ChildLayoutAxis value) {
        this->_current_parent->data.layout.child_layout_axis = value;
        return this;
    }

    TGuiThis* SetDrawFunc(const DrawFunction& value) {
        this->_current_parent->data.control.draw = value;
        return this;
    }

    void UpdateLayout() {
        Layout& root_layout = this->Root()->data.layout;
        root_layout.min_size = {0.0f, 0.0f};
        root_layout.max_size = {(float)GetScreenWidth(), (float)GetScreenHeight()};
        root_layout.bounds = {0.0f, 0.0f, (float)GetScreenWidth(), (float)GetScreenHeight()};
        root_layout.size_flags = {SIZE_FLAGS_FIXED, SIZE_FLAGS_FIXED};

        // note:: update fixed widths would go here, but is unnecessary since the bounds' widths are just the values set at design time
        _update_fit_widths();
        _update_grow_and_shrink_widths();
        // note:: update fixed heights would go here, but is unnecessary since the bounds' heights are just the values set at design time
        _update_text_wrapping();
        _update_fit_heights();
        _update_grow_and_shrink_heights();
        _update_positions_and_alignment();
    }

    void Draw() {
        GuiNode* root = this->Root();
        _draw(root);
    }

protected:
    void _draw(GuiNode* current) {
        current->data.control.draw(current->data.layout.bounds);

        GuiNode* current_child = current->first_child;
        while (current_child) {
            _draw(current_child);
            current_child = current_child->right_sibling;
        }
    }

    void _update_fit_widths() {
        size_t last_index = this->_arena_size - 1u;

        for (size_t i = last_index; i > 0u; --i) {
            GuiNode* current = &this->_get(i);

            if (current->first_child) {
                if (current->data.layout.size_flags.x != SIZE_FLAGS_FIXED) {
                    _update_fit_width_containers(current);
                }
            } else {
                // leaf
            }
        }

        // set the root here because its index is 0u and this would cause bit overflow in the for loop above.
        GuiNode* root = this->Root();
        if (root->data.layout.size_flags.x != SIZE_FLAGS_FIXED) {
            _update_fit_width_containers(root);
        }
    }

    void _update_fit_width_containers(GuiNode* node) {
        float width = 0.0f;
        GuiNode* current = node->first_child;
        if (node->data.layout.child_layout_axis == CHILD_LAYOUT_AXIS_X) {
            // add up all the siblings' widths, including their padding on both sides
            size_t child_count = 0u;
            while (current) {
                Layout* layout = &current->data.layout;
                width += layout->bounds.width + layout->margins.y + layout->margins.w;
                ++child_count;

                current = current->right_sibling;
            }

            // add the total spacing from the parent to the total width
            width += node->data.layout.child_spacing * (float)(child_count - 1u);

        } else {
            // get the max width out of each child, including their padding on both sides
            while (current) {
                Layout* layout = &current->data.layout;
                width = fmaxf(layout->bounds.width + layout->margins.y + layout->margins.w, width);

                current = current->right_sibling;
            }
        }

        width += node->data.layout.padding.y + node->data.layout.padding.w;

        // set the parent's width to the calculated width
        node->data.layout.bounds.width = width;
    }

    void _update_grow_and_shrink_widths() {
        for (size_t i = 0u; i < this->_arena_size; ++i) {
            GuiNode* current = this->_bfs_queue[i];

            if (current->first_child) {
                _update_grow_width_containers(this->_bfs_queue[i]);
            }
        }
    }

    void _update_grow_width_containers(GuiNode* node) {
        float parent_remaining_width = (
                node->data.layout.bounds.width
                - node->data.layout.padding.w
                - node->data.layout.padding.y
        );

        if (node->data.layout.child_layout_axis == CHILD_LAYOUT_AXIS_X) {
            GuiNode *current_child = node->first_child;
            size_t child_count = 0u;
            while (current_child) {
                parent_remaining_width -= (
                        current_child->data.layout.bounds.width
                        + current_child->data.layout.margins.w
                        + current_child->data.layout.margins.y
                );

                ++child_count;

                current_child = current_child->right_sibling;
            }

            float child_spacing = node->data.layout.child_spacing / (float) (child_count / 2u);
            parent_remaining_width -= child_spacing;

            size_t growable_child_count = 0u;
            current_child = node->first_child;
            while (current_child) {
                if (current_child->data.layout.size_flags.x == SIZE_FLAGS_GROW) {
                    ++growable_child_count;
                }

                current_child = current_child->right_sibling;
            }

            while (parent_remaining_width > 0.0f && growable_child_count > 0u) {
                current_child = node->first_child;
                float smallest_width = FLT_MAX;
                float second_smallest_width = FLT_MAX;
                float width_to_add = parent_remaining_width;
                while (current_child) {
                    if (current_child->data.layout.size_flags.x == SIZE_FLAGS_GROW) {
                        if (current_child->data.layout.bounds.width < smallest_width && current_child->data.layout.bounds.width < current_child->data.layout.max_size.x) {
                            second_smallest_width = smallest_width;
                            smallest_width = current_child->data.layout.bounds.width;
                        }

                        if (current_child->data.layout.bounds.width > smallest_width && current_child->data.layout.bounds.width < current_child->data.layout.max_size.x) {
                            second_smallest_width = fminf(second_smallest_width, current_child->data.layout.bounds.width);
                            width_to_add = second_smallest_width - smallest_width;
                        }
                    }

                    current_child = current_child->right_sibling;
                }

                width_to_add = fminf(width_to_add, parent_remaining_width / (float)growable_child_count);

                current_child = node->first_child;
                while (current_child) {
                    if (current_child->data.layout.size_flags.x == SIZE_FLAGS_GROW) {
                        if (FloatEquals(current_child->data.layout.bounds.width, smallest_width) && current_child->data.layout.bounds.width < current_child->data.layout.max_size.x) {
                            float clamped_width_to_add = fminf(width_to_add, current_child->data.layout.max_size.x - current_child->data.layout.bounds.width);
                            current_child->data.layout.bounds.width += clamped_width_to_add;
                            parent_remaining_width -= clamped_width_to_add;

                            if (FloatEquals(current_child->data.layout.bounds.width, current_child->data.layout.max_size.x)) {
                                --growable_child_count;
                            }
                        }
                    }

                    current_child = current_child->right_sibling;
                }
            }
        } else {
            GuiNode *current_child = node->first_child;
            while (current_child) {
                if (current_child->data.layout.size_flags.x == SIZE_FLAGS_GROW) {
                    current_child->data.layout.bounds.width += (
                            parent_remaining_width
                            - current_child->data.layout.bounds.width
                            - current_child->data.layout.margins.w
                            - current_child->data.layout.margins.y
                    );

                    current_child->data.layout.bounds.width = fminf(
                            current_child->data.layout.bounds.width,
                            current_child->data.layout.max_size.x
                    );
                }

                current_child = current_child->right_sibling;
            }
        }
    }

    void _update_text_wrapping() {}

    void _update_fit_heights() {
        size_t last_index = this->_arena_size - 1u;

        for (size_t i = last_index; i > 0u; --i) {
            GuiNode* current = &this->_get(i);

            if (current->first_child) {
                if (current->data.layout.size_flags.y != SIZE_FLAGS_FIXED) {
                    _update_fit_height_containers(current);
                }
            } else {
                // leaf
            }
        }

        // set the root here because its index is 0u and this would cause bit overflow in the for loop above.
        GuiNode* root = this->Root();
        if (root->data.layout.size_flags.y != SIZE_FLAGS_FIXED) {
            _update_fit_height_containers(root);
        }
    }

    void _update_fit_height_containers(GuiNode* node) {
        float height = 0.0f;
        GuiNode* current = node->first_child;
        if (node->data.layout.child_layout_axis == CHILD_LAYOUT_AXIS_Y) {
            // add up all the siblings' heights, including their padding on both top and bottom
            size_t child_count = 0u;
            while (current) {
                Layout* layout = &current->data.layout;
                height += layout->bounds.height + layout->margins.x + layout->margins.z;
                ++child_count;

                current = current->right_sibling;
            }

            // add the total spacing from the parent to the total width
            height += node->data.layout.child_spacing * (float)(child_count - 1u);

        } else {
            // get the max width out of each child, including their padding on both sides
            while (current) {
                Layout* layout = &current->data.layout;
                height = fmaxf(layout->bounds.height + layout->margins.x + layout->margins.z, height);

                current = current->right_sibling;
            }
        }

        height += node->data.layout.padding.x + node->data.layout.padding.z;

        // set the parent's width to the calculated width
        node->data.layout.bounds.height = height;
    }

    void _update_grow_and_shrink_heights() {}

    void _update_positions_and_alignment() {
        GuiNode* root = this->Root();
        _update_positions_and_alignment(root);
    }

    void _update_positions_and_alignment(GuiNode* node) {
        if (node->first_child) {
            switch (node->data.layout.child_layout_axis) {
                case CHILD_LAYOUT_AXIS_X:
                    _set_children_positions_along_x(node);
                    break;
                case CHILD_LAYOUT_AXIS_Y:
                    _set_children_positions_along_y(node);
                    break;
                default:
                    break;
            }
        }

        GuiNode* current = node->first_child;
        while (current) {
            _update_positions_and_alignment(current);

            current = current->right_sibling;
        }
    }

    void _set_children_positions_along_x(GuiNode* current) {
        switch (current->data.layout.child_alignment.x) {
            case CHILD_ALIGNMENT_BEGIN:
                _set_children_x_begin_along_x(current);
                break;
            case CHILD_ALIGNMENT_CENTER:
                _set_children_x_center_along_x(current);
                break;
            case CHILD_ALIGNMENT_END:
                _set_children_x_end_along_x(current);
                break;
            case CHILD_ALIGNMENT_RADIAL:
                break;
            default:
                _set_children_x_begin_along_x(current);
                break;
        }

        switch (current->data.layout.child_alignment.y) {
            case CHILD_ALIGNMENT_BEGIN:
                _set_children_y_begin_along_x(current);
                break;
            case CHILD_ALIGNMENT_CENTER:
                _set_children_y_center_along_x(current);
                break;
            case CHILD_ALIGNMENT_END:
                _set_children_y_end_along_x(current);
                break;
            case CHILD_ALIGNMENT_RADIAL:
                break;
            default:
                _set_children_y_begin_along_x(current);
                break;
        }
    }

    void _set_children_positions_along_y(GuiNode* current) {
        switch (current->data.layout.child_alignment.x) {
            case CHILD_ALIGNMENT_BEGIN:
                break;
            case CHILD_ALIGNMENT_CENTER:
                break;
            case CHILD_ALIGNMENT_END:
                break;
            case CHILD_ALIGNMENT_RADIAL:
                break;
            default:
                break;
        }

        switch (current->data.layout.child_alignment.y) {
            case CHILD_ALIGNMENT_BEGIN:
                break;
            case CHILD_ALIGNMENT_CENTER:
                break;
            case CHILD_ALIGNMENT_END:
                break;
            case CHILD_ALIGNMENT_RADIAL:
                break;
            default:
                break;
        }
    }

    void _set_children_x_begin_along_x(GuiNode* current) {
        size_t child_count = 0u;
        GuiNode *current_child = current->first_child;
        while (current_child) {
            ++child_count;

            current_child = current_child->right_sibling;
        }

        float child_spacing = current->data.layout.child_spacing * (float) (child_count - 1u);
        float current_x = current->data.layout.bounds.x + current->data.layout.padding.w;

        current_child = current->first_child;
        while (current_child) {
            float left_padding_adjustment_x = current_child->data.layout.margins.w;
            if (current_child->left_sibling) {
                left_padding_adjustment_x += current_child->left_sibling->data.layout.margins.y + child_spacing;
            }

            current_child->data.layout.bounds.x = current_x + left_padding_adjustment_x;
            current_x += current_child->data.layout.bounds.width + left_padding_adjustment_x;

            current_child = current_child->right_sibling;
        }
    }

    void _set_children_y_begin_along_x(GuiNode* current) {
        float current_y = current->data.layout.bounds.y + current->data.layout.padding.x;
        GuiNode *current_child = current->first_child;
        while (current_child) {
            float top_padding_adjustment = current_child->data.layout.margins.x;
            current_child->data.layout.bounds.y = current_y + top_padding_adjustment;

            current_child = current_child->right_sibling;
        }
    }

    void _set_children_x_center_along_x(GuiNode* current) {
        float child_widths = 0.0f;
        size_t child_count = 0u;
        GuiNode *current_child = current->first_child;

        child_widths += current->data.layout.padding.y + current->data.layout.padding.z;
        while (current_child) {
            child_widths += (
                    current_child->data.layout.bounds.width
                    + current_child->data.layout.margins.y
                    + current_child->data.layout.margins.w
            );

            ++child_count;

            current_child = current_child->right_sibling;
        }

        float child_spacing = current->data.layout.child_spacing * (float) (child_count - 1u);

        float current_x = 0.0f;
        switch (current->data.layout.size_flags.x) {
            case SIZE_FLAGS_FIT:
            case SIZE_FLAGS_SHRINK:
                current_x = current->data.layout.bounds.x + current->data.layout.padding.w;
                break;
            case SIZE_FLAGS_GROW:
            case SIZE_FLAGS_FIXED:
                current_x = current->data.layout.bounds.x + current->data.layout.padding.w + ((current->data.layout.bounds.width - child_widths - current->data.layout.child_spacing) / 2.0f);
                break;
            default:
                break;
        }

        current_child = current->first_child;
        while (current_child) {
            float left_padding_adjustment_x = current_child->data.layout.margins.w;
            if (current_child->left_sibling) {
                left_padding_adjustment_x += current_child->left_sibling->data.layout.margins.y + child_spacing;
            }

            current_child->data.layout.bounds.x = current_x + left_padding_adjustment_x;
            current_x += current_child->data.layout.bounds.width + left_padding_adjustment_x;

            current_child = current_child->right_sibling;
        }
    }

    void _set_children_y_center_along_x(GuiNode* current) {
        float current_y_parent_part = 0.0f;

        current_y_parent_part = current->data.layout.bounds.y + (current->data.layout.bounds.height / 2.0f);

        GuiNode *current_child = current->first_child;
        while (current_child) {
            float current_y = (
                    current_y_parent_part
                    - (current_child->data.layout.bounds.height / 2.0f)
            );

            current_child->data.layout.bounds.y = current_y;

            current_child = current_child->right_sibling;
        }
    }

    void _set_children_x_end_along_x(GuiNode* current) {
        size_t child_count = 0u;
        GuiNode *current_child = current->first_child;
        while (current_child) {
            ++child_count;

            current_child = current_child->right_sibling;
        }

        float child_spacing = current->data.layout.child_spacing * (float) (child_count - 1u);
        float current_x = current->data.layout.bounds.x + current->data.layout.bounds.width - current->data.layout.padding.y;

        current_child = current->last_child;
        while (current_child) {
            float right_padding_adjustment_x = current_child->data.layout.margins.y;
            if (current_child->right_sibling) {
                right_padding_adjustment_x += current_child->right_sibling->data.layout.margins.w + child_spacing;
            }

            current_x -= current_child->data.layout.bounds.width + right_padding_adjustment_x;
            current_child->data.layout.bounds.x = current_x;

            current_child = current_child->left_sibling;
        }
    }

    void _set_children_y_end_along_x(GuiNode* current) {
        float current_y_parent_part = current->data.layout.bounds.y + current->data.layout.bounds.height - current->data.layout.padding.z;

        GuiNode *current_child = current->first_child;
        while (current_child) {
            float current_y = current_y_parent_part - current_child->data.layout.bounds.height - current_child->data.layout.margins.z;
            current_child->data.layout.bounds.y = current_y;

            current_child = current_child->right_sibling;
        }
    }

    GuiNode* _get_next_parent(GuiNode* current) {
        GuiNode* result = nullptr;

        return result;
    }
};


//template<typename TElement, typename ... TArgs>
//int CenterContainer(TElement element, float width, float height, TArgs... args) {
//    static_assert(std::is_pointer<TElement>::value, "TElement must be a function pointer.");
//    return element({ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f, width, height }, args...);
//};
//
//
//template<typename TElement, typename ... TArgs>
//int ExpandContainer(TElement element, float x, float y, TArgs... args) {
//    static_assert(std::is_pointer<TElement>::value, "TElement must be a function pointer.");
//    return element({ x, y, GetScreenWidth(), GetScreenHeight() }, args...);
//};
//
//
//template<SizeFlags size_flags>
//typename std::enable_if<(size_flags == SIZE_FLAGS_VERTICAL_EXPAND), Rectangle>::type
//get_vbox_child_bounds(const Layout& layout, const ChildLayout& child_layout) {
//    return {
//            layout.next_child_position.x,
//            layout.next_child_position.y,
//            child_layout.minimum_size.x,
//            layout.bounds.height / layout.child_count,
//    };
//}
//
//
//template<SizeFlags size_flags>
//typename std::enable_if<(size_flags == SIZE_FLAGS_HORIZONTAL_EXPAND), Rectangle>::type
//get_vbox_child_bounds(const Layout& layout, const ChildLayout& child_layout) {
//    return {
//            layout.next_child_position.x,
//            layout.next_child_position.y,
//            layout.bounds.width,
//            child_layout.minimum_size.y,
//    };
//}
//
//
//template<SizeFlags size_flags>
//typename std::enable_if<(size_flags == SIZE_FLAGS_GROW), Rectangle>::type
//get_vbox_child_bounds(const Layout& layout, const ChildLayout& child_layout) {
//    std::ignore = child_layout.size_flags;
//    return {
//            layout.next_child_position.x,
//            layout.next_child_position.y,
//            layout.bounds.width,
//            layout.bounds.height / layout.child_count,
//    };
//}
//
//
//template<SizeFlags size_flags>
//typename std::enable_if<(size_flags & SIZE_FLAGS_HORIZONTAL_SHRINK_BEGIN) != 0, Rectangle>::type
//get_vbox_child_bounds(const Layout& layout, const ChildLayout& child_layout) {
//    return {
//            layout.next_child_position.x,
//            layout.next_child_position.y,
//            child_layout.minimum_size.x,
//            child_layout.minimum_size.y,
//    };
//}
//
//
//template<SizeFlags size_flags>
//typename std::enable_if<(size_flags & SIZE_FLAGS_HORIZONTAL_SHRINK_CENTER) != 0, Rectangle>::type
//get_vbox_child_bounds(const Layout& layout, const ChildLayout& child_layout) {
//    return {
//            layout.next_child_position.x + ((layout.bounds.width / 2.0f) - (child_layout.minimum_size.x / 2.0f)),
//            layout.next_child_position.y,
//            child_layout.minimum_size.x,
//            child_layout.minimum_size.y,
//    };
//}
//
//
//template<SizeFlags size_flags>
//typename std::enable_if<(size_flags & SIZE_FLAGS_HORIZONTAL_SHRINK_END) != 0, Rectangle>::type
//get_vbox_child_bounds(const Layout& layout, const ChildLayout& child_layout) {
//    return {
//            layout.bounds.x + layout.bounds.width - child_layout.minimum_size.x,
//            layout.next_child_position.y,
//            child_layout.minimum_size.x,
//            child_layout.minimum_size.y,
//    };
//}
//
//
//Rectangle get_vbox_child_bounds(const Layout& layout, const ChildLayout& child_layout) {
//    switch (child_layout.size_flags) {
//        case SIZE_FLAGS_HORIZONTAL_SHRINK_BEGIN:
//            return get_vbox_child_bounds<SIZE_FLAGS_HORIZONTAL_SHRINK_BEGIN>(layout, child_layout);
//        case SIZE_FLAGS_HORIZONTAL_SHRINK_CENTER:
//            return get_vbox_child_bounds<SIZE_FLAGS_HORIZONTAL_SHRINK_CENTER>(layout, child_layout);
//        case SIZE_FLAGS_HORIZONTAL_SHRINK_END:
//            return get_vbox_child_bounds<SIZE_FLAGS_HORIZONTAL_SHRINK_END>(layout, child_layout);
//        case SIZE_FLAGS_HORIZONTAL_EXPAND:
//            return get_vbox_child_bounds<SIZE_FLAGS_HORIZONTAL_EXPAND>(layout, child_layout);
//        case SIZE_FLAGS_VERTICAL_EXPAND:
//            return get_vbox_child_bounds<SIZE_FLAGS_VERTICAL_EXPAND>(layout, child_layout);
//        case SIZE_FLAGS_SHRINK_BEGIN:
//            return get_vbox_child_bounds<SIZE_FLAGS_SHRINK_BEGIN>(layout, child_layout);
//        case SIZE_FLAGS_SHRINK_CENTER:
//            return get_vbox_child_bounds<SIZE_FLAGS_SHRINK_CENTER>(layout, child_layout);
//        case SIZE_FLAGS_SHRINK_END:
//            return get_vbox_child_bounds<SIZE_FLAGS_SHRINK_END>(layout, child_layout);
//        case SIZE_FLAGS_GROW:
//            return get_vbox_child_bounds<SIZE_FLAGS_GROW>(layout, child_layout);
//        case SIZE_FLAGS_VERTICAL_SHRINK_BEGIN:
//        case SIZE_FLAGS_VERTICAL_SHRINK_CENTER:
//        case SIZE_FLAGS_VERTICAL_SHRINK_END:
//        default:
//            return get_vbox_child_bounds<SIZE_FLAGS_SHRINK_BEGIN>(layout, child_layout);
//    }
//}


//template<typename TElement, typename ... TArgs>
//int VBoxChild(ContainerLayout& layout, const ContainerChildLayout& child_layout, TElement element, TArgs... args) {
//    static_assert(std::is_pointer<TElement>::value, "TElement must be a function pointer.");
//    Rectangle child_bounds = get_vbox_child_bounds(layout, child_layout);
//
//    layout.index++;
//    if (layout.index < layout.child_count) {
//        layout.next_child_position.y = child_bounds.y + child_bounds.height;
//    } else {
//        layout.index = 0;
//        layout.next_child_position.y = 0.0f;
//    }
//
//    return element(child_bounds, args...);
//};
//
//
//    template<SizeFlags size_flags, typename TElement, typename ... TArgs>
//    int VBoxChild(ContainerLayout& layout, const ContainerChildLayout& child_layout, TElement element, TArgs... args) {
//        static_assert(std::is_pointer<TElement>::value, "TElement must be a function pointer.");
//        Rectangle child_bounds = get_vbox_child_bounds<size_flags>(layout, child_layout);
//
//        layout.index++;
//        if (layout.index < layout.child_count) {
//            layout.next_child_position.y = child_bounds.y + child_bounds.height;
//        } else {
//            layout.index = 0;
//            layout.next_child_position.y = 0.0f;
//        }
//
//        return element(child_bounds, args...);
//    };


//template<typename TElement, typename ... TArgs>
//int VBoxContainer(const Rectangle& bounds, TElement element, TArgs... args) {
//    static_assert(std::is_pointer<TElement>::value, "TElement must be a function pointer.");
//
//    return element(bounds, args...);
//};
//
//template<typename TElement, typename ... TElements, typename ... TArgs>
//typename std::enable_if<(sizeof ... (TElements) > 0), int>::type
//VBoxContainer(const Rectangle& bounds, TElement element, TElements... elements, TArgs... args) {
//    static_assert(std::is_pointer<TElement>::value, "TElement must be a function pointer.");
//    static_assert(std::is_pointer<TElements...>::value, "TElements must be function pointers.");
//    constexpr size_t _child_count = sizeof ... (elements) + 1u;
//    constexpr float _child_countf = static_cast<float>(_child_count);
//
//    Rectangle _bounds = bounds;
//    Rectangle child_bounds = _bounds;
//    child_bounds.height = bounds.height / _child_countf;
//    _bounds.y += child_bounds.height;
//
//    VBoxChild(element, child_bounds, args...);
//
//    return VBoxContainer(_bounds, elements..., args...);
//};

} // AutoRayGui

#endif //AUTO_RAY_GUI_GUI_GRAPH_HPP
