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


#ifndef AUTO_RAY_GUI_CONTROL_HPP
#define AUTO_RAY_GUI_CONTROL_HPP


#include "std_extensions/extensions.hpp"

#include <raylib.h>

#include <cstdint>
#include <cmath>
#include <functional>


namespace AutoRayGui {

typedef struct control_t {
    std::function<void(Rectangle)> draw_function;
} Control;


void passthrough_draw_command(Rectangle bounds){}

template<typename ... TArgs>
std::function<void(Rectangle)> build_ray_gui(int& out_return, int(*func)(Rectangle, TArgs...), TArgs ... args) {
    return [&out_return, &func, &args...](Rectangle bounds) -> void {
        out_return = func(std::forward<Rectangle>(bounds), std::forward<TArgs>(args) ...);
    };
}

// pixel drawing
template<typename ... TArgs>
std::function<void(Rectangle)> build_raylib_draw_pixel(void(*func)(int, int, TArgs...), TArgs ... args) {
    return [&func, &args...](Rectangle bounds) -> void {
        Vector2 center {
                bounds.x + (bounds.width / 2.0f),
                bounds.y + (bounds.height / 2.0f),
        };

        func(center.x, center.y, std::forward<TArgs>(args) ...);
    };
}

template<typename ... TArgs>
std::function<void(Rectangle)> build_raylib_draw_pixel(void(*func)(Vector2, TArgs...), TArgs ... args) {
    return [&func, &args...](Rectangle bounds) -> void {
        Vector2 center {
                bounds.x + (bounds.width / 2.0f),
                bounds.y + (bounds.height / 2.0f),
        };

        func(center, std::forward<TArgs>(args) ...);
    };
}

// line drawing
template<typename ... TArgs>
std::function<void(Rectangle)> build_raylib_draw_line(void(*func)(int, int, int, int, TArgs...), TArgs ... args) {
    return [&func, &args...](Rectangle bounds) -> void {
        func(bounds.x, bounds.y, bounds.x + bounds.width, bounds.y + bounds.height, std::forward<TArgs>(args) ...);
    };
}

template<typename ... TArgs>
std::function<void(Rectangle)> build_raylib_draw_line(void(*func)(Vector2, Vector2, TArgs...), TArgs ... args) {
    return [&func, &args...](Rectangle bounds) -> void {
        func({bounds.x, bounds.y}, {bounds.x + bounds.width, bounds.y + bounds.height}, std::forward<TArgs>(args) ...);
    };
}

// line segments from points drawing
template<int point_count, typename ... TArgs>
std::function<void(Rectangle)> build_raylib_draw_line(void(*func)(const Vector2*, int, TArgs...), Vector2 points[point_count], TArgs ... args) {
    return [&func, points, &args...](Rectangle bounds) -> void {
        Vector2 absolute_points[point_count];
        for (int i = 0; i < point_count; ++i) {
            absolute_points[i].x = bounds.x + (points[i].x * bounds.width);
            absolute_points[i].y = bounds.y + (points[i].y * bounds.height);
        }

        func(absolute_points, point_count, std::forward<TArgs>(args) ...);
    };
}

// circle drawing
template<typename ... TArgs>
std::function<void(Rectangle)> build_raylib_draw_circle(void(*func)(int, int, float, TArgs...), float radius, TArgs ... args) {
    return [&func, radius, &args...](Rectangle bounds) -> void {
        Vector2 center {
                bounds.x + (bounds.width / 2.0f),
                bounds.y + (bounds.height / 2.0f),
        };

        float absolute_radius = radius * (fminf(bounds.width, bounds.height) / 2.0f);//todo::allow for choosing min or max functions

        func(center.x, center.y, absolute_radius, std::forward<TArgs>(args) ...);
    };
}

template<typename ... TArgs>
std::function<void(Rectangle)> build_raylib_draw_circle(void(*func)(Vector2, float, TArgs...), float radius, TArgs ... args) {
    return [&func, radius, &args...](Rectangle bounds) -> void {
        Vector2 center {
                bounds.x + (bounds.width / 2.0f),
                bounds.y + (bounds.height / 2.0f),
        };

        float absolute_radius = radius * (fminf(bounds.width, bounds.height) / 2.0f);

        func(center, absolute_radius, std::forward<TArgs>(args) ...);
    };
}

// ellipse drawing
template<typename ... TArgs>
std::function<void(Rectangle)> build_raylib_draw_ellipse(void(*func)(int, int, float, float, TArgs...), float radius_h, float radius_v, TArgs ... args) {
    return [&func, radius_h, radius_v, &args...](Rectangle bounds) -> void {
        float center_h = (bounds.width / 2.0f);
        float center_v = (bounds.height / 2.0f);
        Vector2 center {
                bounds.x + center_h,
                bounds.y + center_v,
        };

        float absolute_radius_h = radius_h * center_h;
        float absolute_radius_v = radius_v * center_v;

        func(center.x, center.y, absolute_radius_h, absolute_radius_v, std::forward<TArgs>(args) ...);
    };
}

// ring drawing todo:: pickup here

}


#endif //AUTO_RAY_GUI_CONTROL_HPP
