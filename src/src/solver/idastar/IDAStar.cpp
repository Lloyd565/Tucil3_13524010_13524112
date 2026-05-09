#include "solver/idastar/IDAStar.hpp"

#include <chrono>
#include <limits>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "solver/Comparators.hpp"
#include "game/Movement.hpp"
#include "game/Rules.hpp"
#include "heuristic/astar/AStarHeuristic.hpp"

using namespace std;

enum Result {
    FOUND, 
    NOTFOUND
};

static Direction getDirectionFromMove(char move) {
    switch (move) {
        case 'U':
            return Direction::Up;
        case 'D':
            return Direction::Down;
        case 'L':
            return Direction::Left;
        case 'R':
            return Direction::Right;
        default:
            throw runtime_error("Error : invalid solution move");
    }
}

static vector<State> buildSolutionSteps(
    const Board& board, 
    const State& startState, 
    const string& moves
) {
    vector<State> tempSteps;
    State currentState = startState;

    tempSteps.push_back(currentState);

    for (char move : moves) {
        optional<State> nextState = Movement::slide(
            board,
            currentState,
            getDirectionFromMove(move)
        );

        if (!nextState.has_value()) throw runtime_error("Error : invalid solution path");

        currentState = nextState.value();
        tempSteps.push_back(currentState);

    }

    return tempSteps;
}

Result dfs(
    const Board& board, 
    const State& currentState, 
    int* threshold, 
    int* iterationCount,
    vector<State>* exploredStates,
    State* finalResult
) {
    int f = currentState.getTotalCost() + AStarHeuristic::compute(board, currentState, "H1");

    if (f > *threshold) return Result::NOTFOUND;

    if (Rules::isGoalState(board, currentState)) return Result::FOUND;

    int minNextThreshold = numeric_limits<int>::infinity();

    vector<State> possibleMoves = Movement::getPossibleMoves(board, currentState);

    for (State move : possibleMoves) {
        exploredStates->push_back(move);
        if (Rules::isGoalState(board, move)) {
            finalResult = &move;
            return Result::FOUND;
        }
        Result result = dfs(board, currentState, threshold, iterationCount, exploredStates, finalResult);
        if (result = Result::FOUND) return Result::FOUND;

        minNextThreshold = min(minNextThreshold, *threshold);
    }

    return Result::NOTFOUND;

}


SolverResult IDAStar::solve(const SolverInput& solverInput) {
    const Board& board = solverInput.getBoard();
    const State& initialState = solverInput.getInitialState();

    int threshold = 10;

    auto startTime = chrono::high_resolution_clock::now();

    int iterationCount = 0;
    vector<State> exploredStates;

    while (true) {
        State finalState;
        int cacheThreshold = threshold;
        Result result = dfs(board, initialState, &threshold, &iterationCount, &exploredStates, &finalState);
        
        if (result == Result::FOUND) {
            auto endTime = chrono::high_resolution_clock::now();
            long long execTime = chrono::duration_cast<chrono::milliseconds>(
                endTime - startTime
            ).count();

            return SolverResult(
            true, 
            finalState.getMoves(),
            finalState.getTotalCost(),
            iterationCount,
            execTime,
            buildSolutionSteps(board, finalState, finalState.getMoves()),
            exploredStates
            );
        }

        if (cacheThreshold == numeric_limits<int>::infinity()) {
            auto endTime = chrono::high_resolution_clock::now();
            long long execTime = chrono::duration_cast<chrono::milliseconds>(
                endTime - startTime
            ).count();

            return SolverResult::notFound(
            iterationCount, 
            execTime,
            exploredStates
            );
        }
    }

}