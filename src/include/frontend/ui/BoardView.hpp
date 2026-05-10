#pragma once

#include <string>
#include <vector>

class BoardView {
    private:
        std::vector<std::string> tiles;
        int playerRow;
        int playerCol;

        void drawTile(char tile, float x, float y, float size) const;
        void drawPlayer(float x, float y, float size) const;

    public:
        BoardView();

        void draw(float x, float y, float width, float height) const;
};
