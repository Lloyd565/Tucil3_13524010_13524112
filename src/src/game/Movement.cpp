#include "game/Movement.hpp"

#include "game/Rules.hpp"

std::optional<State> Movement::slide(
    const Board& board,
    const State& state,
    Direction direction
) {
    int deltaRow = getDeltaRow(direction);
    int deltaCol = getDeltaCol(direction);

    Position current = state.getPlayerPosition();

    int totalCost = 0;

    int nextRequiredNumber = state.getNextRequiredNumber();

    bool moved = false;

    while (true) {
        Position next(current.getRow() + deltaRow, current.getCol() + deltaCol);
        if (!board.isInside(next)) return std::nullopt;
        
        char tile = board.getTile(next);

        if (Rules::isLava(tile)) return std::nullopt;
        if (Rules::isWall(tile)) return std::nullopt;

        State temp(current, nextRequiredNumber, state.getTotalCost() + totalCost, state.getMoves());

        if (!Rules::isValidNumberTile(tile, temp)) return std::nullopt;

        current = next;
        totalCost += board.getCost(current);
        moved = true;

        if (Rules::isNumberTile(tile)) {
            int number = Rules::charToNumber(tile);
            if (number == nextRequiredNumber) nextRequiredNumber++;
        }
    }

    if (!moved) return std::nullopt;
    return state.withMove(
        current, 
        nextRequiredNumber, 
        totalCost, 
        getMoveChar(direction)
    );
}

std::vector<State> Movement::getPossibleMoves(
    const Board& board,
    const State& state
) {
    std::vector<State> result;

    Direction directions[] = {
        Direction::Up,
        Direction::Down,
        Direction::Left,
        Direction::Right
    };

    for (Direction direction : directions) {
        std::optional<State> nextState = slide(board, state, direction);
        if (nextState.has_value()) result.push_back(nextState.value());
    }

    return result;
}

int Movement::getDeltaRow(Direction direction) {
    switch (direction) {
        case Direction::Up:
            return -1;
        case Direction::Down:
            return 1;
        case Direction::Left:
        case Direction::Right:
            return 0;
    }

    return 0;
}

int Movement::getDeltaCol(Direction direction) {
    switch (direction) {
        case Direction::Left:
            return -1;
        case Direction::Right:
            return 1;
        case Direction::Up:
        case Direction::Down:
            return 0;
    }

    return 0;
}

char Movement::getMoveChar(Direction direction) {
    switch (direction) {
        case Direction::Up:
            return 'U';
        case Direction::Down:
            return 'D';
        case Direction::Left:
            return 'L';
        case Direction::Right:
            return 'R';
    }

    return '?';
}
