#include "frontend/screen/ConfigScreen.hpp"

#include "frontend/ui/Button.hpp"
#include "frontend/ui/PaintButton.hpp"
#include "raylib.h"

#include <algorithm>
#include <string>

static float getScreenWidthSafe() {
    return std::max(800, GetScreenWidth());
}

static float getScreenHeightSafe() {
    return std::max(600, GetScreenHeight());
}

static float scaleX() {
    return getScreenWidthSafe() / 1280.0f;
}

static float scaleY() {
    return getScreenHeightSafe() / 720.0f;
}

static Rectangle getConfigPanelBounds() {
    return Rectangle{34.0f * scaleX(), 82.0f * scaleY(), 380.0f * scaleX(), getScreenHeightSafe() - 142.0f * scaleY()};
}

static Rectangle getBoardBounds() {
    return Rectangle{454.0f * scaleX(), 118.0f * scaleY(), getScreenWidthSafe() - 528.0f * scaleX(), getScreenHeightSafe() - 214.0f * scaleY()};
}

static Rectangle getPaintButtonBounds() {
    return Rectangle{getScreenWidthSafe() - 132.0f * scaleX(), 42.0f * scaleY(), 58.0f * scaleX(), 58.0f * scaleY()};
}

static Rectangle getEnterButtonBounds() {
    return Rectangle{getScreenWidthSafe() - 190.0f * scaleX(), getScreenHeightSafe() - 72.0f * scaleY(), 140.0f * scaleX(), 54.0f * scaleY()};
}

static Rectangle getMainMenuButtonBounds() {
    return Rectangle{34.0f * scaleX(), 28.0f * scaleY(), 170.0f * scaleX(), 48.0f * scaleY()};
}

static Rectangle getAlgorithmButtonBounds(int index) {
    const Rectangle panel = getConfigPanelBounds();
    return Rectangle{panel.x + 34.0f * scaleX(), panel.y + (88.0f + index * 56.0f) * scaleY(), panel.width - 68.0f * scaleX(), 44.0f * scaleY()};
}

static Rectangle getHeuristicButtonBounds(int index) {
    const Rectangle panel = getConfigPanelBounds();
    const float startY = panel.y + 366.0f * scaleY();
    const float bottomPadding = 34.0f * scaleY();
    const float availableHeight = panel.y + panel.height - bottomPadding - startY;
    const float gap = 6.0f * scaleY();
    const float buttonHeight = std::max(24.0f * scaleY(), (availableHeight - 5.0f * gap) / 6.0f);

    return Rectangle{
        panel.x + 34.0f * scaleX(),
        startY + index * (buttonHeight + gap),
        panel.width - 68.0f * scaleX(),
        buttonHeight
    };
}

static bool shouldShowHeuristics(const GUIController& controller) {
    return controller.getSelectedAlgorithm() == "A*" ||
           controller.getSelectedAlgorithm() == "GBFS" ||
           controller.getSelectedAlgorithm() == "IDA*";
}

ConfigScreen::ConfigScreen()
    : boardCanvas(getBoardBounds()) {}

void ConfigScreen::update(GUIController& controller) {
    const char* algorithms[] = {"UCS", "GBFS", "A*", "IDA*"};
    const char* heuristics[] = {"H1", "H2", "H3", "H4", "H5", "H6"};

    boardCanvas.setBounds(getBoardBounds());

    if (Button("Main Menu", getMainMenuButtonBounds()).isClicked()) {
        controller.returnToMainMenuFromConfig();
        return;
    }

    for (int i = 0 ; i < 4 ; i++) {
        if (Button(algorithms[i], getAlgorithmButtonBounds(i)).isClicked()) {
            controller.setSelectedAlgorithm(algorithms[i]);
        }
    }

    if (shouldShowHeuristics(controller)) {
        for (int i = 0 ; i < 6 ; i++) {
            if (Button(heuristics[i], getHeuristicButtonBounds(i)).isClicked()) {
                controller.setSelectedHeuristic(heuristics[i]);
            }
        }
    }

    if (PaintButton(getPaintButtonBounds()).isClicked()) {
        controller.openNewGame();
    }

    if (Button("Enter", getEnterButtonBounds()).isClicked()) {
        controller.submitConfig();
    }
}

void ConfigScreen::draw(const GUIController& controller) const {
    const char* title = "Configuration";
    const char* algorithmTitle = "Algorithm";
    const char* heuristicTitle = "Heuristic";
    const char* algorithms[] = {"UCS", "GBFS", "A*", "IDA*"};
    const char* heuristics[] = {"H1", "H2", "H3", "H4", "H5", "H6"};
    const Rectangle panel = getConfigPanelBounds();
    const int titleFontSize = static_cast<int>(38.0f * std::min(scaleX(), scaleY()));
    const int sectionFontSize = static_cast<int>(26.0f * std::min(scaleX(), scaleY()));

    ClearBackground(Color{22, 24, 30, 255});
    Button("Main Menu", getMainMenuButtonBounds()).draw();
    DrawText(
        title,
        static_cast<int>((getScreenWidthSafe() - MeasureText(title, titleFontSize)) / 2.0f),
        static_cast<int>(28.0f * scaleY()),
        titleFontSize,
        Color{246, 248, 252, 255}
    );

    DrawRectangleRounded(panel, 0.05f, 12, Color{34, 38, 48, 255});
    DrawRectangleRoundedLines(panel, 0.05f, 12, Color{85, 94, 112, 255});
    DrawText(algorithmTitle, static_cast<int>(panel.x + 34.0f * scaleX()), static_cast<int>(panel.y + 34.0f * scaleY()), sectionFontSize, Color{246, 248, 252, 255});

    for (int i = 0 ; i < 4 ; i++) {
        Button(algorithms[i], getAlgorithmButtonBounds(i)).draw(controller.getSelectedAlgorithm() == algorithms[i]);
    }

    if (shouldShowHeuristics(controller)) {
        DrawText(heuristicTitle, static_cast<int>(panel.x + 34.0f * scaleX()), static_cast<int>(panel.y + 314.0f * scaleY()), sectionFontSize, Color{246, 248, 252, 255});
        for (int i = 0 ; i < 6 ; i++) {
            Button(heuristics[i], getHeuristicButtonBounds(i)).draw(controller.getSelectedHeuristic() == heuristics[i]);
        }
    }

    boardCanvas.draw(controller.getPaintBoard());
    PaintButton(getPaintButtonBounds()).draw();
    Button("Enter", getEnterButtonBounds()).draw();

    if (!controller.getConfigMessage().empty()) {
        const std::string& message = controller.getConfigMessage();
        const int fontSize = 22;
        const int textWidth = MeasureText(message.c_str(), fontSize);
        const Color textColor = Color{255, 144, 144, 255};

        DrawText(
            message.c_str(),
            static_cast<int>((getScreenWidthSafe() - textWidth) / 2.0f),
            static_cast<int>(getScreenHeightSafe() - 58.0f * scaleY()),
            fontSize,
            textColor
        );
    }
}
