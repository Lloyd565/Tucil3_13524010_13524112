#include "game/Rules.hpp"

#include <cctype>

bool Rules::isWall(char tile) {
    return tile == 'X';
}

bool Rules::isLava(char tile) {
    return tile == 'L';
}

bool Rules::isGoal(char tile) {
    return tile == 'O';
}

bool Rules::isNumberTile(char tile) {
    return tile >= '0' && tile <= '9';
}

int Rules::charToNumber(char tile) {
    if (!isNumberTile(tile)) {
        return -1;
    }

    return tile - '0';
}

bool Rules::isValidNumberTile(char tile, const State& state) {
    if (!isNumberTile(tile)) return true;
    if (charToNumber(tile) < state.getNextRequiredNumber()) return true;
    if (charToNumber(tile) == state.getNextRequiredNumber()) return true;
    return false;
}

bool Rules::isGoalState(const Board& board, const State& state) {
    const Position& position = state.getPlayerPosition();

    if (!isGoal(board.getTile(position))) return false;
    return state.getNextRequiredNumber() > board.getMaxNumber();
}
