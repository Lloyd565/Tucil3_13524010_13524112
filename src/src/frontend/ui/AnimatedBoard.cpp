#include "frontend/ui/AnimatedBoard.hpp"

#include <algorithm>
#include <cctype>

using namespace std;

AnimatedBoard::AnimatedBoard(Rectangle bounds)
    : bounds(bounds) {}

void AnimatedBoard::setBounds(Rectangle bounds) {
    this->bounds = bounds;
}

void AnimatedBoard::draw(
    const vector<string>& board,
    const vector<pair<int, int>>& path,
    int playbackIndex,
    float playbackProgress
) const {
    const int rows = board.size();
    const int cols = board.empty() ? 0 : board[0].size();
    if (rows == 0 || cols == 0 || bounds.width <= 0.0f || bounds.height <= 0.0f) return;

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
            char tile = board[row][col];
            if (tile == 'Z') tile = '*';

            const float tileX = boardX + col * tileSize;
            const float tileY = boardY + row * tileSize;

            drawTile(tile, tileX, tileY, tileSize);
            DrawRectangleLinesEx(Rectangle{tileX, tileY, tileSize, tileSize}, 1.0f, Color{72, 77, 88, 255});
        }
    }

    if (!path.empty()) {
        const int currentIndex = max(0, min(playbackIndex, (int) path.size() - 1));
        const int nextIndex = max(0, min(currentIndex + 1, (int) path.size() - 1));
        const float clampedProgress = max(0.0f, min(playbackProgress, 1.0f));
        const float currentRow = path[currentIndex].first;
        const float currentCol = path[currentIndex].second;
        const float nextRow = path[nextIndex].first;
        const float nextCol = path[nextIndex].second;
        const float drawRow = currentRow + (nextRow - currentRow) * clampedProgress;
        const float drawCol = currentCol + (nextCol - currentCol) * clampedProgress;

        drawPlayer(boardX + drawCol * tileSize, boardY + drawRow * tileSize, tileSize);
    }
}

void AnimatedBoard::drawTile(char tile, float x, float y, float size) const {
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

        DrawText(
            text,
            static_cast<int>(x + (size - textWidth) / 2.0f),
            static_cast<int>(y + (size - fontSize) / 2.0f),
            fontSize,
            Color{24, 29, 39, 255}
        );
    }
}

void AnimatedBoard::drawPlayer(float x, float y, float size) const {
    const float centerX = x + size / 2.0f;
    const float centerY = y + size / 2.0f;
    const float radius = size * 0.25f;

    DrawCircle(static_cast<int>(centerX), static_cast<int>(centerY), radius, Color{33, 125, 220, 255});
    DrawCircle(static_cast<int>(centerX), static_cast<int>(centerY - radius * 0.15f), radius * 0.72f, Color{245, 248, 255, 255});
    DrawCircle(static_cast<int>(centerX - radius * 0.22f), static_cast<int>(centerY - radius * 0.18f), radius * 0.09f, Color{12, 24, 38, 255});
    DrawCircle(static_cast<int>(centerX + radius * 0.22f), static_cast<int>(centerY - radius * 0.18f), radius * 0.09f, Color{12, 24, 38, 255});
    DrawRectangleRounded(
        Rectangle{centerX - radius * 0.45f, centerY + radius * 0.14f, radius * 0.9f, radius * 0.14f},
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
