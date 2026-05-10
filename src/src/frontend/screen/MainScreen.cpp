#include "frontend/screen/MainScreen.hpp"

#include "frontend/ui/Button.hpp"
#include "raylib.h"

static Rectangle getMenuOptionBounds(int index) {
    const float screenWidth = GetScreenWidth();
    const float screenHeight = GetScreenHeight();
    const float rightPanelWidth = screenWidth * 0.52f;
    const float leftPanelWidth = screenWidth - rightPanelWidth;
    const float menuWidth = leftPanelWidth * 0.62f;
    const float menuX = (leftPanelWidth - menuWidth) / 2.0f;
    const float optionHeight = 64.0f;
    const float optionGap = 22.0f;
    const float firstOptionY = screenHeight * 0.42f;

    return Rectangle{
        menuX,
        firstOptionY + index * (optionHeight + optionGap),
        menuWidth,
        optionHeight
    };
}

MainScreen::MainScreen()
    : boardView() {}

void MainScreen::update(GUIController& controller) {
    Button newGameButton("New Game", getMenuOptionBounds(0));
    Button loadGameButton("Load Game", getMenuOptionBounds(1));
    Button exitButton("Exit", getMenuOptionBounds(2));

    if (newGameButton.isClicked()) controller.openNewGame();
    else if (loadGameButton.isClicked()) controller.openLoadGame();
    else if (exitButton.isClicked()) controller.requestExit();
}

void MainScreen::draw() const {
    const float screenWidth = GetScreenWidth();
    const float screenHeight = GetScreenHeight();
    const float rightPanelWidth = screenWidth * 0.52f;
    const float leftPanelWidth = screenWidth - rightPanelWidth;
    const float margin = screenHeight * 0.06f;
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

    Button("New Game", getMenuOptionBounds(0)).draw();
    Button("Load Game", getMenuOptionBounds(1)).draw();
    Button("Exit", getMenuOptionBounds(2)).draw();

    boardView.draw(
        screenWidth - rightPanelWidth,
        margin,
        rightPanelWidth,
        screenHeight - 2.0f * margin
    );
}
