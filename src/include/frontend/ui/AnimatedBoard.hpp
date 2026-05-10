#pragma once

#include "raylib.h"

#include <string>
#include <utility>
#include <vector>

class AnimatedBoard {
    private:
        Rectangle bounds;

        void drawTile(char tile, float x, float y, float size, bool visitedNumberTile) const;
        void drawPlayer(float x, float y, float size) const;
        bool isVisitedNumberTile(
            const std::vector<std::string>& board,
            const std::vector<std::pair<int, int>>& path,
            int playbackIndex,
            float playbackProgress,
            int row,
            int col
        ) const;

    public:
        AnimatedBoard(Rectangle bounds);

        void setBounds(Rectangle bounds);
        void draw(
            const std::vector<std::string>& board,
            const std::vector<std::pair<int, int>>& path,
            int playbackIndex,
            float playbackProgress
        ) const;
};
