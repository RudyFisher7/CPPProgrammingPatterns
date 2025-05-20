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


#ifndef AUTO_RAY_GUI_ENUMS_HPP
#define AUTO_RAY_GUI_ENUMS_HPP


#include <cstdint>


namespace AutoRayGui {

enum SizeFlags : uint16_t {
    SIZE_FLAGS_HORIZONTAL_FIT = 0x01 << 0,
    SIZE_FLAGS_HORIZONTAL_FIXED = 0x01 << 1,
    SIZE_FLAGS_HORIZONTAL_SHRINK = 0x01 << 2,
    SIZE_FLAGS_HORIZONTAL_EXPAND = 0x01 << 3,

    SIZE_FLAGS_VERTICAL_FIT = 0x01 << 4,
    SIZE_FLAGS_VERTICAL_FIXED = 0x01 << 5,
    SIZE_FLAGS_VERTICAL_SHRINK = 0x01 << 6,
    SIZE_FLAGS_VERTICAL_EXPAND = 0x01 << 7,

    SIZE_FLAGS_FIT = SIZE_FLAGS_HORIZONTAL_FIT | SIZE_FLAGS_VERTICAL_FIT,
    SIZE_FLAGS_FIXED = SIZE_FLAGS_HORIZONTAL_FIXED | SIZE_FLAGS_VERTICAL_FIXED,
    SIZE_FLAGS_SHRINK = SIZE_FLAGS_HORIZONTAL_SHRINK | SIZE_FLAGS_VERTICAL_SHRINK,
    SIZE_FLAGS_EXPAND = SIZE_FLAGS_HORIZONTAL_EXPAND | SIZE_FLAGS_VERTICAL_EXPAND,
};

enum ChildLayoutAxis : uint8_t {
    CHILD_LAYOUT_AXIS_MIN = 0,
    CHILD_LAYOUT_AXIS_X = CHILD_LAYOUT_AXIS_MIN,
    CHILD_LAYOUT_AXIS_Y,
    CHILD_LAYOUT_AXIS_SIZE,
};

enum ContainerType : uint8_t {
    CONTAINER_TYPE_MIN = 0,
    CONTAINER_TYPE_NONE = CONTAINER_TYPE_MIN,
    CONTAINER_TYPE_CHILD = CONTAINER_TYPE_NONE,
    CONTAINER_TYPE_ROOT,
    CONTAINER_TYPE_HBOX,
    CONTAINER_TYPE_VBOX,
    CONTAINER_TYPE_GRID,
    CONTAINER_TYPE_CENTER,
    CONTAINER_TYPE_MARGIN,
    CONTAINER_TYPE_PANEL,
    CONTAINER_TYPE_BORDER,
    CONTAINER_TYPE_SCROLL,
    CONTAINER_TYPE_TAB,
    CONTAINER_TYPE_SUBVIEWPORT,
    CONTAINER_TYPE_SIZE,
};


enum IndexingMode : uint8_t {
    INDEXING_MODE_SAFE = 0,
    INDEXING_MODE_UNCHECKED,
};

}


#endif //AUTO_RAY_GUI_ENUMS_HPP
