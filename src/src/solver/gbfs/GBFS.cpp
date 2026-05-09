#include "solver/gbfs/GBFS.hpp"
#include "game/Movement.hpp"
#include "game/Rules.hpp"
#include "heuristic/astar/AStarHeuristic.hpp"
#include "model/PriorityQueue.hpp"
#include "solver/Comparators.hpp"

#include <chrono>
#include <set>
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

static vector<State> reconstructPath(
    const Board& board,
    const State& initial,
    const State& goal
) {
    vector<State> steps;
    steps.push_back(initial);

    State current = initial;
    for (char c : goal.getMoves()) {
        auto next = Movement::slide(board, current, Movement::charToDirection(c));
        if (next.has_value()) {
            steps.push_back(next.value());
            current = next.value();
        }
    }

    return steps;
}

SolverResult GBFS::solve(const SolverInput& solverInput, const string& heuristic) {
    const Board& board = solverInput.getBoard();
    const State& initial = solverInput.getInitialState();

    auto startTime = chrono::high_resolution_clock::now();

    PriorityQueue<CompareGBFS> pq;

    set<StateKey> visited;
    vector<State> exploredStates;
    int iterations = 0;

    int initH = AStarHeuristic::compute(board, initial, heuristic);
    pq.push(initial, initH);

    while (!pq.empty()) {
        State state = pq.pop();

        StateKey key = makeKey(state);
        if (visited.count(key)) continue;
        visited.insert(key);

        exploredStates.push_back(state);
        iterations++;

        if (Rules::isGoalState(board, state)) {
            auto endTime = chrono::high_resolution_clock::now();
            long long ms = chrono::duration_cast<chrono::milliseconds>(
                endTime - startTime).count();

            return SolverResult(true,state.getMoves(),state.getTotalCost(),iterations,ms,reconstructPath(board, initial, state),exploredStates);
        }

        for (const State& next : Movement::getPossibleMoves(board, state)) {
            StateKey nextKey = makeKey(next);
            if (!visited.count(nextKey)) {
                int nextH = AStarHeuristic::compute(board, next, heuristic);
                pq.push(next, nextH);
            }
        }
    }

    auto endTime = chrono::high_resolution_clock::now();
    long long ms = chrono::duration_cast<chrono::milliseconds>(
        endTime - startTime).count();

    return SolverResult::notFound(iterations, ms, exploredStates);
}
