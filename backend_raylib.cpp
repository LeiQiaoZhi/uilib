//
// Created by Qiaozhi Lei on 4/29/25.
//

#include <iostream>

#include "ui.h"
#include <raylib.h>
#include <editor/Editor.h>

int RAYLIB_FONT_SIZE = 20;

void Raylib_DrawText(const char *str, const int x, const int y, const float scale) {
    const Vector2 pos = {static_cast<float>(x), static_cast<float>(y)};
    DrawTextEx(Editor::GetFont(), str, pos, RAYLIB_FONT_SIZE * scale, 1.0, WHITE);
}

float Raylib_MeasureText(const char *str, const float scale) {
    return MeasureTextEx(Editor::GetFont(), str, RAYLIB_FONT_SIZE * scale, 1.0f).x;
}

float Raylib_MeasureTextHeight(const char *str, const float scale) {
    return MeasureTextEx(Editor::GetFont(), str, RAYLIB_FONT_SIZE * scale, 1.0f).y;
}

bool Raylib_IsMousePressed() {
    return IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

void Raylib_DrawRectangle(const int x, const int y, const int w, const int h, const std::array<int, 4> color) {
    const Color c = {
        static_cast<unsigned char>(color[0]),
        static_cast<unsigned char>(color[1]),
        static_cast<unsigned char>(color[2]),
        static_cast<unsigned char>(color[3])
    };
    DrawRectangle(x, y, w, h, c);
}

std::array<float, 2> Raylib_GetMousePos() {
    const Vector2 pos = GetMousePosition();
    return {pos.x, pos.y};
}

void UI_Raylib_Init() {
    UI::getMousePosFn = &Raylib_GetMousePos;
    UI::isMousePressedFn = &Raylib_IsMousePressed;
    UI::drawTextFn = &Raylib_DrawText;
    UI::drawRectFn = &Raylib_DrawRectangle;
    UI::measureTextFn = &Raylib_MeasureText;
    UI::measureTextHeightFn = &Raylib_MeasureTextHeight;
}
