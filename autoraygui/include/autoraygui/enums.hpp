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

enum SizeFlags : uint8_t {
    SIZE_FLAGS_FIT = 0x01 << 0,
    SIZE_FLAGS_FIXED = 0x01 << 1,
    SIZE_FLAGS_SHRINK = 0x01 << 2,
    SIZE_FLAGS_EXPAND = 0x01 << 3,
};


enum ChildAlignment : uint8_t {
    CHILD_ALIGNMENT_BEGIN = 0x01 << 0,
    CHILD_ALIGNMENT_CENTER = 0x01 << 1,
    CHILD_ALIGNMENT_END = 0x01 << 2,
    CHILD_ALIGNMENT_RADIAL = 0x01 << 3,
};

typedef struct vector2_uint8_t {
    uint8_t x;
    uint8_t y;
} Vector2UInt8;


enum ChildLayoutAxis : uint8_t {
    CHILD_LAYOUT_AXIS_MIN = 0,
    CHILD_LAYOUT_AXIS_X = CHILD_LAYOUT_AXIS_MIN,
    CHILD_LAYOUT_AXIS_Y,
    CHILD_LAYOUT_AXIS_SIZE,
};


enum IndexingMode : uint8_t {
    INDEXING_MODE_SAFE = 0,
    INDEXING_MODE_UNCHECKED,
};

}


#endif //AUTO_RAY_GUI_ENUMS_HPP
