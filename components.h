//
// Created by Qiaozhi Lei on 5/8/25.
//

#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "ui.h"

using namespace UI;
using namespace Layout;

struct InputHint {
    LayoutElement root;
    UI_Text label = {
        .wrap = WRAP_NONE,
        .scale = 1.0
    };
    std::string iconPath;

    InputHint(const std::string &text, const std::string &iconPath) : iconPath(std::move(iconPath)) {
        label.text = std::move(text);
        root = LayoutBuilder{}.name("Input Hint").size(FIT, FIT)
                .children({
                    LayoutBuilder{}.name("Input").size(0, GROW)
                    .sizeFn([](LayoutElement *layout) { layout->width = layout->height; })
                    .drawFn([&](LayoutElement *layout) {
                        // UI_DrawRectangle(layout->x, layout->y, layout->width, layout->height, UI_GRAY);
                        UI_DrawImage(this->iconPath, layout->x, layout->y, layout->width, layout->height);
                    }),
                    LayoutBuilder{}.name("Label").pointer(&label.layout),
                });

        InitReferencePointers(root);
        label.Link();
    }
};

#endif //COMPONENTS_H
