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

#include <cctype>
#include <cstdint>
#include <cmath>
#include <functional>


namespace AutoRayGui {

typedef std::function<void(Rectangle)> DrawFunction;

typedef struct control_t {
    std::function<void(Rectangle)> draw;
} Control;


void draw_passthrough(Rectangle _bounds){
    std::ignore = _bounds;
}

template<typename ... TArgs>
std::function<void(Rectangle)> build_ray_gui(int& out_return, int(*func)(Rectangle, TArgs...), TArgs ... args) {
    return [&out_return, func, args...](Rectangle bounds) -> void {
        out_return = func(std::forward<Rectangle>(bounds), args ...);
    };
}

// pixel drawing
template<typename ... TArgs>
std::function<void(Rectangle)> build_raylib_draw_pixel(void(*func)(int, int, TArgs...), TArgs ... args) {
    return [func, args...](Rectangle bounds) -> void {
        Vector2 center {
                bounds.x + (bounds.width / 2.0f),
                bounds.y + (bounds.height / 2.0f),
        };

        func(center.x, center.y, args ...);
    };
}

template<typename ... TArgs>
std::function<void(Rectangle)> build_raylib_draw_pixel(void(*func)(Vector2, TArgs...), TArgs ... args) {
    return [func, args...](Rectangle bounds) -> void {
        Vector2 center {
                bounds.x + (bounds.width / 2.0f),
                bounds.y + (bounds.height / 2.0f),
        };

        func(center, args ...);
    };
}

// line drawing
template<typename ... TArgs>
std::function<void(Rectangle)> build_raylib_draw_line(void(*func)(int, int, int, int, TArgs...), TArgs ... args) {
    return [func, args...](Rectangle bounds) -> void {
        func(bounds.x, bounds.y, bounds.x + bounds.width, bounds.y + bounds.height, args ...);
    };
}

template<typename ... TArgs>
std::function<void(Rectangle)> build_raylib_draw_line(void(*func)(Vector2, Vector2, TArgs...), TArgs ... args) {
    return [func, args...](Rectangle bounds) -> void {
        func({bounds.x, bounds.y}, {bounds.x + bounds.width, bounds.y + bounds.height}, args ...);
    };
}

// line segments from points drawing
template<int point_count, typename ... TArgs>
std::function<void(Rectangle)> build_raylib_draw_line(void(*func)(const Vector2*, int, TArgs...), Vector2 points[point_count], TArgs ... args) {
    return [func, points, args...](Rectangle bounds) -> void {
        Vector2 absolute_points[point_count];
        for (int i = 0; i < point_count; ++i) {
            absolute_points[i].x = bounds.x + (points[i].x * bounds.width);
            absolute_points[i].y = bounds.y + (points[i].y * bounds.height);
        }

        func(absolute_points, point_count, args ...);
    };
}

// circle drawing
template<typename ... TArgs>
std::function<void(Rectangle)> build_raylib_draw_circle(void(*func)(int, int, float, TArgs...), float radius, TArgs ... args) {
    return [func, radius, args...](Rectangle bounds) -> void {
        Vector2 center {
                bounds.x + (bounds.width / 2.0f),
                bounds.y + (bounds.height / 2.0f),
        };

        float absolute_radius = radius * (fminf(bounds.width, bounds.height) / 2.0f);//todo::allow for choosing min or max functions

        func(center.x, center.y, absolute_radius, args ...);
    };
}

template<typename ... TArgs>
std::function<void(Rectangle)> build_raylib_draw_circle(void(*func)(Vector2, float, TArgs...), float radius, TArgs ... args) {
    return [func, radius, args...](Rectangle bounds) -> void {
        Vector2 center {
                bounds.x + (bounds.width / 2.0f),
                bounds.y + (bounds.height / 2.0f),
        };

        float absolute_radius = radius * (fminf(bounds.width, bounds.height) / 2.0f);

        func(center, absolute_radius, args ...);
    };
}

// ellipse drawing
template<typename ... TArgs>
std::function<void(Rectangle)> build_raylib_draw_ellipse(void(*func)(int, int, float, float, TArgs...), float radius_h, float radius_v, TArgs ... args) {
    return [func, radius_h, radius_v, args...](Rectangle bounds) -> void {
        float half_bounds_width = (bounds.width / 2.0f);
        float half_bounds_height = (bounds.height / 2.0f);
        Vector2 center {
                bounds.x + half_bounds_width,
                bounds.y + half_bounds_height,
        };

        float absolute_radius_h = radius_h * half_bounds_width;
        float absolute_radius_v = radius_v * half_bounds_height;

        func(center.x, center.y, absolute_radius_h, absolute_radius_v, args ...);
    };
}

// ring drawing
template<typename ... TArgs>
std::function<void(Rectangle)> build_raylib_draw_ring(void(*func)(Vector2, float, float, float, float, int, TArgs...), float inner_radius, float outer_radius, float start_angle, float end_angle, int segments, TArgs ... args) {
    return [func, inner_radius, outer_radius, start_angle, end_angle, segments, args...](Rectangle bounds) -> void {
        Vector2 center {
                bounds.x + (bounds.width / 2.0f),
                bounds.y + (bounds.height / 2.0f),
        };

        float radius_multiplier = fminf(bounds.width, bounds.height) / 2.0f;
        float absolute_inner_radius = inner_radius * radius_multiplier;
        float absolute_outer_radius = outer_radius * radius_multiplier;

        func(center, absolute_inner_radius, absolute_outer_radius, start_angle, end_angle, segments, args ...);
    };
}

// rectangle drawing
template<typename ... TArgs>
std::function<void(Rectangle)> build_raylib_draw_rectangle(void(*func)(int, int, int, int, TArgs...), TArgs ... args) {
    return [func, args...](Rectangle bounds) -> void {
        func(bounds.x, bounds.y, bounds.width, bounds.height, args ...);
    };
}

template<typename ... TArgs>
std::function<void(Rectangle)> build_raylib_draw_rectangle(void(*func)(Rectangle, TArgs...), TArgs ... args) {
    return [func, args...](Rectangle bounds) -> void {
        func(bounds, args ...);
    };
}

// triangle drawing todo::
// polygon drawing todo::
// spline drawing todo::

// texture drawing todo:: test this and make the rest
template<typename ... TArgs>
std::function<void(Rectangle)> build_raylib_draw_texture(void(*func)(Texture2D, Vector2, float, float, TArgs...), Texture2D texture, float rotation, TArgs ... args) {
    return [func, texture, rotation, args...](Rectangle bounds) -> void {
        Vector2 position {
                bounds.x,
                bounds.y,
        };


        float scale_w = texture.width / fmaxf(bounds.width, 1.0f);
        float scale_h = texture.height / fmaxf(bounds.height, 1.0f);
        float scale = fminf(scale_w, scale_h);

        func(texture, position, rotation, scale, args ...);
    };
}

// text drawing
std::function<void(Rectangle)> build_raylib_draw_fps(void(*func)(int, int)) {
    return [func](Rectangle bounds) -> void {
        func(bounds.x, bounds.y);
    };
}

std::function<void(Rectangle)> build_raylib_draw_text(void(*func)(const char*, int, int, int, Color), const char* text, int font_size, Color color) {
    return [func, text, font_size, color](Rectangle bounds) -> void {
        func(text, bounds.x, bounds.y, font_size, color);
    };
}

template<typename ... TArgs>
std::function<void(Rectangle)> build_raylib_draw_text(void(*func)(Font, const char*, Vector2, TArgs...), Font font, const char* text, TArgs ... args) {
    return [func, font, text, args...](Rectangle bounds) -> void {
        func(font, text, (Vector2){bounds.x, bounds.y}, args ...);
    };
}

std::function<void(Rectangle)> build_raylib_draw_wrapped_text(Font font, const int* codepoints, int codepoint_count, int line_spacing, Color tint) {
    return [font, codepoints, codepoint_count, line_spacing, tint](Rectangle bounds) -> void {
        // fixme:: this algorithm only works when all codepoints are 1 byte/char. utf-8 uses variable length codepoints
        const int* current_codepoints = codepoints;
        int i = 0;
        Vector2 current_position = {bounds.x, bounds.y};
        int last_white_space_i = codepoint_count;
        float width_up_to_last_white_space = 0.0f;
        float current_width = 0.0f;
        while (i < codepoint_count) {
            // find the last white space codepoint within the bounds' width
            while (i < codepoint_count && current_width < bounds.width) {
                int codepoint = codepoints[i];
                int glyph_index = GetGlyphIndex(font, codepoint);
                float glyph_width = font.recs[glyph_index].width;
                if (
                        codepoint == 0x0020
                        || codepoint == 0x0009
                        || codepoint == 0x000A
                        || codepoint == 0x000D
                        || codepoint == 0x00A0
                ) {
                    last_white_space_i = i;
                    width_up_to_last_white_space = current_width;
                }

                current_width += glyph_width;
                ++i;
            }

            DrawTextCodepoints(font, current_codepoints, i - last_white_space_i, current_position, font.baseSize, 0.0f, tint);

            current_position.y += font.baseSize + line_spacing;

            current_codepoints = codepoints + (last_white_space_i + 1);
            i = last_white_space_i + 1;
            last_white_space_i = codepoint_count;
            current_width = 0.0f;
        }
    };
}

}


#endif //AUTO_RAY_GUI_CONTROL_HPP
