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


#include "koi_object/object.hpp"
#include "koi_object/string_name.hpp"
#include "koi_object_editor/editor_demo_object.hpp"

//#include <raylib.h>
//
//#define RAYGUI_IMPLEMENTATION
//#include <raygui.h>

#if defined(__cplusplus)
#pragma message("cpp defined")
#endif
#if defined(__STDC__)
#pragma message("stdc defined")
#endif
#if defined(__STDC_VERSION__)
#pragma message("stdc version defined")
#endif

#if (defined(__cplusplus) && __cplusplus >= 202002L)
#pragma message("cpp 20")
#endif
#if (defined(__STDC__) && __STDC__ == 1 && defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L)
#pragma message("stdc 1 i guess")
#endif

#define CLAY_IMPLEMENTATION
#include <clay.h>
#include <../../clay-h/include/clay_renderer_raylib.c>


void HandleClayErrors(Clay_ErrorData errorData) {
    // See the Clay_ErrorData struct for more information
    printf("%s", errorData.errorText.chars);
    switch(errorData.errorType) {
        // etc
        case CLAY_ERROR_TYPE_TEXT_MEASUREMENT_FUNCTION_NOT_PROVIDED:
            break;
        case CLAY_ERROR_TYPE_ARENA_CAPACITY_EXCEEDED:
            break;
        case CLAY_ERROR_TYPE_ELEMENTS_CAPACITY_EXCEEDED:
            break;
        case CLAY_ERROR_TYPE_TEXT_MEASUREMENT_CAPACITY_EXCEEDED:
            break;
        case CLAY_ERROR_TYPE_DUPLICATE_ID:
            break;
        case CLAY_ERROR_TYPE_FLOATING_CONTAINER_PARENT_NOT_FOUND:
            break;
        case CLAY_ERROR_TYPE_PERCENTAGE_OVER_1:
            break;
        case CLAY_ERROR_TYPE_INTERNAL_ERROR:
            break;
        default:
            break;
    }
}


int main(int argc, char* argv[]) {
//    const Koi::StringName OBJECT{"Object"};
//    const Koi::StringName MY_BOOL{"my_bool"};
//    const Koi::StringName MY_CHAR{"my_char"};
//    const Koi::StringName MY_INT{"my_int"};
//    const Koi::StringName MY_FLOAT{"my_float"};
//    const Koi::StringName EDITOR_DEMO_OBJECT{"EditorDemoObject"};

//    Koi::Object::register_object_class<Koi::Object>(OBJECT);
//    Koi::Object::register_factory_method(OBJECT, []() -> Koi::Object* { return new Koi::Object(); });
//    Koi::Object::register_object_class<Koi::EditorDemoObject>(EDITOR_DEMO_OBJECT);
//    Koi::Object::register_factory_method(EDITOR_DEMO_OBJECT, []() -> Koi::Object* { return new Koi::EditorDemoObject(); });

    Koi::EditorDemoObject object;

    Clay_Raylib_Initialize(1024, 768, "Introducing Clay Demo", FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI | FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);

    bool button_clicked = false;
    bool is_toggle_active = true;
    bool is_value_box_editable = false;
    bool is_spinner_editable = false;
    int value = 0;
    float fvalue = 0.0f;
    char str[4u];
    char* str_str = str;
    bool for_text_input = true;
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

//        GuiPanel({ 200, 200, 400, 400 }, "Edit");
//        if (GuiButton({ 350, 250, 100, 40 }, "Click Me")) {
//            button_clicked = !button_clicked;
//        }
//
//        GuiToggle({ 350, 290, 100, 40 }, "Toggle Me", &is_toggle_active);
//
//        if (button_clicked) {
//            DrawText("Button clicked!", 360, 300, 20, RED);
//        }
//
//        if (GuiValueBox({ 350, 330, 100, 40 }, "Value Me", &value, -200, 200, is_value_box_editable)) {
//            is_value_box_editable = !is_value_box_editable;
//        }
//
//        if (GuiSpinner({ 350, 370, 100, 40 }, "Spinner Me", &value, -200, 200, is_spinner_editable)) {
//            is_spinner_editable = !is_spinner_editable;
//        }
//
//        if (GuiSlider({ 350, 410, 100, 40 }, "Left", "Right", &fvalue, -200.0f, 200.0f)) {}
//
//        if (GuiTextInputBox({ 350, 450, 160, 110 }, "Input Text", "Message", "Submit", str_str, 4, &for_text_input)) {}

        EndDrawing();
    }

    CloseWindow();


    return 0;
}
