#include "frontend/ui/Panel.hpp"

#include "raylib.h"

Panel::Panel(float x, float y, float width, float height, const std::string& text)
    : x(x), y(y), width(width), height(height), text(text) {}

void Panel::draw() const {
    const Rectangle bounds = {x, y, width, height};
    const int fontSize = 32;
    const int textWidth = MeasureText(text.c_str(), fontSize);
    const float textX = x + (width - textWidth) / 2.0f;
    const float textY = y + (height - fontSize) / 2.0f;

    DrawRectangleRounded(bounds, 0.08f, 12, RAYWHITE);
    DrawRectangleRoundedLines(bounds, 0.08f, 12, DARKGRAY);
    DrawText(text.c_str(), static_cast<int>(textX), static_cast<int>(textY), fontSize, DARKGRAY);
}
