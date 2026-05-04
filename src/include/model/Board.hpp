#pragma once;

#include <vector>
#include "model/Position.hpp"

class Board {
    private : 
        int rows;
        int cols;

        std::vector<std::vector<char>> grid;
        std::vector<std::vector<int>> costs;

        Position startPosition;
        Position goalPosition;

        int maxNumber;

    public : 
        Board();
        Board(
            int rows, 
            int cols, 
            std::vector<std::vector<char>> grid,
            std::vector<std::vector<int>> costs,
            Position startPosition,
            Position goalPosition, 
            int maxNumber
        );
        int getRows() const;
        int getCols() const;

        char getTile(const Position& position) const;
        int getCost(const Position& position) const;

        int getMaxNumber() const;

        bool isInside(const Position& position) const;
};