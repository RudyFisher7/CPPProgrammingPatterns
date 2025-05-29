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


#include "autoraygui/control.hpp"

#include <raylib.h>

#ifndef RAYGUI_IMPLEMENTATION
#define RAYGUI_IMPLEMENTATION
#endif
#include <raygui.h>


using namespace AutoRayGui;


int main() {
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

//    control.draw = build_raylib_draw_pixel(&DrawPixel, RAYWHITE);
//    control.draw = build_raylib_draw_pixel(&DrawPixelV, RAYWHITE);
//    control.draw = build_raylib_draw_line(&DrawLine, RAYWHITE);
//    control.draw = build_raylib_draw_line(&DrawLineV, RAYWHITE);
//    control.draw = build_raylib_draw_line(&DrawLineEx, 3.8f, RAYWHITE);
//    control.draw = build_raylib_draw_line<point_count, Color>(&DrawLineStrip, points, RAYWHITE);
//    control.draw = build_raylib_draw_line(&DrawLineBezier, 3.8f, RAYWHITE);
//    control.draw = build_raylib_draw_circle(&DrawCircle, 1.0f, RAYWHITE);
//    control.draw = build_raylib_draw_circle(&DrawCircleSector, 1.0f, 0.0f, 45.0f, 6, RAYWHITE);
//    control.draw = build_raylib_draw_circle(&DrawCircleSectorLines, 1.0f, 0.0f, 45.0f, 6, RAYWHITE);
//    control.draw = build_raylib_draw_circle(&DrawCircleGradient, 3.8f, BLACK, RAYWHITE);
//    control.draw = build_raylib_draw_circle(&DrawCircleV, 1.0f, RAYWHITE);
//    control.draw = build_raylib_draw_circle(&DrawCircleLines, 1.0f, RAYWHITE);
//    control.draw = build_raylib_draw_circle(&DrawCircleLinesV, 1.0f, RAYWHITE);
//    control.draw = build_raylib_draw_ellipse(&DrawEllipse, 1.0f, 1.0f, RAYWHITE);
//    control.draw = build_raylib_draw_ellipse(&DrawEllipseLines, 1.0f, 1.0f, RAYWHITE);

    while (!WindowShouldClose()) // Main loop
    {

        Rectangle bounds {
            0.0f,
            0.0f,
            200.0f,
            60.0f,
        };

        BeginDrawing();
        ClearBackground(BLACK);

        control.draw(bounds);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}