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


#include "autoraygui/gui_tree.hpp"

#include "autoraygui/control.hpp"

#include <raylib.h>

#ifndef RAYGUI_IMPLEMENTATION
#define RAYGUI_IMPLEMENTATION
#endif
#include <raygui.h>


using namespace AutoRayGui;


int main() {
    GuiTree<128u, AutoRayGui::INDEXING_MODE_SAFE> tree;
    const int point_count = 5;
    Vector2 points[point_count] = {
            {0.0f, 1.0f},
            {0.12f, 0.1f},
            {0.4f, 0.2f},
            {0.6f, 0.2f},
            {1.0f, 0.5f},
    };

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "Raylib + RayGUI Example");

    Control control;

    int pressed = 0;
    control.draw = build_ray_gui(pressed, &GuiButton, "Hello!");
//    control.draw = build_raylib_draw_ellipse(&DrawEllipse, 1.0f, 1.0f, RAYWHITE);

    const char* text_to_wrap = "Hello Text! This is some texty-text for texting the text.";

    tree.BeginRoot()
        ->SetChildAlignment({AutoRayGui::CHILD_ALIGNMENT_CENTER, AutoRayGui::CHILD_ALIGNMENT_CENTER})
        ->SetChildLayoutAxis(AutoRayGui::CHILD_LAYOUT_AXIS_X)
        ->SetChildSpacing(32.0f)
        ->Begin()
            ->SetSizeFlags({AutoRayGui::SIZE_FLAGS_FIXED, AutoRayGui::SIZE_FLAGS_FIXED})
            ->SetSize({100.0f, 100.0f})
            ->SetMaxSizeX(MeasureText("texty-text", GetFontDefault().baseSize))
            ->SetText(text_to_wrap, strlen(text_to_wrap), GetFontDefault().baseSize, GetFontDefault().baseSize)
            ->SetDrawFunc(AutoRayGui::build_raylib_draw_wrapped_text(GetFontDefault(), text_to_wrap, strlen(text_to_wrap), GetFontDefault().baseSize, RAYWHITE))
        ->End();

        for (int i = 0; i < 2; ++i) {
            tree.Begin()
                    ->SetChildAlignment({AutoRayGui::CHILD_ALIGNMENT_CENTER, AutoRayGui::CHILD_ALIGNMENT_CENTER})
                    ->SetSizeFlags({AutoRayGui::SIZE_FLAGS_GROW, AutoRayGui::SIZE_FLAGS_GROW})
                    ->SetMaxSize({FLT_MAX, 60.0f})
                    ->SetDrawFunc(AutoRayGui::build_raylib_draw_ellipse(&DrawEllipse, 1.0, 1.0, SKYBLUE))
                    ->Begin()
                    ->SetSizeFlags({AutoRayGui::SIZE_FLAGS_FIXED, AutoRayGui::SIZE_FLAGS_FIXED})
                    ->SetSize({100.0f, 20.0f})
                    ->SetDrawFunc(AutoRayGui::build_ray_gui(pressed, &GuiButton, "Hello!"))
                    ->End()
                    ->Begin()
                    ->SetSizeFlags({AutoRayGui::SIZE_FLAGS_FIXED, AutoRayGui::SIZE_FLAGS_FIXED})
                    ->SetSize({120.0f, 30.0f})
                    ->SetDrawFunc(AutoRayGui::build_ray_gui(pressed, &GuiButton, "Hello Again!"))
                ->End()
            ->End();
        }

    tree.Begin()
            ->SetChildAlignment({AutoRayGui::CHILD_ALIGNMENT_CENTER, AutoRayGui::CHILD_ALIGNMENT_CENTER})
            ->SetSizeFlags({AutoRayGui::SIZE_FLAGS_GROW, AutoRayGui::SIZE_FLAGS_GROW})
            ->SetDrawFunc(AutoRayGui::build_raylib_draw_ellipse(&DrawEllipse, 1.0, 1.0, SKYBLUE))
            ->Begin()
            ->SetSizeFlags({AutoRayGui::SIZE_FLAGS_FIXED, AutoRayGui::SIZE_FLAGS_FIXED})
            ->SetSize({100.0f, 20.0f})
            ->SetDrawFunc(AutoRayGui::build_ray_gui(pressed, &GuiButton, "Hello!"))
            ->End()
            ->Begin()
            ->SetSizeFlags({AutoRayGui::SIZE_FLAGS_FIXED, AutoRayGui::SIZE_FLAGS_FIXED})
            ->SetSize({120.0f, 30.0f})
                ->SetDrawFunc(AutoRayGui::build_ray_gui(pressed, &GuiButton, "Hello Again!"))
            ->End()
        ->End();
    tree.EndRoot();

    while (!WindowShouldClose()) // Main loop
    {
        Rectangle bounds {
                0.0f,
                0.0f,
                200.0f,
                60.0f,
        };

        tree.UpdateLayout();

        BeginDrawing();
        ClearBackground(BLACK);

        tree.Draw();

//        control.draw(bounds);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}