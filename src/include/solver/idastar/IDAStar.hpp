#pragma once

#include "solver/SolverInput.hpp"
#include "solver/SolverResult.hpp"
#include <string>

class IDAStar {
    public:
        static SolverResult solve(const SolverInput& solverInput, const std::string& heuristic);
};
