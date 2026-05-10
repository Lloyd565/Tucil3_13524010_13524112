#pragma once

#include <string>
#include "solver/SolverInput.hpp"
#include "solver/SolverResult.hpp"

class GBFS {
    public:
        static SolverResult solve(const SolverInput& solverInput, const std::string& heuristic);
};
