#include "frontend/ui/PaintButton.hpp"

PaintButton::PaintButton(Rectangle bounds)
    : bounds(bounds) {}

bool PaintButton::isClicked() const {
    return IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
           CheckCollisionPointRec(GetMousePosition(), bounds);
}

void PaintButton::draw() const {
    const bool isHovered = CheckCollisionPointRec(GetMousePosition(), bounds);
    const Color fillColor = isHovered ? Color{255, 255, 255, 255} : Color{246, 248, 252, 255};
    const float centerX = bounds.x + bounds.width / 2.0f;
    const float centerY = bounds.y + bounds.height / 2.0f;

    DrawRectangleRounded(bounds, 0.14f, 10, fillColor);
    DrawRectangleRoundedLines(bounds, 0.14f, 10, Color{74, 80, 94, 255});

    DrawLineEx(
        Vector2{centerX - 11.0f, centerY + 11.0f},
        Vector2{centerX + 10.0f, centerY - 10.0f},
        7.0f,
        Color{93, 63, 36, 255}
    );
    DrawCircle(static_cast<int>(centerX + 13.0f), static_cast<int>(centerY - 13.0f), 8.0f, Color{250, 210, 60, 255});
    DrawCircle(static_cast<int>(centerX + 16.0f), static_cast<int>(centerY - 16.0f), 3.0f, Color{255, 132, 64, 255});
    DrawTriangle(
        Vector2{centerX - 16.0f, centerY + 16.0f},
        Vector2{centerX - 6.0f, centerY + 8.0f},
        Vector2{centerX - 1.0f, centerY + 21.0f},
        Color{36, 40, 50, 255}
    );
}
