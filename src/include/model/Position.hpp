#pragma once 

class Position {
    private : 
        int row;
        int col;

    public : 
        Position();
        Position(int row, int col);

        int getRow() const;
        int getCol() const;

        Position moveCopyPosition(int row, int col) const;
        void moveOriginalPosition(int row, int col);

        bool operator==(const Position& other) const;
        bool operator!=(const Position& other) const;
};
