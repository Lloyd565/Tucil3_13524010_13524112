#include "frontend/ui/BoardView.hpp"

#include "raylib.h"

#include <algorithm>
#include <cctype>

using namespace std;

BoardView::BoardView()
    : tiles({
          "XXXXXXX",
          "X***2*X",
          "X**X**X",
          "X*L**OX",
          "X****3X",
          "X**X**X",
          "XXXXXXX"
      }),
      playerRow(3),
      playerCol(1) {}

void BoardView::draw(float x, float y, float width, float height) const {
    const int rows = tiles.size();
    const int cols = tiles.empty() ? 0 : tiles[0].size();
    if (rows == 0 || cols == 0) return;

    const float boardSize = min(width, height);
    const float tileSize = boardSize / max(rows, cols);
    const float boardWidth = tileSize * cols;
    const float boardHeight = tileSize * rows;
    const float boardX = x + (width - boardWidth) / 2.0f;
    const float boardY = y + (height - boardHeight) / 2.0f;

    DrawRectangleRec(
        Rectangle{boardX - 2.0f, boardY - 2.0f, boardWidth + 4.0f, boardHeight + 4.0f},
        Color{230, 232, 236, 255}
    );

    for (int row = 0 ; row < rows ; row++) {
        for (int col = 0 ; col < cols ; col++) {
            const float tileX = boardX + col * tileSize;
            const float tileY = boardY + row * tileSize;

            drawTile(tiles[row][col], tileX, tileY, tileSize);
            DrawRectangleLinesEx(
                Rectangle{tileX, tileY, tileSize, tileSize},
                1.0f,
                Color{58, 60, 66, 255}
            );
        }
    }

    drawPlayer(
        boardX + playerCol * tileSize,
        boardY + playerRow * tileSize,
        tileSize
    );
}

void BoardView::drawTile(char tile, float x, float y, float size) const {
    Color tileColor = WHITE;

    if (tile == 'X') tileColor = Color{10, 10, 12, 255};
    else if (tile == 'L') tileColor = Color{220, 45, 45, 255};
    else if (isdigit(static_cast<unsigned char>(tile))) tileColor = Color{250, 210, 60, 255};
    else if (tile == 'O') tileColor = Color{24, 150, 48, 255};

    DrawRectangleRec(Rectangle{x, y, size, size}, tileColor);

    if (isdigit(static_cast<unsigned char>(tile))) {
        const int fontSize = static_cast<int>(size * 0.44f);
        const string numberText(1, tile);
        const int textWidth = MeasureText(numberText.c_str(), fontSize);
        const int textX = static_cast<int>(x + (size - textWidth) / 2.0f);
        const int textY = static_cast<int>(y + (size - fontSize) / 2.0f);

        DrawText(numberText.c_str(), textX, textY, fontSize, Color{36, 30, 8, 255});
    }
}

void BoardView::drawPlayer(float x, float y, float size) const {
    const float centerX = x + size / 2.0f;
    const float centerY = y + size / 2.0f;
    const float radius = size * 0.28f;

    DrawCircle(static_cast<int>(centerX), static_cast<int>(centerY), radius, Color{33, 125, 220, 255});
    DrawCircle(static_cast<int>(centerX), static_cast<int>(centerY - radius * 0.15f), radius * 0.72f, Color{245, 248, 255, 255});
    DrawCircle(static_cast<int>(centerX - radius * 0.22f), static_cast<int>(centerY - radius * 0.18f), radius * 0.09f, Color{12, 24, 38, 255});
    DrawCircle(static_cast<int>(centerX + radius * 0.22f), static_cast<int>(centerY - radius * 0.18f), radius * 0.09f, Color{12, 24, 38, 255});
    DrawRectangleRounded(
        Rectangle{
            centerX - radius * 0.45f,
            centerY + radius * 0.14f,
            radius * 0.9f,
            radius * 0.14f
        },
        0.5f,
        8,
        Color{12, 24, 38, 255}
    );
    DrawTriangle(
        Vector2{centerX + radius * 0.6f, centerY - radius * 0.95f},
        Vector2{centerX + radius * 1.1f, centerY - radius * 0.25f},
        Vector2{centerX + radius * 0.38f, centerY - radius * 0.38f},
        Color{255, 132, 64, 255}
    );
}
