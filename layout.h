//
// Created by Qiaozhi Lei on 4/15/25.
//

#ifndef LAYOUT_H
#define LAYOUT_H
#include <string>
#include <vector>

namespace Layout {
    enum AxisDirection {
        HORIZONTAL,
        VERTICAL
    };

    enum Alignment {
        START,
        CENTER,
        END,
    };

    enum Sizing {
        FIXED,
        FIT,
        GROW
    };

    struct LayoutElement;
    using DrawFn = std::function<void(LayoutElement *)>;
    using SizeFn = std::function<void(LayoutElement *)>;
    using EventFn = std::function<void(LayoutElement *)>;

    struct LayoutElement {
        std::string debugName = "Element";
        int width = 0;
        int height = 0;
        int x = 0;
        int y = 0;
        int padding = 20;
        int gap = 10;

        int maxWidth = INT_MAX;
        int maxHeight = INT_MAX;
        int minWidth = 0;
        int minHeight = 0;

        AxisDirection mainAxis = HORIZONTAL;
        Sizing widthSizing = FIXED;
        Sizing heightSizing = FIXED;
        Alignment mainAlignment = START;
        Alignment crossAlignment = START;

        std::vector<LayoutElement> children;

        DrawFn drawFn = nullptr;
        SizeFn sizeFn = nullptr;
        EventFn updateFn = nullptr;
        EventFn onMouseEnterFn = nullptr;
        EventFn onMouseLeaveFn = nullptr;
        EventFn onMouseClickFn = nullptr;
        bool hovering;

        LayoutElement **referencePointer = nullptr;

        int GetMainCoord() const {
            return mainAxis == HORIZONTAL ? x : y;
        }

        int GetCrossCoord() const {
            return mainAxis == HORIZONTAL ? y : x;
        }

        void SetCoord(const int mainCoord, const int crossCoord, const AxisDirection parentAxis) {
            if (parentAxis == HORIZONTAL) {
                x = mainCoord;
                y = crossCoord;
            } else {
                x = crossCoord;
                y = mainCoord;
            }
        }

        Sizing GetMainSizing() const {
            return mainAxis == HORIZONTAL ? widthSizing : heightSizing;
        }

        Sizing GetCrossSizing() const {
            return mainAxis == HORIZONTAL ? heightSizing : widthSizing;
        }

        int GetDimension(const AxisDirection axis) const {
            return axis == HORIZONTAL ? width : height;
        }

        std::string SizingChar(const Sizing sizing) const {
            switch (sizing) {
                case FIT:
                    return "F";
                case GROW:
                    return "G";
            }
            return "";
        }

        Sizing GetSizing(const AxisDirection mainAxis) const {
            return mainAxis == HORIZONTAL ? widthSizing : heightSizing;
        }

        AxisDirection GetCrossAxis() const {
            return mainAxis == HORIZONTAL ? VERTICAL : HORIZONTAL;
        }

        void AddDimension(AxisDirection axis, int add) {
            if (axis == HORIZONTAL)
                width += add;
            else
                height += add;
        }

        int GetMaxDimension(const AxisDirection axis) const {
            return axis == HORIZONTAL ? maxWidth : maxHeight;
        };

        // constructor
        LayoutElement() {
            width = 0;
            height = 0;
        }
    };

    struct LayoutBuilder {
        LayoutElement current;

        LayoutBuilder &name(const std::string &name) {
            current.debugName = name;
            return *this;
        }

        LayoutBuilder &width(const int width) {
            current.width = width;
            current.widthSizing = FIXED;
            return *this;
        }

        LayoutBuilder &width(const Sizing widthSizing) {
            current.widthSizing = widthSizing;
            return *this;
        }

        LayoutBuilder &height(const int height) {
            current.height = height;
            current.heightSizing = FIXED;
            return *this;
        }

        LayoutBuilder &height(const Sizing heightSizing) {
            current.heightSizing = heightSizing;
            return *this;
        }

        LayoutBuilder &maxHeight(const int maxHeight) {
            current.maxHeight = maxHeight;
            return *this;
        }

        LayoutBuilder &maxWidth(const int maxWidth) {
            current.maxWidth = maxWidth;
            return *this;
        }

        LayoutBuilder &minHeight(const int minHeight) {
            current.minHeight = minHeight;
            return *this;
        }

        LayoutBuilder &minWidth(const int minWidth) {
            current.minWidth = minWidth;
            return *this;
        }

        LayoutBuilder &size(const int width, const int height) {
            current.width = width;
            current.widthSizing = FIXED;
            current.height = height;
            current.heightSizing = FIXED;
            return *this;
        }

        LayoutBuilder &size(const Sizing widthSizing, const Sizing heightSizing) {
            current.widthSizing = widthSizing;
            current.heightSizing = heightSizing;
            return *this;
        }

        LayoutBuilder &size(const int width, const Sizing heightSizing) {
            current.width = width;
            current.widthSizing = FIXED;
            current.heightSizing = heightSizing;
            return *this;
        }

        LayoutBuilder &size(const Sizing widthSizing, const int height) {
            current.widthSizing = widthSizing;
            current.height = height;
            current.heightSizing = FIXED;
            return *this;
        }

        LayoutBuilder &mainAxis(const AxisDirection axis) {
            current.mainAxis = axis;
            return *this;
        }

        LayoutBuilder &alignment(const Alignment mainAlignment, const Alignment crossAlignment) {
            current.mainAlignment = mainAlignment;
            current.crossAlignment = crossAlignment;
            return *this;
        }

        LayoutBuilder &padding(const int padding) {
            current.padding = padding;
            return *this;
        }

        LayoutBuilder &gap(const int gap) {
            current.gap = gap;
            return *this;
        }

        LayoutBuilder &children(std::vector<LayoutElement> children) {
            current.children = std::move(children);
            return *this;
        }

        LayoutBuilder &drawFn(const DrawFn &drawFn) {
            current.drawFn = drawFn;
            return *this;
        }

        LayoutBuilder &sizeFn(const SizeFn &sizeFn) {
            current.sizeFn = sizeFn;
            return *this;
        }

        LayoutBuilder &updateFn(const EventFn &updateFn) {
            current.updateFn = updateFn;
            return *this;
        }

        LayoutBuilder &onMouseEnterFn(const EventFn &onHoverFn) {
            current.onMouseEnterFn = onHoverFn;
            return *this;
        }

        LayoutBuilder &onMouseLeaveFn(const EventFn &onMouseLeaveFn) {
            current.onMouseLeaveFn = onMouseLeaveFn;
            return *this;
        }

        LayoutBuilder &onMouseClickFn(const EventFn &onMouseClickFn) {
            current.onMouseClickFn = onMouseClickFn;
            return *this;
        }

        LayoutBuilder &pointer(LayoutElement **pointer) {
            current.referencePointer = pointer;
            return *this;
        }

        operator LayoutElement &&() {
            return std::move(current);
        }

        LayoutBuilder copy() const {
            LayoutBuilder builder;
            builder.current = current;
            return builder;
        }
    };

#ifdef LAYOUT_IMPLEMENTATION

    void CalculateGrow(LayoutElement &element) {
        std::vector<LayoutElement *> childrenGrowMain;
        std::vector<LayoutElement *> childrenGrowCross;
        int childrenDimensionMain = 0;
        for (auto &child: element.children) {
            if (child.GetSizing(element.mainAxis) == GROW) {
                childrenGrowMain.emplace_back(&child);
            }
            if (child.GetSizing(element.GetCrossAxis()) == GROW) {
                childrenGrowCross.emplace_back(&child);
            }
            childrenDimensionMain += child.GetDimension(element.mainAxis);
        }

        if (childrenGrowMain.size() > 0) {
            int mainRemain = element.GetDimension(element.mainAxis) - childrenDimensionMain
                             - element.padding * 2 - element.gap * (element.children.size() - 1);
#ifdef LAYOUT_VERBOSE
            std::cout << "Grow along main, remain: " << mainRemain << std::endl << "\t";
            for (const auto *child: childrenGrowMain)
                std::cout << child->debugName << " ";
            std::cout << std::endl;
#endif

            std::vector<LayoutElement *> childrenToGrow;
            while (mainRemain > 0 && childrenGrowMain.size() > 0) {
                // find smallest and second smallest
                int smallest = INT_MAX;
                int smallestChildIndex = -1;
                int secondSmallest = INT_MAX;
                int secondSmallestIndex = -1;
                for (int i = 0; i < childrenGrowMain.size(); i++) {
                    auto *child = childrenGrowMain[i];
                    if (child->GetDimension(element.mainAxis) < smallest) {
                        secondSmallest = smallest;
                        secondSmallestIndex = smallestChildIndex;

                        smallest = child->GetDimension(element.mainAxis);
                        smallestChildIndex = i;
                    } else if (child->GetDimension(element.mainAxis) < secondSmallest) {
                        secondSmallest = child->GetDimension(element.mainAxis);
                        secondSmallestIndex = i;
                    }
                }

                // remove smallest and add it to toGrow
                auto *smallestChild = childrenGrowMain[smallestChildIndex];
                childrenGrowMain.erase(childrenGrowMain.begin() + smallestChildIndex);
                childrenToGrow.emplace_back(smallestChild);

                // grow amount is difference between smallest and second smallest
                int growAmount = (mainRemain + childrenToGrow.size() - 1) / childrenToGrow.size();
                // if there is no second smallest, grow fully
                if (secondSmallestIndex != -1) {
                    growAmount = std::min(growAmount, secondSmallest - smallest);
                }
#ifdef LAYOUT_VERBOSE
                std::cout << "Grow pass, remain:" << mainRemain << " grow by:" << growAmount << std::endl;
#endif

                // grow
                for (int i = int(childrenToGrow.size()) - 1; i >= 0; --i) {
                    auto *child = childrenToGrow[i];
                    // max constraint
                    int childGrowAmount = growAmount;
                    int max = child->GetMaxDimension(element.mainAxis);
                    if (child->GetDimension(element.mainAxis) + growAmount > max) {
#ifdef LAYOUT_VERBOSE
                        std::cout << "Child " << child->debugName << " grow amount exceeds max, "
                                << child->GetDimension(element.mainAxis) << "+" << growAmount << " > " << max <<
                                std::endl;
#endif
                        childGrowAmount = max - child->GetDimension(element.mainAxis);
                        // remove child from toGrow
                        childrenToGrow.erase(childrenToGrow.begin() + i);
                    }
                    child->AddDimension(element.mainAxis, childGrowAmount);
                    mainRemain -= childGrowAmount;
                }
            }
        }

        if (childrenGrowCross.size() > 0) {
#ifdef LAYOUT_VERBOSE
            std::cout << "Grow along cross, " << std::endl << "\t";
            for (auto *child: childrenGrowCross) {
                int crossRemain = element.GetDimension(element.GetCrossAxis())
                                  - child->GetDimension(element.GetCrossAxis()) - element.padding * 2;
                std::cout << child->debugName << " " << crossRemain << ",";
            }
            std::cout << std::endl;
#endif
            for (auto *child: childrenGrowCross) {
                int crossRemain = element.GetDimension(element.GetCrossAxis())
                                  - child->GetDimension(element.GetCrossAxis()) - element.padding * 2;
                int max = child->GetMaxDimension(element.GetCrossAxis());
                if (child->GetDimension(element.GetCrossAxis()) + crossRemain > max) {
                    crossRemain = max - child->GetDimension(element.GetCrossAxis());
                }
                child->AddDimension(element.GetCrossAxis(), crossRemain);
            }
        }
    }

    void CalculateSize(LayoutElement &element) {
        // recalculate Fit sizing
        int mainSum = 0;
        if (element.children.size() > 0)
            mainSum += element.gap * (element.children.size() - 1) + element.padding * 2;

        int maxCross = 0;
        for (auto &child: element.children) {
            const int main = element.mainAxis == HORIZONTAL ? child.width : child.height;
            int cross = element.mainAxis == HORIZONTAL ? child.height : child.width;

            mainSum += main;
            maxCross = std::max(maxCross, cross);
        }
        if (element.children.size() > 0)
            maxCross += element.padding * 2;

        if (element.heightSizing != FIXED) {
            element.height = element.mainAxis == HORIZONTAL ? maxCross : mainSum;
            element.height = std::max(element.minHeight, element.height);
        }
        if (element.widthSizing != FIXED) {
            element.width = element.mainAxis == HORIZONTAL ? mainSum : maxCross;
            element.width = std::max(element.minWidth, element.width);
        }
    }

    void DFS_Size(LayoutElement &current) {
        for (auto &child: current.children) {
            DFS_Size(child);
        }
        CalculateSize(current);
    }

    void DFS_Grow(LayoutElement &current) {
        CalculateGrow(current);
        for (auto &child: current.children) {
            DFS_Grow(child);
        }
    }

    void CustomSizing(LayoutElement &root) {
        std::vector<LayoutElement *> toExplore = {&root};
        while (!toExplore.empty()) {
            LayoutElement *current = toExplore.back();
            toExplore.pop_back();

            if (current->sizeFn != nullptr)
                current->sizeFn(current);

            for (auto &child: current->children) {
                toExplore.emplace_back(&child);
            }
        }
    }

    void CalculateLayout(LayoutElement &root) {
        //  Sizes
        DFS_Size(root);

        // Grow
        DFS_Grow(root);

        CustomSizing(root);

        DFS_Grow(root);

        // Positions
        std::vector<LayoutElement *> toExplore = {&root};
        while (!toExplore.empty()) {
            LayoutElement *current = toExplore.back();
            toExplore.pop_back();

            int childrenMainSum = current->gap * (current->children.size() - 1);
            for (auto &child: current->children) {
                childrenMainSum += child.GetDimension(current->mainAxis);
            }
            int mainStart = current->GetMainCoord();
            if (current->mainAlignment == START)
                mainStart += current->padding;
            if (current->mainAlignment == CENTER)
                mainStart += current->GetDimension(current->mainAxis) * 0.5 - childrenMainSum * 0.5;
            if (current->mainAlignment == END)
                mainStart += current->GetDimension(current->mainAxis) - childrenMainSum - current->padding;

            // calculate positions of children
            int mainAxisProcessed = 0;
            for (auto &child: current->children) {
                const int main = mainStart + mainAxisProcessed;
                int cross = current->GetCrossCoord();
                if (current->crossAlignment == START)
                    cross += current->padding;
                if (current->crossAlignment == CENTER) {
                    cross += current->GetDimension(current->GetCrossAxis()) * 0.5 - child.GetDimension(
                        current->GetCrossAxis()) * 0.5;
                }
                if (current->crossAlignment == END) {
                    cross += current->GetDimension(current->GetCrossAxis()) - child.GetDimension(
                        current->GetCrossAxis()) - current->padding;
                }

                child.SetCoord(main, cross, current->mainAxis);

                mainAxisProcessed += child.GetDimension(current->mainAxis) + current->gap;
            }

            for (auto &child: current->children) {
#ifdef LAYOUT_VERBOSE
                std::cout << TextFormat("[%s], %p, %s, %s%ix%s%i, (%i,%i), P%i G%i", child.debugName.c_str(), &child,
                                        child.mainAxis == HORIZONTAL ? "H" : "V",
                                        child.SizingChar(child.widthSizing).c_str(), child.width,
                                        child.SizingChar(child.heightSizing).c_str(), child.height,
                                        child.x, child.y, child.padding, child.gap) << std::endl;
#endif
                toExplore.emplace_back(&child);
            }
        }
    }

    void InitReferencePointers(LayoutElement &root) {
        std::vector<LayoutElement *> toExplore = {&root};
        while (!toExplore.empty()) {
            LayoutElement *current = toExplore.back();
            toExplore.pop_back();

            if (current->referencePointer != nullptr)
                *current->referencePointer = current;

            for (auto &child: current->children) {
                toExplore.emplace_back(&child);
            }
        }
    }

    void DrawUI(LayoutElement &root) {
        std::vector<LayoutElement *> toExplore = {&root};
        while (!toExplore.empty()) {
            LayoutElement *current = toExplore.back();
            toExplore.pop_back();

            if (current->drawFn != nullptr)
                current->drawFn(current);

            for (auto &child: current->children) {
                toExplore.emplace_back(&child);
            }
        }
    }
#endif
}

#endif //LAYOUT_H
