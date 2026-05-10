#include "frontend/ui/SizeConfig.hpp"

#include "frontend/ui/Button.hpp"

#include <cstdio>

SizeConfig::SizeConfig(Rectangle bounds)
    : bounds(bounds) {}

static float minScale(float xScale, float yScale) {
    return xScale < yScale ? xScale : yScale;
}

Rectangle SizeConfig::getRowsMinusBounds() const {
    const float xScale = bounds.width / 230.0f;
    const float yScale = bounds.height / 530.0f;
    return Rectangle{bounds.x + 28.0f * xScale, bounds.y + 118.0f * yScale, 54.0f * xScale, 46.0f * yScale};
}

Rectangle SizeConfig::getRowsPlusBounds() const {
    const float xScale = bounds.width / 230.0f;
    const float yScale = bounds.height / 530.0f;
    return Rectangle{bounds.x + bounds.width - 82.0f * xScale, bounds.y + 118.0f * yScale, 54.0f * xScale, 46.0f * yScale};
}

Rectangle SizeConfig::getColsMinusBounds() const {
    const float xScale = bounds.width / 230.0f;
    const float yScale = bounds.height / 530.0f;
    return Rectangle{bounds.x + 28.0f * xScale, bounds.y + 226.0f * yScale, 54.0f * xScale, 46.0f * yScale};
}

Rectangle SizeConfig::getColsPlusBounds() const {
    const float xScale = bounds.width / 230.0f;
    const float yScale = bounds.height / 530.0f;
    return Rectangle{bounds.x + bounds.width - 82.0f * xScale, bounds.y + 226.0f * yScale, 54.0f * xScale, 46.0f * yScale};
}

int SizeConfig::getRowDelta() const {
    if (Button("-", getRowsMinusBounds()).isClicked()) return -1;
    if (Button("+", getRowsPlusBounds()).isClicked()) return 1;

    return 0;
}

int SizeConfig::getColDelta() const {
    if (Button("-", getColsMinusBounds()).isClicked()) return -1;
    if (Button("+", getColsPlusBounds()).isClicked()) return 1;

    return 0;
}

void SizeConfig::draw(int rows, int cols) const {
    const char* title = "Board Size";
    const float xScale = bounds.width / 230.0f;
    const float yScale = bounds.height / 530.0f;
    const float uiScale = minScale(xScale, yScale);
    const int titleFontSize = static_cast<int>(28.0f * uiScale);
    const int labelFontSize = static_cast<int>(22.0f * uiScale);
    const int valueFontSize = static_cast<int>(30.0f * uiScale);
    const int titleWidth = MeasureText(title, titleFontSize);
    char rowsText[8];
    char colsText[8];

    std::snprintf(rowsText, sizeof(rowsText), "%d", rows);
    std::snprintf(colsText, sizeof(colsText), "%d", cols);

    DrawRectangleRounded(bounds, 0.06f, 12, Color{34, 38, 48, 255});
    DrawRectangleRoundedLines(bounds, 0.06f, 12, Color{85, 94, 112, 255});
    DrawText(
        title,
        static_cast<int>(bounds.x + (bounds.width - titleWidth) / 2.0f),
        static_cast<int>(bounds.y + 30.0f * yScale),
        titleFontSize,
        Color{246, 248, 252, 255}
    );

    DrawText("Rows", static_cast<int>(bounds.x + 28.0f * xScale), static_cast<int>(bounds.y + 88.0f * yScale), labelFontSize, Color{204, 210, 222, 255});
    Button("-", getRowsMinusBounds()).draw();
    Button("+", getRowsPlusBounds()).draw();
    DrawText(
        rowsText,
        static_cast<int>(bounds.x + (bounds.width - MeasureText(rowsText, valueFontSize)) / 2.0f),
        static_cast<int>(bounds.y + 124.0f * yScale),
        valueFontSize,
        Color{246, 248, 252, 255}
    );

    DrawText("Columns", static_cast<int>(bounds.x + 28.0f * xScale), static_cast<int>(bounds.y + 196.0f * yScale), labelFontSize, Color{204, 210, 222, 255});
    Button("-", getColsMinusBounds()).draw();
    Button("+", getColsPlusBounds()).draw();
    DrawText(
        colsText,
        static_cast<int>(bounds.x + (bounds.width - MeasureText(colsText, valueFontSize)) / 2.0f),
        static_cast<int>(bounds.y + 232.0f * yScale),
        valueFontSize,
        Color{246, 248, 252, 255}
    );
}
