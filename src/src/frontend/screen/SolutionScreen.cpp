#include "frontend/screen/SolutionScreen.hpp"

#include "frontend/ui/Button.hpp"
#include "raylib.h"

#include <algorithm>
#include <cstdio>
#include <string>

static float getScreenWidthSafe() {
    return std::max(900, GetScreenWidth());
}

static float getScreenHeightSafe() {
    return std::max(620, GetScreenHeight());
}

static Rectangle getInfoPanelBounds() {
    return Rectangle{34.0f, 118.0f, 330.0f, getScreenHeightSafe() - 188.0f};
}

static Rectangle getBoardBounds() {
    return Rectangle{398.0f, 128.0f, getScreenWidthSafe() - 474.0f, getScreenHeightSafe() - 214.0f};
}

static Rectangle getControlButtonBounds(int index) {
    const float width = 126.0f;
    const float gap = 14.0f;
    const float totalWidth = 5.0f * width + 4.0f * gap;
    const float startX = (getScreenWidthSafe() - totalWidth) / 2.0f;

    return Rectangle{startX + index * (width + gap), 48.0f, width, 48.0f};
}

static Rectangle getBottomButtonBounds(int index) {
    const float width = 156.0f;
    const float gap = 14.0f;
    const float startX = getScreenWidthSafe() - 3.0f * width - 2.0f * gap - 34.0f;

    return Rectangle{startX + index * (width + gap), getScreenHeightSafe() - 72.0f, width, 54.0f};
}

static void drawInfoLine(const char* label, const char* value, float x, float y) {
    DrawText(label, static_cast<int>(x), static_cast<int>(y), 20, Color{176, 185, 204, 255});
    DrawText(value, static_cast<int>(x), static_cast<int>(y + 28.0f), 26, Color{246, 248, 252, 255});
}

SolutionScreen::SolutionScreen()
    : animatedBoard(getBoardBounds()) {}

void SolutionScreen::update(GUIController& controller) {
    animatedBoard.setBounds(getBoardBounds());
    controller.updatePlayback(GetFrameTime());

    if (Button("Pause", getControlButtonBounds(0)).isClicked()) controller.pausePlayback();
    if (Button("Resume", getControlButtonBounds(1)).isClicked()) controller.resumePlayback();
    if (Button("Stop", getControlButtonBounds(2)).isClicked()) controller.stopPlayback();
    if (Button("Back", getControlButtonBounds(3)).isClicked()) controller.stepPlaybackBack();
    if (Button("Forward", getControlButtonBounds(4)).isClicked()) controller.stepPlaybackForward();

    if (Button("Save", getBottomButtonBounds(0)).isClicked()) controller.saveSolution();
    if (Button("Main Menu", getBottomButtonBounds(1)).isClicked()) controller.openMainMenu();
    if (Button("Reconfigure", getBottomButtonBounds(2)).isClicked()) controller.openConfig();
}

void SolutionScreen::draw(const GUIController& controller) const {
    const Rectangle infoPanel = getInfoPanelBounds();
    const char* title = "Solution";
    const int titleFontSize = 38;
    char costText[32];
    char iterationText[32];
    char timeText[32];
    const std::string movesText = controller.getSolutionMoves().empty() ? "-" : controller.getSolutionMoves();

    std::snprintf(costText, sizeof(costText), "%d", controller.getSolutionCost());
    std::snprintf(iterationText, sizeof(iterationText), "%d", controller.getSolutionIterations());
    std::snprintf(timeText, sizeof(timeText), "%lld ms", controller.getSolutionExecutionTime());

    ClearBackground(Color{22, 24, 30, 255});
    DrawText(
        title,
        static_cast<int>((getScreenWidthSafe() - MeasureText(title, titleFontSize)) / 2.0f),
        10,
        titleFontSize,
        Color{246, 248, 252, 255}
    );

    Button("Pause", getControlButtonBounds(0)).draw(!controller.isPlaybackPlaying());
    Button("Resume", getControlButtonBounds(1)).draw(controller.isPlaybackPlaying());
    Button("Stop", getControlButtonBounds(2)).draw();
    Button("Back", getControlButtonBounds(3)).draw();
    Button("Forward", getControlButtonBounds(4)).draw();

    DrawRectangleRounded(infoPanel, 0.05f, 12, Color{34, 38, 48, 255});
    DrawRectangleRoundedLines(infoPanel, 0.05f, 12, Color{85, 94, 112, 255});
    DrawText("Infographics", static_cast<int>(infoPanel.x + 28.0f), static_cast<int>(infoPanel.y + 28.0f), 28, Color{246, 248, 252, 255});
    drawInfoLine("Cost", costText, infoPanel.x + 28.0f, infoPanel.y + 92.0f);
    drawInfoLine("Solution", movesText.c_str(), infoPanel.x + 28.0f, infoPanel.y + 174.0f);
    drawInfoLine("Iterations", iterationText, infoPanel.x + 28.0f, infoPanel.y + 256.0f);
    drawInfoLine("Execution Time", timeText, infoPanel.x + 28.0f, infoPanel.y + 338.0f);

    animatedBoard.draw(
        controller.getPaintBoard(),
        controller.getPlaybackPath(),
        controller.getPlaybackIndex(),
        controller.getPlaybackProgress()
    );

    Button("Save", getBottomButtonBounds(0)).draw();
    Button("Main Menu", getBottomButtonBounds(1)).draw();
    Button("Reconfigure", getBottomButtonBounds(2)).draw();
}
