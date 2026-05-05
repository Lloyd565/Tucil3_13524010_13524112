#pragma once

#include "solver/SolverInput.hpp"
#include "solver/SolverResult.hpp"

class UCS {
    public:
        static SolverResult solve(const SolverInput& solverInput);
};
