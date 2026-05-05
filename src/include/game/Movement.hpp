#pragma once 

#include <optional>
#include <vector>

#include "model/Board.hpp"
#include "model/State.hpp"

enum class Direction {
    Up, 
    Down, 
    Left, 
    Right
};

class Movement {
    public : 
        static std::optional<State> slide(
            const Board& board,
            const State& state, 
            Direction direction
        );

        static std::vector<State> getPossibleMoves(
            const Board& board, 
            const State& state
        );

    private : 
        static int getDeltaRow(Direction direction);
        static int getDeltaCol(Direction direction);
        static char getMoveChar(Direction direction);
};