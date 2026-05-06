#include "heuristic/astar/AStarHeuristic.hpp"
#include "game/Rules.hpp"

#include <cmath>
#include <cstdlib>

static int manhattanDistance(const Position& a, const Position& b) {
    return std::abs(a.getRow() - b.getRow()) + std::abs(a.getCol() - b.getCol());
}

static Position findCheckpoint(const Board& board, int number) {
    for (int i = 0; i < board.getRows(); i++) {
        for (int j = 0; j < board.getCols(); j++) {
            Position pos(i, j);
            if (Rules::charToNumber(board.getTile(pos)) == number) return pos;
        }
    }
    return board.getGoalPosition();
}

// H1: Manhattan distance langsung ke goal (abaikan checkpoint)
int AStarHeuristic::h1(const Board& board, const State& state) {
    return manhattanDistance(state.getPlayerPosition(), board.getGoalPosition());
}

// H2: Manhattan ke checkpoint berikutnya + Manhattan dari checkpoint ke goal
int AStarHeuristic::h2(const Board& board, const State& state) {
    int next = state.getNextRequiredNumber();
    if (next > board.getMaxNumber()) {
        return manhattanDistance(state.getPlayerPosition(), board.getGoalPosition());
    }
    Position checkpoint = findCheckpoint(board, next);
    return manhattanDistance(state.getPlayerPosition(), checkpoint)
         + manhattanDistance(checkpoint, board.getGoalPosition());
}

// H3: Euclidean distance ke goal
int AStarHeuristic::h3(const Board& board, const State& state) {
    int dr = state.getPlayerPosition().getRow() - board.getGoalPosition().getRow();
    int dc = state.getPlayerPosition().getCol() - board.getGoalPosition().getCol();
    return static_cast<int>(std::sqrt(static_cast<double>(dr * dr + dc * dc)));
}

int AStarHeuristic::compute(const Board& board, const State& state, const std::string& heuristic) {
    if (heuristic == "H2") return h2(board, state);
    if (heuristic == "H3") return h3(board, state);
    return h1(board, state);
}
