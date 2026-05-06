#include "solver/ucs/UCS.hpp"

#include <unordered_map>
#include <string>
#include <limits>
#include <chrono>
#include <optional>

#include "model/PriorityQueue.hpp"
#include "solver/Comparators.hpp"
#include "game/Movement.hpp"
#include "game/Rules.hpp"

using namespace std;

string getStateKey(const State& state) {
    const Position& position = state.getPlayerPosition();
    return to_string(position.getRow()) + "," +
            to_string(position.getCol()) + "," + 
            to_string(state.getNextRequiredNumber());
}

vector<State> buildSolutionSteps(
    const Board& board, 
    const State& startState, 
    const string& moves
) {
    vector<State> tempSteps;
    State currentState = startState;

    tempSteps.push_back(currentState);

    for (char move : moves) {
        optional<State> nextState;

        
        if (move == 'U') nextState = Movement::slide(board, currentState, Direction::Up);
        else if (move == 'L') nextState = Movement::slide(board, currentState, Direction::Left);
        else if (move == 'R') nextState = Movement::slide(board, currentState, Direction::Right);
        else nextState = Movement::slide(board, currentState, Direction::Down);

        if (!nextState.has_value()) break;

        currentState = nextState.value();
        tempSteps.push_back(currentState);

    }

    return tempSteps;
}

SolverResult UCS::solve(const SolverInput& solverInput) {
    Board board = solverInput.getBoard();

    auto startTime = chrono::high_resolution_clock::now();
    PriorityQueue<CompareUCS> queue;

    State startState = solverInput.getInitialState();

    queue.push(startState, startState.getTotalCost());

    unordered_map<string, int> bestCost;
    bestCost[getStateKey(startState)] = 0;

    int iterationCount = 0;

    vector<State> exploredState;

    while (!queue.empty()) {
        State currentState = queue.pop();
        iterationCount++;
        exploredState.push_back(currentState);

        if (Rules::isGoalState(board, currentState)) {
            auto endTime = chrono::high_resolution_clock::now();

            long long execTime = chrono::duration_cast<chrono::milliseconds>(
                endTime - startTime
            ).count();

            vector<State> solutionSteps = buildSolutionSteps(board, startState, currentState.getMoves());

            return SolverResult (
                true, 
                currentState.getMoves(),
                currentState.getTotalCost(),
                iterationCount,
                execTime, 
                solutionSteps, 
                exploredState
            );
        }

        vector<State> possibleMoves = Movement::getPossibleMoves(board, currentState);

        for (const State& nextState : possibleMoves) {
            string key = getStateKey(nextState);
            int newCost = nextState.getTotalCost();

            if (
                bestCost.find(key) == bestCost.end() ||
                newCost < bestCost[key]
            ) {
                bestCost[key] = newCost;
                queue.push(nextState, nextState.getTotalCost());
            }

            
        }
    }

    return SolverResult();
}


