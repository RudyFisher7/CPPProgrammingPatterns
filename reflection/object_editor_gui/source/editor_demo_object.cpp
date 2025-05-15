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


#include "koi_object_editor/editor_demo_object.hpp"


namespace Koi {

EditorDemoObject::EditorDemoObject() :
        my_bool(false),
        my_char('A'),
        my_int(0),
        my_float(0.0f),
        Object({
               {"my_bool", my_bool},
               {"my_char", my_char},
               {"my_int", my_int},
               {"my_float", my_float},
        }) {}

EditorDemoObject::EditorDemoObject(
        bool in_my_bool,
        char in_my_char,
        int in_my_int,
        float in_my_float
) :
        my_bool(in_my_bool),
        my_char(in_my_char),
        my_int(in_my_int),
        my_float(in_my_float),
        Object({
               {"my_bool", my_bool},
               {"my_char", my_char},
               {"my_int", my_int},
               {"my_float", my_float},
        }) {}

} // Koi