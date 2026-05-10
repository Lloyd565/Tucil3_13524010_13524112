#include "solver/idastar/IDAStar.hpp"

#include <chrono>
#include <limits>
#include <optional>
#include <set>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

#include "game/Movement.hpp"
#include "game/Rules.hpp"
#include "heuristic/astar/AStarHeuristic.hpp"

using namespace std;

using StateKey = tuple<int, int, int>;

struct SearchResult {
    bool found;
    int nextThreshold;
};

static StateKey makeKey(const State& state) {
    return {
        state.getPlayerPosition().getRow(),
        state.getPlayerPosition().getCol(),
        state.getNextRequiredNumber()
    };
}

static int computeHeuristic(
    const Board& board,
    const State& state,
    const string& heuristic
) {
    return AStarHeuristic::compute(board, state, heuristic);
}

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

static SearchResult dfs(
    const Board& board, 
    const State& currentState, 
    const string& heuristic,
    int threshold, 
    int& iterationCount,
    vector<State>& exploredStates,
    set<StateKey>& pathStates,
    State& finalResult
) {
    int f = currentState.getTotalCost() + computeHeuristic(board, currentState, heuristic);

    if (f > threshold) return {false, f};

    iterationCount++;
    exploredStates.push_back(currentState);

    if (Rules::isGoalState(board, currentState)) {
        finalResult = currentState;
        return {true, threshold};
    }

    int minNextThreshold = numeric_limits<int>::max();

    vector<State> possibleMoves = Movement::getPossibleMoves(board, currentState);
    for (const State& nextState : possibleMoves) {
        StateKey nextKey = makeKey(nextState);
        if (pathStates.count(nextKey)) continue;

        pathStates.insert(nextKey);
        SearchResult result = dfs(
            board,
            nextState,
            heuristic,
            threshold,
            iterationCount,
            exploredStates,
            pathStates,
            finalResult
        );
        pathStates.erase(nextKey);

        if (result.found) return result;
        if (result.nextThreshold < minNextThreshold) minNextThreshold = result.nextThreshold;
    }

    return {false, minNextThreshold};
}

SolverResult IDAStar::solve(const SolverInput& solverInput, const string& heuristic) {
    const Board& board = solverInput.getBoard();
    const State& initialState = solverInput.getInitialState();

    int threshold = computeHeuristic(board, initialState, heuristic);

    auto startTime = chrono::high_resolution_clock::now();

    int iterationCount = 0;
    vector<State> exploredStates;

    while (true) {
        State finalState;
        set<StateKey> pathStates;
        pathStates.insert(makeKey(initialState));

        SearchResult result = dfs(
            board,
            initialState,
            heuristic,
            threshold,
            iterationCount,
            exploredStates,
            pathStates,
            finalState
        );
        
        if (result.found) {
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

        if (result.nextThreshold == numeric_limits<int>::max()) {
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

        threshold = result.nextThreshold;
    }

}
