#include "frontend/screen/NewGameScreen.hpp"

#include "frontend/ui/Button.hpp"
#include "frontend/ui/SizeConfig.hpp"
#include "frontend/ui/TilePalette.hpp"
#include "raylib.h"

#include <algorithm>

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

static Rectangle getSizeConfigBounds() {
    return Rectangle{34.0f * scaleX(), 96.0f * scaleY(), 230.0f * scaleX(), getScreenHeightSafe() - 190.0f * scaleY()};
}

static Rectangle getPaletteBounds() {
    return Rectangle{getScreenWidthSafe() - 264.0f * scaleX(), 96.0f * scaleY(), 230.0f * scaleX(), getScreenHeightSafe() - 190.0f * scaleY()};
}

static Rectangle getBoardCanvasBounds() {
    return Rectangle{286.0f * scaleX(), 82.0f * scaleY(), getScreenWidthSafe() - 572.0f * scaleX(), getScreenHeightSafe() - 148.0f * scaleY()};
}

static Rectangle getEnterButtonBounds() {
    return Rectangle{getScreenWidthSafe() - 190.0f * scaleX(), getScreenHeightSafe() - 72.0f * scaleY(), 140.0f * scaleX(), 54.0f * scaleY()};
}

static Rectangle getBackButtonBounds() {
    return Rectangle{34.0f * scaleX(), 28.0f * scaleY(), 120.0f * scaleX(), 48.0f * scaleY()};
}

NewGameScreen::NewGameScreen()
    : boardCanvas(getBoardCanvasBounds()) {}

void NewGameScreen::update(GUIController& controller) {
    SizeConfig sizeConfig(getSizeConfigBounds());
    TilePalette tilePalette(getPaletteBounds());
    int clickedRow = -1;
    int clickedCol = -1;

    boardCanvas.setBounds(getBoardCanvasBounds());

    if (Button("Back", getBackButtonBounds()).isClicked()) {
        controller.cancelNewGame();
        return;
    }

    const int rowDelta = sizeConfig.getRowDelta();
    const int colDelta = sizeConfig.getColDelta();
    if (rowDelta != 0 || colDelta != 0) {
        controller.setPaintBoardSize(
            controller.getPaintRows() + rowDelta,
            controller.getPaintCols() + colDelta
        );
    }

    const char selectedTile = tilePalette.getClickedTile();
    if (selectedTile != '\0') {
        controller.setSelectedPaintTile(selectedTile);
    }

    if (boardCanvas.getPaintedCell(controller.getPaintRows(), controller.getPaintCols(), clickedRow, clickedCol)) {
        controller.paintTile(clickedRow, clickedCol);
    }

    if (Button("Enter", getEnterButtonBounds()).isClicked()) {
        controller.submitNewGame();
    }
}

void NewGameScreen::draw(const GUIController& controller) const {
    const char* title = "New Game";
    const int titleFontSize = static_cast<int>(38.0f * std::min(scaleX(), scaleY()));

    ClearBackground(Color{22, 24, 30, 255});
    DrawText(
        title,
        static_cast<int>((getScreenWidthSafe() - MeasureText(title, titleFontSize)) / 2.0f),
        static_cast<int>(28.0f * scaleY()),
        titleFontSize,
        Color{246, 248, 252, 255}
    );

    Button("Back", getBackButtonBounds()).draw();
    SizeConfig(getSizeConfigBounds()).draw(controller.getPaintRows(), controller.getPaintCols());
    boardCanvas.draw(controller.getPaintBoard(), controller.getPaintCosts());
    TilePalette(getPaletteBounds()).draw(controller.getSelectedPaintTile());
    Button("Enter", getEnterButtonBounds()).draw();

    if (!controller.getNewGameMessage().empty()) {
        const std::string& message = controller.getNewGameMessage();
        const int fontSize = 22;
        const int textWidth = MeasureText(message.c_str(), fontSize);
        const Color textColor = message.find("must") != std::string::npos ?
                                Color{255, 144, 144, 255} :
                                Color{154, 230, 168, 255};

        DrawText(
            message.c_str(),
            static_cast<int>((getScreenWidthSafe() - textWidth) / 2.0f),
            static_cast<int>(getScreenHeightSafe() - 58.0f * scaleY()),
            fontSize,
            textColor
        );
    }
}
