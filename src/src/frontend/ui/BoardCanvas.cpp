#include "frontend/ui/BoardCanvas.hpp"

#include <algorithm>
#include <cctype>
#include <string>

using namespace std;

BoardCanvas::BoardCanvas(Rectangle bounds)
    : bounds(bounds),
      isPainting(false),
      lastPaintedRow(-1),
      lastPaintedCol(-1) {}

void BoardCanvas::setBounds(Rectangle bounds) {
    this->bounds = bounds;
}

bool BoardCanvas::getPaintedCell(int rows, int cols, int& row, int& col) {
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        isPainting = false;
        lastPaintedRow = -1;
        lastPaintedCol = -1;
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        isPainting = true;
        lastPaintedRow = -1;
        lastPaintedCol = -1;
    }

    if (!isPainting || !IsMouseButtonDown(MOUSE_BUTTON_LEFT)) return false;
    if (!getCellAtMouse(rows, cols, row, col)) return false;
    if (row == lastPaintedRow && col == lastPaintedCol) return false;

    lastPaintedRow = row;
    lastPaintedCol = col;

    return true;
}

bool BoardCanvas::getCellAtMouse(int rows, int cols, int& row, int& col) const {
    if (rows <= 0 || cols <= 0) return false;
    if (bounds.width <= 0.0f || bounds.height <= 0.0f) return false;

    const float boardSize = min(bounds.width, bounds.height);
    const float tileSize = boardSize / max(rows, cols);
    const float boardWidth = tileSize * cols;
    const float boardHeight = tileSize * rows;
    const float boardX = bounds.x + (bounds.width - boardWidth) / 2.0f;
    const float boardY = bounds.y + (bounds.height - boardHeight) / 2.0f;
    const Vector2 mousePosition = GetMousePosition();
    const Rectangle boardBounds = {boardX, boardY, boardWidth, boardHeight};

    if (!CheckCollisionPointRec(mousePosition, boardBounds)) return false;

    row = static_cast<int>((mousePosition.y - boardY) / tileSize);
    col = static_cast<int>((mousePosition.x - boardX) / tileSize);

    return row >= 0 && row < rows && col >= 0 && col < cols;
}

void BoardCanvas::draw(const vector<string>& board, const vector<vector<int>>& costs) const {
    const int rows = board.size();
    const int cols = board.empty() ? 0 : board[0].size();
    if (rows == 0 || cols == 0) return;
    if (bounds.width <= 0.0f || bounds.height <= 0.0f) return;

    const float boardSize = min(bounds.width, bounds.height);
    const float tileSize = boardSize / max(rows, cols);
    const float boardWidth = tileSize * cols;
    const float boardHeight = tileSize * rows;
    const float boardX = bounds.x + (bounds.width - boardWidth) / 2.0f;
    const float boardY = bounds.y + (bounds.height - boardHeight) / 2.0f;

    DrawRectangleRounded(
        Rectangle{boardX - 8.0f, boardY - 8.0f, boardWidth + 16.0f, boardHeight + 16.0f},
        0.03f,
        8,
        Color{246, 248, 252, 255}
    );

    for (int row = 0 ; row < rows ; row++) {
        for (int col = 0 ; col < cols ; col++) {
            const float tileX = boardX + col * tileSize;
            const float tileY = boardY + row * tileSize;
            const int cost = row < (int) costs.size() && col < (int) costs[row].size() ? costs[row][col] : 1;

            drawTile(board[row][col], cost, tileX, tileY, tileSize);
            DrawRectangleLinesEx(
                Rectangle{tileX, tileY, tileSize, tileSize},
                1.0f,
                Color{72, 77, 88, 255}
            );
        }
    }
}

void BoardCanvas::drawTile(char tile, int cost, float x, float y, float size) const {
    Color tileColor = WHITE;

    if (tile == 'X') tileColor = Color{10, 10, 12, 255};
    else if (tile == 'L') tileColor = Color{220, 45, 45, 255};
    else if (tile == 'O') tileColor = Color{24, 150, 48, 255};
    else if (isdigit(static_cast<unsigned char>(tile))) tileColor = Color{250, 210, 60, 255};

    DrawRectangleRec(Rectangle{x, y, size, size}, tileColor);

    if (isdigit(static_cast<unsigned char>(tile))) {
        const int fontSize = static_cast<int>(size * 0.42f);
        const char text[2] = {tile, '\0'};
        const int textWidth = MeasureText(text, fontSize);
        const int textX = static_cast<int>(x + (size - textWidth) / 2.0f);
        const int textY = static_cast<int>(y + (size - fontSize) / 2.0f);

        DrawText(text, textX, textY, fontSize, Color{24, 29, 39, 255});
    }
    else if (tile == 'Z') {
        drawPlayer(x, y, size);
    }

    drawCost(tile, cost, x, y, size);
}

void BoardCanvas::drawCost(char tile, int cost, float x, float y, float size) const {
    const int fontSize = std::max(9, std::min(18, static_cast<int>(size * 0.18f)));
    const std::string text = std::to_string(cost);
    const Color textColor = (tile == 'X' || tile == 'L' || tile == 'O') ?
                            Color{246, 248, 252, 118} :
                            Color{24, 29, 39, 105};

    DrawText(
        text.c_str(),
        static_cast<int>(x + size * 0.07f),
        static_cast<int>(y + size - fontSize - size * 0.06f),
        fontSize,
        textColor
    );
}

void BoardCanvas::drawPlayer(float x, float y, float size) const {
    const float centerX = x + size / 2.0f;
    const float centerY = y + size / 2.0f;
    const float radius = size * 0.25f;

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
