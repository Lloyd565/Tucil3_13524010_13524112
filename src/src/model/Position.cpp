#include "model/Position.hpp"

using namespace std;

Position::Position() : row(0), col(0) {}

Position::Position(int row, int col) : row(row), col(col) {}

int Position::getRow() const {
    return row;
}

int Position::getCol() const {
    return col;
}

Position Position::moveCopyPosition(int row, int col) const {
    return Position(row, col);
}

void Position::moveOriginalPosition(int row, int col) {
    this->row = row;
    this->col = col;
}

bool Position::operator==(const Position& other) const {
    return row == other.row && col == other.col;
}

bool Position::operator!=(const Position& other) const {
    return !(*this == other);
}
