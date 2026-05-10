#include "frontend/screen/NewGameScreen.hpp"

#include "frontend/ui/BoardCanvas.hpp"
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

static Rectangle getSizeConfigBounds() {
    return Rectangle{34.0f, 96.0f, 230.0f, getScreenHeightSafe() - 190.0f};
}

static Rectangle getPaletteBounds() {
    return Rectangle{getScreenWidthSafe() - 264.0f, 96.0f, 230.0f, getScreenHeightSafe() - 190.0f};
}

static Rectangle getBoardCanvasBounds() {
    return Rectangle{286.0f, 82.0f, getScreenWidthSafe() - 572.0f, getScreenHeightSafe() - 148.0f};
}

static Rectangle getEnterButtonBounds() {
    return Rectangle{getScreenWidthSafe() - 190.0f, getScreenHeightSafe() - 72.0f, 140.0f, 54.0f};
}

void NewGameScreen::update(GUIController& controller) {
    SizeConfig sizeConfig(getSizeConfigBounds());
    TilePalette tilePalette(getPaletteBounds());
    BoardCanvas boardCanvas(getBoardCanvasBounds());
    int clickedRow = -1;
    int clickedCol = -1;

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

    if (boardCanvas.getClickedCell(controller.getPaintRows(), controller.getPaintCols(), clickedRow, clickedCol)) {
        controller.paintTile(clickedRow, clickedCol);
    }

    if (Button("Enter", getEnterButtonBounds()).isClicked()) {
        controller.submitNewGame();
    }
}

void NewGameScreen::draw(const GUIController& controller) const {
    const char* title = "New Game";
    const int titleFontSize = 38;

    ClearBackground(Color{22, 24, 30, 255});
    DrawText(
        title,
        static_cast<int>((getScreenWidthSafe() - MeasureText(title, titleFontSize)) / 2.0f),
        28,
        titleFontSize,
        Color{246, 248, 252, 255}
    );

    SizeConfig(getSizeConfigBounds()).draw(controller.getPaintRows(), controller.getPaintCols());
    BoardCanvas(getBoardCanvasBounds()).draw(controller.getPaintBoard());
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
            static_cast<int>(getScreenHeightSafe() - 58.0f),
            fontSize,
            textColor
        );
    }
}
