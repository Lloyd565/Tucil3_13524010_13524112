#include "solver/astar/AStar.hpp"
#include "game/Movement.hpp"
#include "game/Rules.hpp"
#include "heuristic/astar/AStarHeuristic.hpp"

#include <chrono>
#include <map>
#include <queue>
#include <tuple>
#include <vector>

using namespace std;
using StateKey = tuple<int, int, int>;

static StateKey makeKey(const State& state) {
    return {
        state.getPlayerPosition().getRow(),
        state.getPlayerPosition().getCol(),
        state.getNextRequiredNumber()
    };
}

static Direction charToDirection(char c) {
    switch (c) {
        case 'U': return Direction::Up;
        case 'D': return Direction::Down;
        case 'L': return Direction::Left;
        default:  return Direction::Right;
    }
}

static vector<State> reconstructPath(
    const Board& board,
    const State& initial,
    const State& goal
) {
    vector<State> steps;
    steps.push_back(initial);

    State current = initial;
    for (char c : goal.getMoves()) {
        auto next = Movement::slide(board, current, charToDirection(c));
        if (next.has_value()) {
            steps.push_back(next.value());
            current = next.value();
        }
    }

    return steps;
}

SolverResult AStar::solve(const SolverInput& solverInput, const string& heuristic) {
    const Board& board = solverInput.getBoard();
    const State& initial = solverInput.getInitialState();

    auto startTime = chrono::high_resolution_clock::now();

    using PQElement = pair<int, State>;
    auto cmp = [](const PQElement& a, const PQElement& b) {
        return a.first > b.first;
    };
    priority_queue<PQElement, vector<PQElement>, decltype(cmp)> pq(cmp);

    map<StateKey, int> bestG;
    vector<State> exploredStates;
    int iterations = 0;

    int initH = AStarHeuristic::compute(board, initial, heuristic);
    pq.push({initH, initial});
    bestG[makeKey(initial)] = 0;

    while (!pq.empty()) {
        auto [f, state] = pq.top();
        pq.pop();

        StateKey key = makeKey(state);
        int g = state.getTotalCost();

        if (bestG.count(key) && bestG[key] < g) continue;

        exploredStates.push_back(state);
        iterations++;

        if (Rules::isGoalState(board, state)) {
            auto endTime = chrono::high_resolution_clock::now();
            long long ms = chrono::duration_cast<chrono::milliseconds>(
                endTime - startTime).count();

            return SolverResult(
                true,
                state.getMoves(),
                state.getTotalCost(),
                iterations,
                ms,
                reconstructPath(board, initial, state),
                exploredStates
            );
        }

        for (const State& next : Movement::getPossibleMoves(board, state)) {
            StateKey nextKey = makeKey(next);
            int nextG = next.getTotalCost();

            if (!bestG.count(nextKey) || nextG < bestG[nextKey]) {
                bestG[nextKey] = nextG;
                int nextH = AStarHeuristic::compute(board, next, heuristic);
                pq.push({nextG + nextH, next});
            }
        }
    }

    auto endTime = chrono::high_resolution_clock::now();
    long long ms = chrono::duration_cast<chrono::milliseconds>(
        endTime - startTime).count();

    return SolverResult::notFound(iterations, ms, exploredStates);
}
