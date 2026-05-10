#pragma once

#include "model/Board.hpp"
#include "model/State.hpp"

class GBFSHeuristic {
    public:
        static int compute(const Board& board, const State& state);
};
