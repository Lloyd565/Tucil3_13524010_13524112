#pragma once

#include "raylib.h"

#include <string>
#include <vector>

class BoardCanvas {
    private:
        Rectangle bounds;

        void drawTile(char tile, float x, float y, float size) const;
        void drawPlayer(float x, float y, float size) const;

    public:
        BoardCanvas(Rectangle bounds);

        bool getClickedCell(int rows, int cols, int& row, int& col) const;
        void draw(const std::vector<std::string>& board) const;
};
