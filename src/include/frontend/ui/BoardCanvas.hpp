#pragma once

#include "raylib.h"

#include <string>
#include <vector>

class BoardCanvas {
    private:
        Rectangle bounds;
        bool isPainting;
        int lastPaintedRow;
        int lastPaintedCol;

        bool getCellAtMouse(int rows, int cols, int& row, int& col) const;
        void drawTile(char tile, float x, float y, float size) const;
        void drawPlayer(float x, float y, float size) const;

    public:
        BoardCanvas(Rectangle bounds);

        void setBounds(Rectangle bounds);
        bool getPaintedCell(int rows, int cols, int& row, int& col);
        void draw(const std::vector<std::string>& board) const;
};
