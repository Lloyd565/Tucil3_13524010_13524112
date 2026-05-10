#include "frontend/ui/AnimatedBoard.hpp"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <string>

using namespace std;

AnimatedBoard::AnimatedBoard(Rectangle bounds)
    : bounds(bounds) {}

void AnimatedBoard::setBounds(Rectangle bounds) {
    this->bounds = bounds;
}

void AnimatedBoard::draw(
    const vector<string>& board,
    const vector<vector<int>>& costs,
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
            const int cost = row < (int) costs.size() && col < (int) costs[row].size() ? costs[row][col] : 1;

            drawTile(
                tile,
                cost,
                tileX,
                tileY,
                tileSize,
                isVisitedNumberTile(board, path, playbackIndex, playbackProgress, row, col)
            );
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

void AnimatedBoard::drawTile(char tile, int cost, float x, float y, float size, bool visitedNumberTile) const {
    Color tileColor = WHITE;

    if (tile == 'X') tileColor = Color{10, 10, 12, 255};
    else if (tile == 'L') tileColor = Color{220, 45, 45, 255};
    else if (tile == 'O') tileColor = Color{24, 150, 48, 255};
    else if (visitedNumberTile) tileColor = Color{246, 241, 202, 255};
    else if (isdigit(static_cast<unsigned char>(tile))) tileColor = Color{250, 210, 60, 255};

    DrawRectangleRec(Rectangle{x, y, size, size}, tileColor);

    if (isdigit(static_cast<unsigned char>(tile))) {
        const int fontSize = static_cast<int>(size * 0.42f);
        const char text[2] = {tile, '\0'};
        const int textWidth = MeasureText(text, fontSize);
        const Color textColor = visitedNumberTile ?
                                Color{118, 113, 76, 255} :
                                Color{24, 29, 39, 255};

        DrawText(
            text,
            static_cast<int>(x + (size - textWidth) / 2.0f),
            static_cast<int>(y + (size - fontSize) / 2.0f),
            fontSize,
            textColor
        );
    }

    drawCost(tile, cost, x, y, size);
}

void AnimatedBoard::drawCost(char tile, int cost, float x, float y, float size) const {
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

bool AnimatedBoard::isVisitedNumberTile(
    const vector<string>& board,
    const vector<pair<int, int>>& path,
    int playbackIndex,
    float playbackProgress,
    int row,
    int col
) const {
    if (!isdigit(static_cast<unsigned char>(board[row][col]))) return false;
    if (path.size() <= 1) return false;

    const int currentIndex = max(0, min(playbackIndex, (int) path.size() - 1));
    const float clampedProgress = max(0.0f, min(playbackProgress, 1.0f));

    for (int i = 0 ; i < currentIndex ; i++) {
        const int startRow = path[i].first;
        const int startCol = path[i].second;
        const int endRow = path[i + 1].first;
        const int endCol = path[i + 1].second;

        if (startRow == endRow && row == startRow && col >= min(startCol, endCol) && col <= max(startCol, endCol)) return true;
        if (startCol == endCol && col == startCol && row >= min(startRow, endRow) && row <= max(startRow, endRow)) return true;
    }

    if (currentIndex >= (int) path.size() - 1) return false;

    const int startRow = path[currentIndex].first;
    const int startCol = path[currentIndex].second;
    const int endRow = path[currentIndex + 1].first;
    const int endCol = path[currentIndex + 1].second;
    const int rowDelta = (endRow > startRow) - (endRow < startRow);
    const int colDelta = (endCol > startCol) - (endCol < startCol);
    const int distance = abs(endRow - startRow) + abs(endCol - startCol);
    const int reachedDistance = static_cast<int>(distance * clampedProgress);

    for (int step = 0 ; step <= reachedDistance ; step++) {
        if (row == startRow + rowDelta * step && col == startCol + colDelta * step) return true;
    }

    return false;
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
