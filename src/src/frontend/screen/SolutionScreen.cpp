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

static float scaleX() {
    return getScreenWidthSafe() / 1280.0f;
}

static float scaleY() {
    return getScreenHeightSafe() / 720.0f;
}

static Rectangle getInfoPanelBounds() {
    return Rectangle{34.0f * scaleX(), 118.0f * scaleY(), 330.0f * scaleX(), getScreenHeightSafe() - 188.0f * scaleY()};
}

static Rectangle getBoardBounds() {
    return Rectangle{398.0f * scaleX(), 128.0f * scaleY(), getScreenWidthSafe() - 474.0f * scaleX(), getScreenHeightSafe() - 214.0f * scaleY()};
}

static Rectangle getControlButtonBounds(int index) {
    const float width = 126.0f * scaleX();
    const float gap = 14.0f * scaleX();
    const float totalWidth = 5.0f * width + 4.0f * gap;
    const float startX = (getScreenWidthSafe() - totalWidth) / 2.0f;

    return Rectangle{startX + index * (width + gap), 48.0f * scaleY(), width, 48.0f * scaleY()};
}

static Rectangle getBottomButtonBounds(int index) {
    const float width = 156.0f * scaleX();
    const float gap = 14.0f * scaleX();
    const float startX = getScreenWidthSafe() - 3.0f * width - 2.0f * gap - 34.0f * scaleX();

    return Rectangle{startX + index * (width + gap), getScreenHeightSafe() - 72.0f * scaleY(), width, 54.0f * scaleY()};
}

static void drawInfoLine(const char* label, const char* value, float x, float y) {
    const int labelFontSize = static_cast<int>(20.0f * std::min(scaleX(), scaleY()));
    const int valueFontSize = static_cast<int>(26.0f * std::min(scaleX(), scaleY()));
    DrawText(label, static_cast<int>(x), static_cast<int>(y), labelFontSize, Color{176, 185, 204, 255});
    DrawText(value, static_cast<int>(x), static_cast<int>(y + 28.0f * scaleY()), valueFontSize, Color{246, 248, 252, 255});
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

    if (Button("Save", getBottomButtonBounds(0)).isClicked()) controller.openSave();
    if (Button("Reconfigure", getBottomButtonBounds(1)).isClicked()) controller.openConfig();
    if (Button("Main Menu", getBottomButtonBounds(2)).isClicked()) controller.openMainMenu();
}

void SolutionScreen::draw(const GUIController& controller) const {
    const Rectangle infoPanel = getInfoPanelBounds();
    const char* title = "Solution";
    const int titleFontSize = static_cast<int>(38.0f * std::min(scaleX(), scaleY()));
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
        static_cast<int>(10.0f * scaleY()),
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
    DrawText("Infographics", static_cast<int>(infoPanel.x + 28.0f * scaleX()), static_cast<int>(infoPanel.y + 28.0f * scaleY()), static_cast<int>(28.0f * std::min(scaleX(), scaleY())), Color{246, 248, 252, 255});
    drawInfoLine("Cost", costText, infoPanel.x + 28.0f * scaleX(), infoPanel.y + 92.0f * scaleY());
    drawInfoLine("Solution", movesText.c_str(), infoPanel.x + 28.0f * scaleX(), infoPanel.y + 174.0f * scaleY());
    drawInfoLine("Iterations", iterationText, infoPanel.x + 28.0f * scaleX(), infoPanel.y + 256.0f * scaleY());
    drawInfoLine("Execution Time", timeText, infoPanel.x + 28.0f * scaleX(), infoPanel.y + 338.0f * scaleY());

    animatedBoard.draw(
        controller.getPaintBoard(),
        controller.getPlaybackPath(),
        controller.getPlaybackIndex(),
        controller.getPlaybackProgress()
    );

    Button("Save", getBottomButtonBounds(0)).draw();
    Button("Reconfigure", getBottomButtonBounds(1)).draw();
    Button("Main Menu", getBottomButtonBounds(2)).draw();
}
