#include "solver/astar/AStar.hpp"
#include "game/Movement.hpp"
#include "game/Rules.hpp"
#include "heuristic/astar/AStarHeuristic.hpp"
#include "model/PriorityQueue.hpp"
#include "solver/Comparators.hpp"

#include <chrono>
#include <string>
#include <unordered_map>
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
    const State& init,
    const State& goal
) {
    vector<State> steps;
    steps.push_back(init);
    State curr = init;
    for (char c : goal.getMoves()) {
        auto next = Movement::slide(board, curr, Movement::charToDirection(c));
        if (next.has_value()) {
            steps.push_back(next.value());
            curr = next.value();
        }
    }
    return steps;
}

SolverResult AStar::solve(const SolverInput& solverInput, const string& heuristic) {
    const Board& board = solverInput.getBoard();
    const State& init = solverInput.getInitialState();
    auto startTime = chrono::high_resolution_clock::now();
    
    PriorityQueue<CompareAStar> pq;
    unordered_map<string, int> bestG;
    vector<State> exploredStates;
    int iter = 0;
    int initH = AStarHeuristic::compute(board, init, heuristic);
    pq.push(init, initH);
    bestG[getStateKey(init)] = 0;

    while (!pq.empty()) {
        State state = pq.pop();
        string key = getStateKey(state);
        int g = state.getTotalCost();
        if (bestG.count(key) && bestG[key] < g) continue;
        exploredStates.push_back(state);
        iter++;
        if (Rules::isGoalState(board, state)) {
            auto endTime = chrono::high_resolution_clock::now();
            long long ms = chrono::duration_cast<chrono::milliseconds>(
                endTime - startTime).count();
            return SolverResult(true, state.getMoves(), state.getTotalCost(), iter, ms, buildSolutionSteps(board, init, state), exploredStates);
        }

        for (const State& next : Movement::getPossibleMoves(board, state)) {
            string nextKey = getStateKey(next);
            int nextG = next.getTotalCost();

            if (!bestG.count(nextKey) || nextG < bestG[nextKey]) {
                bestG[nextKey] = nextG;
                int nextH = AStarHeuristic::compute(board, next, heuristic);
                pq.push(next, nextG + nextH);
            }
        }
    }
    auto endTime = chrono::high_resolution_clock::now();
    long long ms = chrono::duration_cast<chrono::milliseconds>(
        endTime - startTime).count();
    return SolverResult::notFound(iter, ms, exploredStates);
}
