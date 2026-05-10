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

static Rectangle getConfigPanelBounds() {
    return Rectangle{34.0f, 82.0f, 380.0f, getScreenHeightSafe() - 142.0f};
}

static Rectangle getBoardBounds() {
    return Rectangle{454.0f, 118.0f, getScreenWidthSafe() - 528.0f, getScreenHeightSafe() - 214.0f};
}

static Rectangle getPaintButtonBounds() {
    return Rectangle{getScreenWidthSafe() - 132.0f, 42.0f, 58.0f, 58.0f};
}

static Rectangle getEnterButtonBounds() {
    return Rectangle{getScreenWidthSafe() - 190.0f, getScreenHeightSafe() - 72.0f, 140.0f, 54.0f};
}

static Rectangle getAlgorithmButtonBounds(int index) {
    const Rectangle panel = getConfigPanelBounds();
    return Rectangle{panel.x + 34.0f, panel.y + 88.0f + index * 56.0f, panel.width - 68.0f, 44.0f};
}

static Rectangle getHeuristicButtonBounds(int index) {
    const Rectangle panel = getConfigPanelBounds();
    return Rectangle{panel.x + 34.0f, panel.y + 366.0f + index * 44.0f, panel.width - 68.0f, 36.0f};
}

static bool shouldShowHeuristics(const GUIController& controller) {
    return controller.getSelectedAlgorithm() == "A*" ||
           controller.getSelectedAlgorithm() == "GBFS";
}

ConfigScreen::ConfigScreen()
    : boardCanvas(getBoardBounds()) {}

void ConfigScreen::update(GUIController& controller) {
    const char* algorithms[] = {"UCS", "GBFS", "A*", "IDA*"};
    const char* heuristics[] = {"H1", "H2", "H3", "H4", "H5"};

    boardCanvas.setBounds(getBoardBounds());

    for (int i = 0 ; i < 4 ; i++) {
        if (Button(algorithms[i], getAlgorithmButtonBounds(i)).isClicked()) {
            controller.setSelectedAlgorithm(algorithms[i]);
        }
    }

    if (shouldShowHeuristics(controller)) {
        for (int i = 0 ; i < 5 ; i++) {
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
    const char* heuristics[] = {"H1", "H2", "H3", "H4", "H5"};
    const Rectangle panel = getConfigPanelBounds();
    const int titleFontSize = 38;
    const int sectionFontSize = 26;

    ClearBackground(Color{22, 24, 30, 255});
    DrawText(
        title,
        static_cast<int>((getScreenWidthSafe() - MeasureText(title, titleFontSize)) / 2.0f),
        28,
        titleFontSize,
        Color{246, 248, 252, 255}
    );

    DrawRectangleRounded(panel, 0.05f, 12, Color{34, 38, 48, 255});
    DrawRectangleRoundedLines(panel, 0.05f, 12, Color{85, 94, 112, 255});
    DrawText(algorithmTitle, static_cast<int>(panel.x + 34.0f), static_cast<int>(panel.y + 34.0f), sectionFontSize, Color{246, 248, 252, 255});

    for (int i = 0 ; i < 4 ; i++) {
        Button(algorithms[i], getAlgorithmButtonBounds(i)).draw(controller.getSelectedAlgorithm() == algorithms[i]);
    }

    if (shouldShowHeuristics(controller)) {
        DrawText(heuristicTitle, static_cast<int>(panel.x + 34.0f), static_cast<int>(panel.y + 314.0f), sectionFontSize, Color{246, 248, 252, 255});
        for (int i = 0 ; i < 5 ; i++) {
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
        const Color textColor = message.find("Choose") != std::string::npos ?
                                Color{255, 144, 144, 255} :
                                Color{154, 230, 168, 255};

        DrawText(
            message.c_str(),
            static_cast<int>((getScreenWidthSafe() - textWidth) / 2.0f),
            static_cast<int>(getScreenHeightSafe() - 58.0f),
            fontSize,
            textColor
        );
    }
}
