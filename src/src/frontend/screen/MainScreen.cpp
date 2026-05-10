#include "frontend/screen/MainScreen.hpp"

#include "raylib.h"

MainScreen::MainScreen()
    : boardView() {}

void MainScreen::draw() const {
    const float screenWidth = GetScreenWidth();
    const float screenHeight = GetScreenHeight();
    const float rightPanelWidth = screenWidth * 0.55f;
    const float margin = screenHeight * 0.06f;

    ClearBackground(Color{22, 24, 30, 255});
    boardView.draw(
        screenWidth - rightPanelWidth,
        margin,
        rightPanelWidth,
        screenHeight - 2.0f * margin
    );
}
