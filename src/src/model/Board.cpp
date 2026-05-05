#include "model/Board.hpp"

using namespace std;

Board::Board()
    : rows(0),
      cols(0),
      grid(),
      costs(),
      startPosition(),
      goalPosition(),
      maxNumber(0) {}

Board::Board(
    int rows,
    int cols,
    const vector<vector<char>>& grid,
    const vector<vector<int>>& costs,
    Position startPosition,
    Position goalPosition,
    int maxNumber
) : rows(rows),
    cols(cols),
    grid(grid),
    costs(costs),
    startPosition(startPosition),
    goalPosition(goalPosition),
    maxNumber(maxNumber) {}

int Board::getRows() const {
    return rows;
}

int Board::getCols() const {
    return cols;
}

char Board::getTile(const Position& position) const {
    return grid[position.getRow()][position.getCol()];
}

int Board::getCost(const Position& position) const {
    return costs[position.getRow()][position.getCol()];
}

const Position& Board::getStartPosition() const {
    return startPosition;
}

const Position& Board::getGoalPosition() const {
    return goalPosition;
}

int Board::getMaxNumber() const {
    return maxNumber;
}

bool Board::isInside(const Position& position) const {
    return position.getRow() >= 0 &&
           position.getRow() < rows &&
           position.getCol() >= 0 &&
           position.getCol() < cols;
}
