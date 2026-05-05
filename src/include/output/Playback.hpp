#pragma once 

#include "model/Board.hpp"
#include "solver/SolverResult.hpp"

class Playback {
    public : 
        static void run(const Board& board, const SolverResult& result);

    private : 
        static void showStep(const Board& board, const SolverResult& result, int index);
};