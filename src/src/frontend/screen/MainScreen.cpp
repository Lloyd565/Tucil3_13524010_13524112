#include "frontend/screen/MainScreen.hpp"

#include "raylib.h"

static void drawMenuOption(const char* text, float x, float y, float width, float height) {
    const Rectangle bounds = {x, y, width, height};
    const int fontSize = 28;
    const int textWidth = MeasureText(text, fontSize);
    const int textX = static_cast<int>(x + (width - textWidth) / 2.0f);
    const int textY = static_cast<int>(y + (height - fontSize) / 2.0f);

    DrawRectangleRounded(bounds, 0.08f, 12, Color{246, 248, 252, 255});
    DrawRectangleRoundedLines(bounds, 0.08f, 12, Color{74, 80, 94, 255});
    DrawText(text, textX, textY, fontSize, Color{31, 35, 45, 255});
}

MainScreen::MainScreen()
    : boardView() {}

void MainScreen::draw() const {
    const float screenWidth = GetScreenWidth();
    const float screenHeight = GetScreenHeight();
    const float rightPanelWidth = screenWidth * 0.52f;
    const float leftPanelWidth = screenWidth - rightPanelWidth;
    const float margin = screenHeight * 0.06f;
    const float menuWidth = leftPanelWidth * 0.62f;
    const float menuX = (leftPanelWidth - menuWidth) / 2.0f;
    const float optionHeight = 64.0f;
    const float optionGap = 22.0f;
    const float firstOptionY = screenHeight * 0.42f;
    const char* title = "Tucil 3 Solver";
    const int titleFontSize = 42;
    const int titleWidth = MeasureText(title, titleFontSize);

    ClearBackground(Color{22, 24, 30, 255});

    DrawText(
        title,
        static_cast<int>((leftPanelWidth - titleWidth) / 2.0f),
        static_cast<int>(screenHeight * 0.25f),
        titleFontSize,
        Color{246, 248, 252, 255}
    );

    drawMenuOption("New Game", menuX, firstOptionY, menuWidth, optionHeight);
    drawMenuOption("Load Game", menuX, firstOptionY + optionHeight + optionGap, menuWidth, optionHeight);
    drawMenuOption("Exit", menuX, firstOptionY + 2.0f * (optionHeight + optionGap), menuWidth, optionHeight);

    boardView.draw(
        screenWidth - rightPanelWidth,
        margin,
        rightPanelWidth,
        screenHeight - 2.0f * margin
    );
}
