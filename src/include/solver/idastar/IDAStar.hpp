#pragma once

#include "solver/SolverInput.hpp"
#include "solver/SolverResult.hpp"

class IDAStar {
    public:
        static SolverResult solve(const SolverInput& solverInput);
};
