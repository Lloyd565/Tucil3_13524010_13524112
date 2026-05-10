#include "solver/gbfs/GBFS.hpp"
#include "game/Movement.hpp"
#include "game/Rules.hpp"
#include "heuristic/gbfs/GBFSHeuristic.hpp"
#include "model/PriorityQueue.hpp"
#include "solver/Comparators.hpp"

#include <chrono>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

static string getStateKey(const State& state) {
    const auto& pos = state.getPlayerPosition();
    return to_string(pos.getRow()) + "," +
           to_string(pos.getCol()) + "," +
           to_string(state.getNextRequiredNumber());
}

static vector<State> buildSolutionSteps(
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

SolverResult GBFS::solve(const SolverInput& solverInput) {
    const Board& board = solverInput.getBoard();
    const State& init = solverInput.getInitialState();
    auto startTime = chrono::high_resolution_clock::now();

    PriorityQueue<CompareGBFS> pq;
    unordered_set<string> visited;
    vector<State> exploredStates;
    int iterations = 0;
    int initH = GBFSHeuristic::compute(board, init);
    pq.push(init, initH);

    while (!pq.empty()) {
        State state = pq.pop();
        string key = getStateKey(state);
        if (visited.count(key)) continue;
        visited.insert(key);
        exploredStates.push_back(state);
        iterations++;

        if (Rules::isGoalState(board, state)) {
            auto endTime = chrono::high_resolution_clock::now();
            long long ms = chrono::duration_cast<chrono::milliseconds>(
                endTime - startTime).count();

            return SolverResult(true, state.getMoves(), state.getTotalCost(), iterations, ms, buildSolutionSteps(board, init, state), exploredStates);
        }

        for (const State& next : Movement::getPossibleMoves(board, state)) {
            string nextKey = getStateKey(next);
            if (!visited.count(nextKey)) {
                int nextH = GBFSHeuristic::compute(board, next);
                pq.push(next, nextH);
            }
        }
    }

    auto endTime = chrono::high_resolution_clock::now();
    long long ms = chrono::duration_cast<chrono::milliseconds>(
        endTime - startTime).count();
    return SolverResult::notFound(iterations, ms, exploredStates);
}
