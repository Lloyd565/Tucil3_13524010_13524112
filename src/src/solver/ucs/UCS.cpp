#include "solver/ucs/UCS.hpp"

#include <chrono>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

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

Direction getDirectionFromMove(char move) {
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

vector<State> buildSolutionSteps(
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

SolverResult UCS::solve(const SolverInput& solverInput) {
    const Board& board = solverInput.getBoard();

    auto startTime = chrono::high_resolution_clock::now();
    PriorityQueue<CompareUCS> queue;

    const State& startState = solverInput.getInitialState();

    queue.push(startState, startState.getTotalCost());

    unordered_map<string, int> bestCost;
    bestCost[getStateKey(startState)] = startState.getTotalCost();

    int iterationCount = 0;

    vector<State> exploredStates;

    while (!queue.empty()) {
        State currentState = queue.pop();
        string currentKey = getStateKey(currentState);

        if (currentState.getTotalCost() > bestCost[currentKey]) {
            continue;
        }

        iterationCount++;
        exploredStates.push_back(currentState);

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
                exploredStates
            );
        }

        vector<State> possibleMoves = Movement::getPossibleMoves(board, currentState);

        for (const State& nextState : possibleMoves) {
            string key = getStateKey(nextState);
            int newCost = nextState.getTotalCost();

            if (bestCost.find(key) == bestCost.end() || newCost < bestCost[key]) {
                bestCost[key] = newCost;
                queue.push(nextState, newCost);
            }
        }
    }

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
