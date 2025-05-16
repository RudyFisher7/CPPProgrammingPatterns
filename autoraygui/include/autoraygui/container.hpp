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


#ifndef AUTO_RAY_GUI_CONTAINER_HPP
#define AUTO_RAY_GUI_CONTAINER_HPP

#include <raylib.h>

#include <cstdint>
#include <functional>
#include <type_traits>
#include <tuple>


namespace AutoRayGui {

enum SizeFlags : uint16_t {
    SIZE_FLAGS_HORIZONTAL_SHRINK_BEGIN = 0x01 << 0,
    SIZE_FLAGS_HORIZONTAL_SHRINK_CENTER = 0x01 << 1,
    SIZE_FLAGS_HORIZONTAL_SHRINK_END = 0x01 << 2,
    SIZE_FLAGS_HORIZONTAL_EXPAND = 0x01 << 3,

    SIZE_FLAGS_VERTICAL_SHRINK_BEGIN = 0x01 << 4,
    SIZE_FLAGS_VERTICAL_SHRINK_CENTER = 0x01 << 5,
    SIZE_FLAGS_VERTICAL_SHRINK_END = 0x01 << 6,
    SIZE_FLAGS_VERTICAL_EXPAND = 0x01 << 7,

    SIZE_FLAGS_SHRINK_BEGIN = SIZE_FLAGS_HORIZONTAL_SHRINK_BEGIN | SIZE_FLAGS_VERTICAL_SHRINK_BEGIN,
    SIZE_FLAGS_SHRINK_CENTER = SIZE_FLAGS_HORIZONTAL_SHRINK_CENTER | SIZE_FLAGS_VERTICAL_SHRINK_CENTER,
    SIZE_FLAGS_SHRINK_END = SIZE_FLAGS_HORIZONTAL_SHRINK_END | SIZE_FLAGS_VERTICAL_SHRINK_END,
    SIZE_FLAGS_EXPAND = SIZE_FLAGS_HORIZONTAL_EXPAND | SIZE_FLAGS_VERTICAL_EXPAND,
};

struct ContainerLayout {
    Rectangle bounds;
    size_t child_count;
    size_t index;
    Vector2 next_child_position;
};

struct ContainerChildLayout {
    Vector2 minimum_size;
    SizeFlags size_flags;
};

template<typename TElement, typename ... TArgs>
int CenterContainer(TElement element, float width, float height, TArgs... args) {
    static_assert(std::is_pointer<TElement>::value, "TElement must be a function pointer.");
    return element({ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f, width, height }, args...);
};


template<typename TElement, typename ... TArgs>
int ExpandContainer(TElement element, float x, float y, TArgs... args) {
    static_assert(std::is_pointer<TElement>::value, "TElement must be a function pointer.");
    return element({ x, y, GetScreenWidth(), GetScreenHeight() }, args...);
};


template<SizeFlags size_flags>
typename std::enable_if<(size_flags == SIZE_FLAGS_VERTICAL_EXPAND), Rectangle>::type
get_vbox_child_bounds(const ContainerLayout& layout, const ContainerChildLayout& child_layout) {
    return {
            layout.next_child_position.x,
            layout.next_child_position.y,
            child_layout.minimum_size.x,
            layout.bounds.height / layout.child_count,
    };
}


template<SizeFlags size_flags>
typename std::enable_if<(size_flags == SIZE_FLAGS_HORIZONTAL_EXPAND), Rectangle>::type
get_vbox_child_bounds(const ContainerLayout& layout, const ContainerChildLayout& child_layout) {
    return {
            layout.next_child_position.x,
            layout.next_child_position.y,
            layout.bounds.width,
            child_layout.minimum_size.y,
    };
}


template<SizeFlags size_flags>
typename std::enable_if<(size_flags == SIZE_FLAGS_EXPAND), Rectangle>::type
get_vbox_child_bounds(const ContainerLayout& layout, const ContainerChildLayout& child_layout) {
    std::ignore = child_layout.size_flags;
    return {
            layout.next_child_position.x,
            layout.next_child_position.y,
            layout.bounds.width,
            layout.bounds.height / layout.child_count,
    };
}


template<SizeFlags size_flags>
typename std::enable_if<(size_flags & SIZE_FLAGS_HORIZONTAL_SHRINK_BEGIN) != 0, Rectangle>::type
get_vbox_child_bounds(const ContainerLayout& layout, const ContainerChildLayout& child_layout) {
    return {
            layout.next_child_position.x,
            layout.next_child_position.y,
            child_layout.minimum_size.x,
            child_layout.minimum_size.y,
    };
}


template<SizeFlags size_flags>
typename std::enable_if<(size_flags & SIZE_FLAGS_HORIZONTAL_SHRINK_CENTER) != 0, Rectangle>::type
get_vbox_child_bounds(const ContainerLayout& layout, const ContainerChildLayout& child_layout) {
    return {
            layout.next_child_position.x + ((layout.bounds.width / 2.0f) - (child_layout.minimum_size.x / 2.0f)),
            layout.next_child_position.y,
            child_layout.minimum_size.x,
            child_layout.minimum_size.y,
    };
}


template<SizeFlags size_flags>
typename std::enable_if<(size_flags & SIZE_FLAGS_HORIZONTAL_SHRINK_END) != 0, Rectangle>::type
get_vbox_child_bounds(const ContainerLayout& layout, const ContainerChildLayout& child_layout) {
    return {
            layout.bounds.x + layout.bounds.width - child_layout.minimum_size.x,
            layout.next_child_position.y,
            child_layout.minimum_size.x,
            child_layout.minimum_size.y,
    };
}


Rectangle get_vbox_child_bounds(const ContainerLayout& layout, const ContainerChildLayout& child_layout) {
    switch (child_layout.size_flags) {
        case SIZE_FLAGS_HORIZONTAL_SHRINK_BEGIN:
            return get_vbox_child_bounds<SIZE_FLAGS_HORIZONTAL_SHRINK_BEGIN>(layout, child_layout);
        case SIZE_FLAGS_HORIZONTAL_SHRINK_CENTER:
            return get_vbox_child_bounds<SIZE_FLAGS_HORIZONTAL_SHRINK_CENTER>(layout, child_layout);
        case SIZE_FLAGS_HORIZONTAL_SHRINK_END:
            return get_vbox_child_bounds<SIZE_FLAGS_HORIZONTAL_SHRINK_END>(layout, child_layout);
        case SIZE_FLAGS_HORIZONTAL_EXPAND:
            return get_vbox_child_bounds<SIZE_FLAGS_HORIZONTAL_EXPAND>(layout, child_layout);
        case SIZE_FLAGS_VERTICAL_EXPAND:
            return get_vbox_child_bounds<SIZE_FLAGS_VERTICAL_EXPAND>(layout, child_layout);
        case SIZE_FLAGS_SHRINK_BEGIN:
            return get_vbox_child_bounds<SIZE_FLAGS_SHRINK_BEGIN>(layout, child_layout);
        case SIZE_FLAGS_SHRINK_CENTER:
            return get_vbox_child_bounds<SIZE_FLAGS_SHRINK_CENTER>(layout, child_layout);
        case SIZE_FLAGS_SHRINK_END:
            return get_vbox_child_bounds<SIZE_FLAGS_SHRINK_END>(layout, child_layout);
        case SIZE_FLAGS_EXPAND:
            return get_vbox_child_bounds<SIZE_FLAGS_EXPAND>(layout, child_layout);
        case SIZE_FLAGS_VERTICAL_SHRINK_BEGIN:
        case SIZE_FLAGS_VERTICAL_SHRINK_CENTER:
        case SIZE_FLAGS_VERTICAL_SHRINK_END:
        default:
            return get_vbox_child_bounds<SIZE_FLAGS_SHRINK_BEGIN>(layout, child_layout);
    }
}


template<typename TElement, typename ... TArgs>
int VBoxChild(ContainerLayout& layout, const ContainerChildLayout& child_layout, TElement element, TArgs... args) {
    static_assert(std::is_pointer<TElement>::value, "TElement must be a function pointer.");
    Rectangle child_bounds = get_vbox_child_bounds(layout, child_layout);

    layout.index++;
    if (layout.index < layout.child_count) {
        layout.next_child_position.y = child_bounds.y + child_bounds.height;
    } else {
        layout.index = 0;
        layout.next_child_position.y = 0.0f;
    }

    return element(child_bounds, args...);
};


    template<SizeFlags size_flags, typename TElement, typename ... TArgs>
    int VBoxChild(ContainerLayout& layout, const ContainerChildLayout& child_layout, TElement element, TArgs... args) {
        static_assert(std::is_pointer<TElement>::value, "TElement must be a function pointer.");
        Rectangle child_bounds = get_vbox_child_bounds<size_flags>(layout, child_layout);

        layout.index++;
        if (layout.index < layout.child_count) {
            layout.next_child_position.y = child_bounds.y + child_bounds.height;
        } else {
            layout.index = 0;
            layout.next_child_position.y = 0.0f;
        }

        return element(child_bounds, args...);
    };


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

#endif //AUTO_RAY_GUI_CONTAINER_HPP
