#include "frontend/ui/TileLegend.hpp"

#include <algorithm>

static float legendScale() {
    return std::max(0.78f, std::min(GetScreenWidth() / 1280.0f, GetScreenHeight() / 720.0f));
}

TileLegend::TileLegend(Rectangle bounds)
    : bounds(bounds) {}

void TileLegend::setBounds(Rectangle bounds) {
    this->bounds = bounds;
}

void TileLegend::draw() const {
    const int titleFontSize = static_cast<int>(22.0f * legendScale());

    DrawRectangleRounded(bounds, 0.08f, 12, Color{34, 38, 48, 235});
    DrawRectangleRoundedLines(bounds, 0.08f, 12, Color{85, 94, 112, 255});
    DrawText(
        "Legend",
        static_cast<int>(bounds.x + 18.0f * legendScale()),
        static_cast<int>(bounds.y + 16.0f * legendScale()),
        titleFontSize,
        Color{246, 248, 252, 255}
    );

    drawItem(0, WHITE, "Floor");
    drawItem(1, Color{10, 10, 12, 255}, "Wall");
    drawItem(2, Color{220, 45, 45, 255}, "Lava");
    drawItem(3, Color{250, 210, 60, 255}, "Number", "1");
    drawItem(4, Color{246, 241, 202, 255}, "Used");
    drawItem(5, Color{24, 150, 48, 255}, "Goal");
    drawItem(6, Color{33, 125, 220, 255}, "Player");
}

void TileLegend::drawItem(int index, Color color, const char* label, const char* marker) const {
    const float scale = legendScale();
    const float swatchSize = 22.0f * scale;
    const float x = bounds.x + 18.0f * scale;
    const float y = bounds.y + (58.0f + index * 36.0f) * scale;
    const int fontSize = static_cast<int>(18.0f * scale);

    DrawRectangleRec(Rectangle{x, y, swatchSize, swatchSize}, color);
    DrawRectangleLinesEx(Rectangle{x, y, swatchSize, swatchSize}, 1.0f, Color{104, 112, 130, 255});

    if (marker != nullptr) {
        const int markerWidth = MeasureText(marker, fontSize);
        DrawText(
            marker,
            static_cast<int>(x + (swatchSize - markerWidth) / 2.0f),
            static_cast<int>(y + (swatchSize - fontSize) / 2.0f),
            fontSize,
            Color{24, 29, 39, 255}
        );
    }

    DrawText(
        label,
        static_cast<int>(x + swatchSize + 10.0f * scale),
        static_cast<int>(y + 2.0f * scale),
        fontSize,
        Color{218, 224, 236, 255}
    );
}
