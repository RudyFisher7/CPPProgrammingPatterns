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

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "Raylib + RayGUI Example");

    float width = 100;
    float height = 40;

    bool is_checked = false;
    while (!WindowShouldClose()) // Main loop
    {
        // Update logic (if needed)

        BeginDrawing();
        ClearBackground(BLACK);

        // Draw GUI elements
        if (AutoRayGui::ExpandContainer(&GuiPanel, 0.0f, 0.0f, "An expanded panel!"))
        {
            // Button was clicked - handle event
        }

        if (AutoRayGui::CenterContainer(&GuiCheckBox, width, height, "Check Me!", &is_checked))
        {
            // Button was clicked - handle event
        }


        AutoRayGui::ContainerLayout vbox_layout{{0.0f, 0.0f, 160.0f, 200.0f}, 4u, 0u, {0.0f, 0.0f}};

        AutoRayGui::ContainerChildLayout vbox_child_layout {
                {80.0f, 20.0f},
                AutoRayGui::SIZE_FLAGS_SHRINK_BEGIN,
        };

        std::string message = "Hello from vbox! Child: ";
//        for (size_t i = 0u; i < vbox_layout.child_count; ++i) {
//            if (AutoRayGui::VBoxChild<AutoRayGui::SIZE_FLAGS_EXPAND>(vbox_layout, vbox_child_layout, &GuiLabel, (message + std::to_string(i)).c_str())) {
//                //
//            }
//        }

        if (AutoRayGui::VBoxChild<AutoRayGui::SIZE_FLAGS_EXPAND>(vbox_layout, vbox_child_layout, &GuiLabel, (message + std::to_string(0)).c_str())) {
            //
        }

        if (AutoRayGui::VBoxChild<AutoRayGui::SIZE_FLAGS_VERTICAL_EXPAND>(vbox_layout, vbox_child_layout, &GuiLabel, (message + std::to_string(0)).c_str())) {
            //
        }

        if (AutoRayGui::VBoxChild<AutoRayGui::SIZE_FLAGS_HORIZONTAL_SHRINK_CENTER>(vbox_layout, vbox_child_layout, &GuiLabel, (message + std::to_string(0)).c_str())) {
            //
        }

        if (AutoRayGui::VBoxChild<AutoRayGui::SIZE_FLAGS_HORIZONTAL_SHRINK_END>(vbox_layout, vbox_child_layout, &GuiLabel, (message + std::to_string(0)).c_str())) {
            //
        }


        EndDrawing();
    }

    CloseWindow();

    return 0;
}