//
// Created by Qiaozhi Lei on 4/29/25.
//

#ifndef UI_H
#define UI_H

#include "layout.h"
#include <string>
#include <sstream>
#include <vector>

namespace UI {
    using IsMousePressed = bool(*)();
    inline IsMousePressed isMousePressedFn = nullptr;

    inline bool UI_IsMousePressed() {
        if (isMousePressedFn != nullptr) {
            return isMousePressedFn();
        }
        return false;
    }

    using GetMousePosFn = std::array<float, 2>(*)();
    inline GetMousePosFn getMousePosFn = nullptr;

    inline std::array<float, 2> UI_GetMousePos() {
        if (getMousePosFn != nullptr) {
            return getMousePosFn();
        }
        return {0, 0};
    }

    using DrawTextFn = void(*)(const char *, int, int, float);
    inline DrawTextFn drawTextFn = nullptr;

    inline void UI_DrawText(const char *str, const int x, const int y, float scale) {
        if (drawTextFn != nullptr) {
            drawTextFn(str, x, y, scale);
        }
    }

    using MeasureTextFn = float(*)(const char *, float);
    inline MeasureTextFn measureTextFn = nullptr;

    inline float UI_MeasureText(const char *str, const float scale) {
        if (measureTextFn != nullptr) {
            return measureTextFn(str, scale);
        }
        return 0;
    }

    using MeasureTextHeightFn = float(*)(const char *, float);
    inline MeasureTextHeightFn measureTextHeightFn = nullptr;

    inline float UI_MeasureTextHeight(const char *str, const float scale) {
        if (measureTextHeightFn != nullptr) {
            return measureTextHeightFn(str, scale);
        }
        return 0;
    }

    inline std::string UI_WrapText(const std::string &text, float scale, float maxWidth) {
        std::vector<std::string> paragraphs;
        size_t start = 0;
        // split on existing newlines
        while (true) {
            size_t pos = text.find('\n', start);
            if (pos == std::string::npos) {
                paragraphs.emplace_back(text.substr(start));
                break;
            }
            paragraphs.emplace_back(text.substr(start, pos - start));
            start = pos + 1;
        }

        std::string output;
        for (size_t pi = 0; pi < paragraphs.size(); ++pi) {
            const std::string &para = paragraphs[pi];
            std::istringstream iss(para);
            std::string word;
            std::string line;
            while (iss >> word) {
                if (line.empty()) {
                    // first word on line
                    line = word;
                } else {
                    // test adding word with a space
                    std::string test = line + ' ' + word;
                    if (UI_MeasureText(test.c_str(), scale) <= maxWidth) {
                        line = std::move(test);
                    } else {
                        // flush current line
                        output += line;
                        output += '\n';
                        line = word;
                    }
                }
                // handle a single word longer than maxWidth
                if (UI_MeasureText(line.c_str(), scale) > maxWidth) {
                    // break mid-word
                    std::string part;
                    for (char c: line) {
                        part.push_back(c);
                        if (UI_MeasureText(part.c_str(), scale) > maxWidth) {
                            // emit everything up to previous char
                            part.pop_back();
                            output += part;
                            output += '\n';
                            part.clear();
                            part.push_back(c);
                        }
                    }
                    line = std::move(part);
                }
            }
            // flush last line of paragraph
            if (!line.empty()) {
                output += line;
            }
            if (pi + 1 < paragraphs.size())
                output += '\n'; // preserve original blank‐line separators
        }
        return output;
    }

    inline bool CollisionMouseLayout(Layout::LayoutElement &element) {
        std::array<float, 2> mouse = UI_GetMousePos();
        return mouse[0] >= element.x && mouse[0] <= element.x + element.width &&
               mouse[1] >= element.y && mouse[1] <= element.y + element.height;
    }

    inline void DetectInputEvents(Layout::LayoutElement &root) {
        std::vector<Layout::LayoutElement *> toExplore = {&root};
        while (!toExplore.empty()) {
            Layout::LayoutElement *current = toExplore.back();
            toExplore.pop_back();

            if (current->updateFn != nullptr)
                current->updateFn(current);
            if (CollisionMouseLayout(*current) && !current->hovering) {
                if (current->onMouseEnterFn != nullptr) {
                    current->onMouseEnterFn(current);
                    current->hovering = true;
                }
            }
            if (!CollisionMouseLayout(*current) && current->hovering) {
                if (current->onMouseLeaveFn != nullptr) {
                    current->onMouseLeaveFn(current);
                    current->hovering = false;
                }
            }
            if (CollisionMouseLayout(*current) && UI_IsMousePressed()) {
                if (current->onMouseClickFn != nullptr) {
                    current->onMouseClickFn(current);
                }
            }

            for (auto &child: current->children) {
                toExplore.emplace_back(&child);
            }
        }
    }

    enum TextWrap {
        WRAP_NONE = 0,
        WRAP_WORD = 1,
    };

    struct UI_Text {
        Layout::LayoutElement *layout;
        const char *text;
        float scale = 1.0;
        TextWrap wrap = WRAP_WORD;

        void SizeFn(Layout::LayoutElement *layout) {
            if (wrap == WRAP_WORD) {
                layout->height = UI_MeasureTextHeight(UI_WrapText(text, scale, layout->width).c_str(),
                                                      scale);
            } else {
                layout->width = UI_MeasureText(text, scale);
                layout->height = UI_MeasureTextHeight(text, scale);
            }
        }

        void DrawFn(Layout::LayoutElement *layout) {
            if (wrap == WRAP_WORD) {
                UI_DrawText(
                    UI_WrapText(text, scale, layout->width).c_str(), layout->x, layout->y, scale);
            } else {
                UI_DrawText(text, layout->x, layout->y, scale);
            }
        }

        void Link() {
            if (layout == nullptr) return;
            layout->sizeFn = [&](Layout::LayoutElement *layout) {
                SizeFn(layout);
            };
            layout->drawFn = [&](Layout::LayoutElement *layout) {
                DrawFn(layout);
            };
        }
    };
}

#endif //UI_H
