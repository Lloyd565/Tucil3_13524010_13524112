#pragma once 

#include "model/Board.hpp"
#include "model/State.hpp"
#include "solver/SolverResult.hpp"

class Printer {
    public : 
        static void printBoard(const Board& board, const State& state);
        static void printSolution(const Board& board, const SolverResult& result);
        static void printStep(const Board& board, const State& state, int stepNumber, char move);
        static void printFullStep(const Board& board, const SolverResult& result);
};