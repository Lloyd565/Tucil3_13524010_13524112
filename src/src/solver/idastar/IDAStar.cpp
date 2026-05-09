#include "solver/idastar/IDAStar.hpp"

#include <chrono>
#include <limits>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

#include "game/Movement.hpp"
#include "game/Rules.hpp"
#include "heuristic/astar/AStarHeuristic.hpp"

using namespace std;

static const int SEARCH_FOUND = -1;

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

static bool isOnPath(
    const vector<State>& path,
    const State& state
) {
    for (const State& pathState : path) {
        if (pathState == state) return true;
    }
    return false;
}

static int dfs(
    const Board& board, 
    const State& currentState, 
    const string& heuristic,
    int threshold, 
    int& iterationCount,
    vector<State>& exploredStates,
    vector<State>& path,
    State& finalResult
) {
    int f = currentState.getTotalCost() + AStarHeuristic::compute(board, currentState, heuristic);

    if (f > threshold) return f;

    iterationCount++;
    exploredStates.push_back(currentState);

    if (Rules::isGoalState(board, currentState)) {
        finalResult = currentState;
        return SEARCH_FOUND;
    }

    int minNextThreshold = numeric_limits<int>::max();

    vector<State> possibleMoves = Movement::getPossibleMoves(board, currentState);
    for (const State& nextState : possibleMoves) {
        if (isOnPath(path, nextState)) {
            continue;
        }

        path.push_back(nextState);
        int result = dfs(
            board,
            nextState,
            heuristic,
            threshold,
            iterationCount,
            exploredStates,
            path,
            finalResult
        );
        path.pop_back();

        if (result == SEARCH_FOUND) return SEARCH_FOUND;
        if (result < minNextThreshold) minNextThreshold = result;
    }

    return minNextThreshold;
}

SolverResult IDAStar::solve(const SolverInput& solverInput, const string& heuristic) {
    const Board& board = solverInput.getBoard();
    const State& initialState = solverInput.getInitialState();

    int threshold = AStarHeuristic::compute(board, initialState, heuristic);

    auto startTime = chrono::high_resolution_clock::now();

    int iterationCount = 0;
    vector<State> exploredStates;

    while (true) {
        State finalState;
        vector<State> path;
        path.push_back(initialState);

        int result = dfs(
            board,
            initialState,
            heuristic,
            threshold,
            iterationCount,
            exploredStates,
            path,
            finalState
        );
        
        if (result == SEARCH_FOUND) {
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
                buildSolutionSteps(board, initialState, finalState.getMoves()),
                exploredStates
            );
        }

        if (result == numeric_limits<int>::max()) {
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

        threshold = result;
    }

}
