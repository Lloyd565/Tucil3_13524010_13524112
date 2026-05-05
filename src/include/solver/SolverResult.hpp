#pragma once 

#include <string>
#include <vector>

#include "model/State.hpp"

class SolverResult {
    private : 
        bool found;
        std::string moves;
        int totalCost;
        int iterations;
        long long executionTime;

        std::vector<State> solutionSteps;
        std::vector<State> exploredStates;

    public : 
        SolverResult();
        SolverResult(
            bool found,
            const std::string& moves,
            int totalCost,
            int iterations,
            long long executionTimeMs,
            const std::vector<State>& solutionSteps,
            const std::vector<State>& exploredStates
        );

        static SolverResult notFound(
            int iterations, 
            long long executionTime, 
            const std::vector<State>& exploredStates 
        );

        bool isFound() const;
        const std::string& getMoves() const;
        int getTotalCost() const;
        int getIterations() const;
        long long getExecutionTime() const;

        const std::vector<State>& getSolutionSteps() const;
        const std::vector<State>& getExploredStates() const;
};