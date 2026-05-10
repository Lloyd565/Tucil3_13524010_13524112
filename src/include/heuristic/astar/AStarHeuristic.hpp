#pragma once

#include <string>
#include "model/Board.hpp"
#include "model/State.hpp"

class AStarHeuristic {
    public:
        static int compute(const Board& board, const State& state, const std::string& heuristic);

    private:
        static int h1(const Board& board, const State& state);
        static int h2(const Board& board, const State& state);
        static int h3(const Board& board, const State& state);
        static int h4(const Board& board, const State& state);
        static int h5(const Board& board, const State& state);
};
