//
// Created by Qiaozhi Lei on 4/29/25.
//

#include <iostream>

#include "ui.h"
#include <raylib.h>

int RAYLIB_FONT_SIZE = 20;

void Raylib_DrawText(const char *str, const int x, const int y, float scale) {
    DrawText(str, x, y, RAYLIB_FONT_SIZE * scale, WHITE);
}

float Raylib_MeasureText(const char *str, float scale) {
    return MeasureText(str, RAYLIB_FONT_SIZE * scale);
}

float Raylib_MeasureTextHeight(const char *str, float scale) {
    return MeasureTextEx(GetFontDefault(), str, RAYLIB_FONT_SIZE * scale, 1.0f).y;
}

void UI_Raylib_Init() {
    UI::drawTextFn = &Raylib_DrawText;
    UI::measureTextFn = &Raylib_MeasureText;
    UI::measureTextHeightFn = &Raylib_MeasureTextHeight;
}
