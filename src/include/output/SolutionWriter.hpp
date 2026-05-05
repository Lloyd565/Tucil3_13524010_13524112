#pragma once 

#include <fstream>
#include <string>

#include "model/Board.hpp"
#include "model/State.hpp"
#include "solver/SolverResult.hpp"

class SolutionWriter {
    public : 
        static void save (
            const std::string& filePath,
            const Board& board,
            const SolverResult& result,
            const std::string& algorithmName,
            const std::string& heuristicName = "-"
        );

    private : 
        static void writeBoard(
            std::ofstream& out,
            const Board& board,
            const State& state
        );

        static void writeSolutionSteps(
            std::ofstream& out,
            const Board& board,
            const SolverResult& result
        );
};
