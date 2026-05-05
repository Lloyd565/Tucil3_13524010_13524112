#include "solver/SolverResult.hpp"

SolverResult::SolverResult()
    : found(false),
      moves(""),
      totalCost(0),
      iterations(0),
      executionTime(0),
      solutionSteps(),
      exploredStates() {}

SolverResult::SolverResult(
    bool found,
    const std::string& moves,
    int totalCost,
    int iterations,
    long long executionTimeMs,
    const std::vector<State>& solutionSteps,
    const std::vector<State>& exploredStates
) : found(found),
    moves(moves),
    totalCost(totalCost),
    iterations(iterations),
    executionTime(executionTimeMs),
    solutionSteps(solutionSteps),
    exploredStates(exploredStates) {}

SolverResult SolverResult::notFound(
    int iterations,
    long long executionTime,
    const std::vector<State>& exploredStates
) {
    return SolverResult(
        false,
        "",
        0,
        iterations,
        executionTime,
        std::vector<State>(),
        exploredStates
    );
}

bool SolverResult::isFound() const {
    return found;
}

const std::string& SolverResult::getMoves() const {
    return moves;
}

int SolverResult::getTotalCost() const {
    return totalCost;
}

int SolverResult::getIterations() const {
    return iterations;
}

long long SolverResult::getExecutionTime() const {
    return executionTime;
}

const std::vector<State>& SolverResult::getSolutionSteps() const {
    return solutionSteps;
}

const std::vector<State>& SolverResult::getExploredStates() const {
    return exploredStates;
}
