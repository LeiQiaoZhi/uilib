//
// Created by Qiaozhi Lei on 4/29/25.
//

#include <iostream>

#include "ui.h"
#include <raylib.h>
#include <editor/Editor.h>

int RAYLIB_FONT_SIZE = 20;

void Raylib_DrawText(const char *str, const int x, const int y, float scale) {
    const Vector2 pos = {static_cast<float>(x), static_cast<float>(y)};
    DrawTextEx(Editor::GetFont(), str, pos, RAYLIB_FONT_SIZE * scale, 1.0, WHITE);
}

float Raylib_MeasureText(const char *str, float scale) {
    return MeasureTextEx(Editor::GetFont(), str, RAYLIB_FONT_SIZE * scale, 1.0f).x;
}

float Raylib_MeasureTextHeight(const char *str, float scale) {
    return MeasureTextEx(Editor::GetFont(), str, RAYLIB_FONT_SIZE * scale, 1.0f).y;
}

bool Raylib_IsMousePressed() {
    return IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

std::array<float, 2> Raylib_GetMousePos() {
    const Vector2 pos = GetMousePosition();
    return {pos.x, pos.y};
}

void UI_Raylib_Init() {
    UI::getMousePosFn = &Raylib_GetMousePos;
    UI::isMousePressedFn = &Raylib_IsMousePressed;
    UI::drawTextFn = &Raylib_DrawText;
    UI::measureTextFn = &Raylib_MeasureText;
    UI::measureTextHeightFn = &Raylib_MeasureTextHeight;
}
