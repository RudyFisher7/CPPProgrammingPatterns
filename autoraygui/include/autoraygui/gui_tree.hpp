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

#include <array>
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
                {0.0f, 0.0f},
                {0.0f, 0.0f},
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

    TGuiThis* SetPadding(const Vector4& value) {
        this->_current_parent->data.layout.bounds = value;
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

        root->data.control.draw(root->data.layout.bounds);
        root->first_child->data.control.draw(root->first_child->data.layout.bounds);
    }

protected:
    void _update_fit_widths() {
        GuiNode* root = this->Root();
        GuiNode* current = root;

        // get to the left-most bottom leaf node of the tree
        while (current->first_child != nullptr) {
            current = current->first_child;
        }

        while (current != root) {
            // if the parent has a fixed size, then its children don't determine its size
            GuiNode* current_parent = current->parent;
            if (current_parent->data.layout.size_flags.x != SIZE_FLAGS_FIXED) {// todo:: revisit this. i think we assume all parents fit their children in this pass, then the 'grow' pass will overwrite the necessary parents' widths
                float width = 0.0f;
                if (current_parent->data.layout.child_layout_axis == CHILD_LAYOUT_AXIS_X) {
                    float spacing_from_parent = current_parent->data.layout.child_spacing;
                    size_t child_count = 1u;
                    Layout* layout = &current->data.layout;
                    // add up all the siblings' widths, including their padding on both sides
                    width += layout->bounds.width + layout->padding.y + layout->padding.z;
                    while (current->right_sibling != nullptr) {
                        layout = &current->data.layout;
                        width += layout->bounds.width + layout->padding.y + layout->padding.z;
                        current = current->right_sibling;

                        ++child_count;
                    }

                    // add the total spacing from the parent to the total width
                    width += spacing_from_parent * (float)(child_count - 1u);

                } else {
                    // get the max width out of each child, including their padding on both sides
                    Layout* layout = &current->data.layout;
                    width = fmaxf(layout->bounds.width + layout->padding.y + layout->padding.z, width);
                    while (current->right_sibling != nullptr) {
                        layout = &current->data.layout;
                        width = fmaxf(layout->bounds.width + layout->padding.y + layout->padding.z, width);
                        current = current->right_sibling;
                    }
                }

                // set the parent's width to the calculated width
                current_parent->data.layout.bounds.width = width;
            }

            // if the parent has a sibling to the right
            if (current_parent->right_sibling) {
                // get the next parent's first child and repeat
                if (current_parent->right_sibling->first_child) {
                    current = current_parent->right_sibling->first_child;
                }
            } else if (current_parent->parent) {
                // else, if there are no more parents, then return to left-most parent
                // the parents are now the current children to add up the widths of
                if (current_parent->parent->first_child) {
                    current = current_parent->parent->first_child;
                } else {
                    current = current_parent;
                }
            } else {
                current = current_parent;
            }

            current_parent = current->parent;
        }
    }

    void _update_grow_and_shrink_widths() {
//        GuiNode* root = this->Root();
//        Layout& layout = root->first_child->data.layout;
//        layout.bounds.width = layout.min_size.x;
    }

    void _update_text_wrapping() {}

    void _update_fit_heights() {}
    void _update_grow_and_shrink_heights() {
//        GuiNode* root = this->Root();
//        Layout& layout = root->first_child->data.layout;
//        layout.bounds.height = layout.min_size.y;
    }

    void _update_positions_and_alignment() {
        GuiNode* root = this->Root();

        GuiNode* current_layer_start = root;
        while (current_layer_start->right_sibling != nullptr) {

//            GuiNode* current = current_layer_start->
        }

        _set_child_position(root->first_child->data.layout.bounds, root->data.layout.bounds, root->data.layout.child_alignment);
    }

    void _set_child_position(Rectangle& child_bounds, const Rectangle& parent_bounds, const Vector2UInt8& child_alignment) {
        switch (child_alignment.x) {
            case CHILD_ALIGNMENT_CENTER:
                child_bounds.x = parent_bounds.x + ((parent_bounds.width / 2.0f) - (child_bounds.width / 2.0f));
                break;
            default:
                break;
        }

        switch (child_alignment.y) {
            case CHILD_ALIGNMENT_CENTER:
                child_bounds.y = parent_bounds.y + ((parent_bounds.height / 2.0f) - (child_bounds.height / 2.0f));
                break;
            default:
                break;
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
//typename std::enable_if<(size_flags == SIZE_FLAGS_EXPAND), Rectangle>::type
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
//        case SIZE_FLAGS_EXPAND:
//            return get_vbox_child_bounds<SIZE_FLAGS_EXPAND>(layout, child_layout);
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
