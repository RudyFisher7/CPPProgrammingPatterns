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
class GuiGraph final {
private:
    typedef GuiGraph<size, indexing_mode> Graph;

    std::array<GuiNode, size> _arena {};

    size_t _next_id;
    size_t _arena_size;

    GuiNode* _current_parent;
    GuiNode* _current_left_sibling;

public:
    GuiGraph() : _next_id(0u), _arena_size(0u), _current_parent(nullptr), _current_left_sibling(nullptr) {}

    Graph* BeginRoot() {
        _get(_next_id) = {
            {
                {
                     {0.0f, 0.0f, 0.0f, 0.0f},
                     {0.0f, 0.0f},
                     {0.0f, 0.0f},
                     SIZE_FLAGS_FIT,
                     CONTAINER_TYPE_ROOT,
                },
                { &draw_passthrough },
            },
            nullptr,
            nullptr,
            nullptr,
            nullptr,
            nullptr,
        };

        GuiNode* root = &_get(_next_id++);

        _current_parent = root;

        return this;
    }

    Graph* EndRoot() {
        _arena_size = _next_id;
        _next_id = 0;
        return this;
    }

    Graph* Begin() {
        _get(_next_id++) = {
                {
                        {
                                {0.0f, 0.0f, 0.0f, 0.0f},
                                {0.0f, 0.0f},
                                {0.0f, 0.0f},
                                SIZE_FLAGS_SHRINK,
                                CONTAINER_TYPE_CHILD,
                        },
                        { &draw_passthrough },
                },
                _current_parent,
                _current_left_sibling,
                nullptr,
                nullptr,
                nullptr,
        };

        GuiNode* child = &_get(_next_id++);

        if (!_current_parent->first_child) {
            _current_parent->first_child = child;
        }

        _current_parent->last_child = child;

        if (_current_left_sibling) {
            _current_left_sibling->right_sibling = child;
        } else {
            _current_left_sibling = child;
        }

        _current_parent = child;
        _current_left_sibling = nullptr;

        return this;
    }

    Graph* End() {
        _current_left_sibling = _current_parent->right_sibling;
        _current_parent = _current_parent->parent;
        return this;
    }

    Graph* Bounds(const Rectangle& bounds) {
        _current_parent->data.layout.bounds = bounds;
        return this;
    }

    Graph* MinSize(const Vector2& min_size) {
        _current_parent->data.layout.min_size = min_size;
        return this;
    }

    Graph* MaxSize(const Vector2& max_size) {
        _current_parent->data.layout.max_size = max_size;
        return this;
    }

    Graph* SizeFlags(SizeFlags size_flags) {
        _current_parent->data.layout.size_flags = size_flags;
        return this;
    }

    Graph* ContainerType(ContainerType type) {
        _current_parent->data.layout.type = type;
        return this;
    }

    Graph* DrawFunc(const DrawFunction& draw) {
        _current_parent->data.control.draw = draw;
        return this;
    }

    void UpdateLayout() {
        _get(0u).data.layout = {
                {0.0f, 0.0f, GetScreenWidth(), GetScreenHeight()},
                {0.0f, 0.0f},
                {GetMonitorWidth(0), GetMonitorHeight(0)},
                SIZE_FLAGS_FIT,
                CONTAINER_TYPE_ROOT,
        };

        _update_fit_widths();
        _update_grow_and_shrink_widths();
        _update_text_wrapping();
        _update_fit_heights();
        _update_grow_and_shrink_heights();
        _update_positions_and_anchors();
    }

    void Draw() {}

private:
    void _update_fit_widths() {}
    void _update_grow_and_shrink_widths() {}
    void _update_text_wrapping() {}
    void _update_fit_heights() {}
    void _update_grow_and_shrink_heights() {}
    void _update_positions_and_anchors() {}

    inline int _register_next_id() {
        return ++_next_id;
    }

    template<IndexingMode in_indexing_mode = indexing_mode>
    inline typename std::enable_if<in_indexing_mode == INDEXING_MODE_SAFE, GuiNode&>::type
    _get(int index) {
        return _arena.at(index);
    }

    template<IndexingMode in_indexing_mode = indexing_mode>
    inline typename std::enable_if<in_indexing_mode == INDEXING_MODE_UNCHECKED, GuiNode&>::type
    _get(int index) {
        return _arena[index];
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
