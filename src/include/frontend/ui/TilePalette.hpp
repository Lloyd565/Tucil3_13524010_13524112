#pragma once

#include "raylib.h"

class TilePalette {
    private:
        Rectangle bounds;

        Rectangle getTileBounds(int index) const;
        void drawTileOption(const char* label, char tile, int index, char selectedTile) const;

    public:
        TilePalette(Rectangle bounds);

        char getClickedTile() const;
        void draw(char selectedTile) const;
};
