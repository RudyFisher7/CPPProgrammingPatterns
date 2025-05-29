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

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>

#include <iostream>
#include <string>


// RAYGUI

int TestGuiFuncP0(Rectangle bounds) {
    return 1;
}

int TestGuiFuncP1(Rectangle bounds, const char* text) {
    std::cout << text << std::endl;
    return 1;
}

int TestGuiFuncP1(Rectangle bounds, int value) {
    return value;
}

int TestGuiFuncP2(Rectangle bounds, const char* text, int value) {
    std::cout << text << std::endl;
    return value;
}

int TestGuiFuncP2(Rectangle bounds, const char* text, int* pressed) {
    std::cout << text << std::endl;
    *pressed = 1;
    return 1;
}

int TestGuiFuncP4(Rectangle bounds, const char* text, const char* message, int* pressed, int* active) {
    std::cout << text << std::endl;
    std::cout << message << std::endl;
    *pressed = 1;
    *active = 1;
    return 1;
}


// RAYLIB DRAW LINES

void TestDrawLineFunc(int start_x, int start_y, int end_x, int end_y, Color color) {
    (void)start_x;
    (void)start_y;
    (void)end_x;
    (void)end_y;
    (void)color;
}

void TestDrawLineFuncV(Vector2 start, Vector2 end, Color color) {
    (void)start;
    (void)end;
    (void)color;
}

void TestDrawFromPointsFunc(const Vector2 *points, int point_count, Color color) {
    (void)points;
    (void)point_count;
    (void)color;
}


// RAYLIB DRAW CIRCLES

void TestDrawCircleFunc(int center_x, int center_y, float radius, Color color) {
    (void)center_x;
    (void)center_y;
    (void)radius;
    (void)color;
}

void TestDrawCircleFuncV(Vector2 center, float radius, Color color) {
    (void)center;
    (void)radius;
    (void)color;
}

void TestDrawCircleSectorFunc(Vector2 center, float radius, float start_angle, float end_angle, int segments, Color color) {
    (void)center;
    (void)radius;
    (void)start_angle;
    (void)end_angle;
    (void)segments;
    (void)color;
}


// RAYLIB DRAW CIRCLES




TEST_CASE("Control GuiFuncs", "[Control][RayGui]") {
    Rectangle bounds {0.0f, 0.0f, 100.0f, 20.0f};

    AutoRayGui::Control passthrough { &AutoRayGui::draw_passthrough };
    passthrough.draw(bounds);
    CHECK(true);


    AutoRayGui::Control reassigned { &AutoRayGui::draw_passthrough };
    reassigned.draw(bounds);
    REQUIRE(true);

    int r_reassigned = 0;
    reassigned.draw = AutoRayGui::build_ray_gui(r_reassigned, &TestGuiFuncP0);
    reassigned.draw(bounds);
    CHECK(r_reassigned == 1);


    int r0 = 0;
    AutoRayGui::Control c0 {
            AutoRayGui::build_ray_gui(r0, &TestGuiFuncP0)
    };

    c0.draw(bounds);
    CHECK(r0 == 1);


    int r1 = 0;
    AutoRayGui::Control c1 {
            AutoRayGui::build_ray_gui(r1, &TestGuiFuncP1, "Hello Draw Func!")
    };

    c1.draw(bounds);
    CHECK(r1 == 1);


    int r2 = 0;
    AutoRayGui::Control c2 {
            AutoRayGui::build_ray_gui(r2, &TestGuiFuncP1, 1)
    };

    c2.draw(bounds);
    CHECK(r2 == 1);


    int r3 = 0;
    int p3 = 8;
    AutoRayGui::Control c3 {
            AutoRayGui::build_ray_gui(r3, &TestGuiFuncP1, p3)
    };

    c3.draw(bounds);
    CHECK(r3 == p3);


    int r4 = 0;
    int p4 = 8;
    AutoRayGui::Control c4 {
            AutoRayGui::build_ray_gui(r4, &TestGuiFuncP2, "Hello P2!", p4)
    };

    c4.draw(bounds);
    CHECK(r4 == p4);


    int r5 = 0;
    int p5_pressed = 0;
    AutoRayGui::Control c5 {
            AutoRayGui::build_ray_gui(r5, &TestGuiFuncP2, "Hello P2!", &p5_pressed)
    };

    c5.draw(bounds);
    CHECK(r5 == 1);
    CHECK(p5_pressed == 1);


    int r6 = 0;
    std::string p6_text = "Hello P4!";
    std::string p6_message = "Message P4!";
    int p6_pressed = 0;
    int p6_active = 0;
    AutoRayGui::Control c6 {
            AutoRayGui::build_ray_gui(
                    r6,
                    &TestGuiFuncP4,
                    p6_text.c_str(),
                    p6_message.c_str(),
                    &p6_pressed,
                    &p6_active
            )
    };

    c6.draw(bounds);
    CHECK(r6 == 1);
    CHECK(p6_pressed == 1);
    CHECK(p6_active == 1);
}


// todo:: implement test draw functions for verifying bounds use inside functions.
TEST_CASE("Control DrawFuncs", "[Control][RayLib]") {
    Rectangle bounds {0.0f, 0.0f, 100.0f, 20.0f};
    Color color = RAYWHITE;

    AutoRayGui::Control c0 {
            AutoRayGui::build_raylib_draw_line(&TestDrawLineFunc, color)
    };

    c0.draw(bounds);


    AutoRayGui::Control c1 {
            AutoRayGui::build_raylib_draw_line(&TestDrawLineFuncV, color)
    };

    c1.draw(bounds);


    Vector2 points2[5] = {
            {0.0f, 1.0f},
            {0.12f, 0.1f},
            {0.4f, 0.2f},
            {0.6f, 0.2f},
            {1.0f, 0.5f},
    };

    AutoRayGui::Control c2 {
            AutoRayGui::build_raylib_draw_line<5, Color>(&TestDrawFromPointsFunc, points2, color)
    };

    c2.draw(bounds);


    AutoRayGui::Control c3 {
            AutoRayGui::build_raylib_draw_circle(
                    &TestDrawCircleFunc,
                    1.0f,
                    color
            )
    };

    c3.draw(bounds);


    AutoRayGui::Control c4 {
            AutoRayGui::build_raylib_draw_circle(
                    &TestDrawCircleFuncV,
                    1.0f,
                    color
            )
    };

    c4.draw(bounds);


    AutoRayGui::Control c5 {
            AutoRayGui::build_raylib_draw_circle(
                    &TestDrawCircleSectorFunc,
                    1.0f,
                    0.0f,
                    0.2f,
                    4,
                    color
            )
    };

    c5.draw(bounds);


    CHECK(true);
}


// This test is just to make sure the templates can be used with all of raylib's draw functions.
TEST_CASE("RayLib Draw Func", "[RayLib]") {
    using namespace AutoRayGui;

    const int point_count = 5;
    Vector2 points[point_count] = {
            {0.0f, 1.0f},
            {0.12f, 0.1f},
            {0.4f, 0.2f},
            {0.6f, 0.2f},
            {1.0f, 0.5f},
    };

    build_raylib_draw_pixel(&DrawPixel, RAYWHITE);
    build_raylib_draw_pixel(&DrawPixelV, RAYWHITE);
    build_raylib_draw_line(&DrawLine, RAYWHITE);
    build_raylib_draw_line(&DrawLineV, RAYWHITE);
    build_raylib_draw_line(&DrawLineEx, 3.8f, RAYWHITE);
    build_raylib_draw_line<point_count, Color>(&DrawLineStrip, points, RAYWHITE);
    build_raylib_draw_line(&DrawLineBezier, 3.8f, RAYWHITE);
    build_raylib_draw_circle(&DrawCircle, 3.8f, RAYWHITE);
    build_raylib_draw_circle(&DrawCircleSector, 3.8f, 0.0f, 0.3f, 3, RAYWHITE);
    build_raylib_draw_circle(&DrawCircleSectorLines, 3.8f, 0.0f, 0.3f, 3, RAYWHITE);
    build_raylib_draw_circle(&DrawCircleGradient, 3.8f, BLACK, RAYWHITE);
    build_raylib_draw_circle(&DrawCircleV, 1.0f, RAYWHITE);
    build_raylib_draw_circle(&DrawCircleLines, 1.0f, RAYWHITE);
    build_raylib_draw_circle(&DrawCircleLinesV, 1.0f, RAYWHITE);
    build_raylib_draw_ellipse(&DrawEllipse, 1.0f, 1.0f, RAYWHITE);
    build_raylib_draw_ellipse(&DrawEllipseLines, 1.0f, 1.0f, RAYWHITE);
    build_raylib_draw_ring(&DrawRing, 0.5f, 1.0f, 0.0f, 0.5f, 3, RAYWHITE);
    build_raylib_draw_ring(&DrawRingLines, 0.5f, 1.0f, 0.0f, 0.5f, 3, RAYWHITE);
    build_raylib_draw_rectangle(&DrawRectangleRec, RAYWHITE);
    build_raylib_draw_rectangle(&DrawRectanglePro, {0.5f, 0.5f}, 0.5f, RAYWHITE);
    build_raylib_draw_rectangle(&DrawRectangleGradientV, SKYBLUE, RAYWHITE);
    build_raylib_draw_rectangle(&DrawRectangleGradientH, SKYBLUE, RAYWHITE);
    build_raylib_draw_rectangle(&DrawRectangleGradientEx, RED, YELLOW, BLUE, RAYWHITE);
    build_raylib_draw_rectangle(&DrawRectangleLines, RAYWHITE);
    build_raylib_draw_rectangle(&DrawRectangleRounded, 10.0f, 3, RAYWHITE);
    build_raylib_draw_rectangle(&DrawRectangleRounded, 10.0f, 3, RAYWHITE);
    build_raylib_draw_rectangle(&DrawRectangleRoundedLinesEx, 10.0f, 3, 4.0f, RAYWHITE);
    build_raylib_draw_texture(&DrawTextureEx, (Texture2D){}, 10.0f, RAYWHITE);//fixme:: use a real texture
    build_raylib_draw_fps(&DrawFPS);
    build_raylib_draw_text(&DrawText, "Hello Text!", GetFontDefault().baseSize, RAYWHITE);
    build_raylib_draw_text(&DrawTextEx, GetFontDefault(),  "Hello Text!", (float)GetFontDefault().baseSize, 4.0f, RAYWHITE);

    CHECK(true);
}


int main(int argc, char* argv[]) {
    // your setup ...

    int result = Catch::Session().run(argc, argv);

    // your clean-up...

    return result;
}
