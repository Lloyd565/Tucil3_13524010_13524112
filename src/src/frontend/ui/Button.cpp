#include "frontend/ui/Button.hpp"

Button::Button(const char* text, Rectangle bounds)
    : text(text),
      bounds(bounds) {}

bool Button::isClicked() const {
    return IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
           CheckCollisionPointRec(GetMousePosition(), bounds);
}

void Button::draw() const {
    draw(false);
}

void Button::draw(bool isSelected) const {
    const int fontSize = 28;
    const int textWidth = MeasureText(text, fontSize);
    const int textX = static_cast<int>(bounds.x + (bounds.width - textWidth) / 2.0f);
    const int textY = static_cast<int>(bounds.y + (bounds.height - fontSize) / 2.0f);
    const bool isHovered = CheckCollisionPointRec(GetMousePosition(), bounds);
    const Color fillColor = isSelected ? Color{250, 210, 60, 255} :
                            isHovered ? Color{255, 255, 255, 255} :
                                        Color{246, 248, 252, 255};

    DrawRectangleRounded(bounds, 0.08f, 12, fillColor);
    DrawRectangleRoundedLines(bounds, 0.08f, 12, Color{74, 80, 94, 255});
    DrawText(text, textX, textY, fontSize, Color{31, 35, 45, 255});
}
