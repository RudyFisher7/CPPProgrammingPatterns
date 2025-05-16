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


#include "autoraygui/container.hpp"


#include <raylib.h>

#ifndef RAYGUI_IMPLEMENTATION
#define RAYGUI_IMPLEMENTATION
#endif
#include <raygui.h>

#include <string>


int main() {

    AutoRayGui::GUI<8u, AutoRayGui::INDEXING_MODE_SAFE>& gui = AutoRayGui::GUI<8u, AutoRayGui::INDEXING_MODE_SAFE>::get_singleton();
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "Raylib + RayGUI Example");

    float width = 100;
    float height = 40;

    bool is_checked = false;
    while (!WindowShouldClose()) // Main loop
    {
        // Update logic (if needed)
        BeginDrawing();
        int current_id = gui.BeginDrawing();
        ClearBackground(BLACK);

        Vector2 m = MeasureTextEx(GuiGetFont(), "Hello Free Child!", GetFontDefault().baseSize, 2.0f);
        int current_child_id = gui.Begin<AutoRayGui::CONTAINER_TYPE_CENTER>(current_id, m.x, m.y);
        std::pair<int, int> result {current_child_id, 0};
        result = gui.Child<AutoRayGui::CONTAINER_TYPE_CENTER>(result.first, current_id, &GuiButton, "Hello Btn  Child!");
        result = gui.Child<AutoRayGui::CONTAINER_TYPE_CENTER>(result.first, current_id, &GuiLabel, "Hello Free Child!");
        current_child_id = result.first;
        gui.End(current_id);
        current_id = current_child_id;

        gui.EndDrawing();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}