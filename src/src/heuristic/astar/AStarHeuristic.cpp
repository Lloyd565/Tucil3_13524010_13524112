#include "heuristic/astar/AStarHeuristic.hpp"
#include "game/Rules.hpp"

#include <climits>
#include <cmath>
#include <cstdlib>

static int manhattanDistance(const Position& a, const Position& b) {
    return std::abs(a.getRow() - b.getRow()) + std::abs(a.getCol() - b.getCol());
}

static int getMinTraverseCost(const Board& board) {
    int minCost = INT_MAX;
    for (int i = 0; i < board.getRows(); i++) {
        for (int j = 0; j < board.getCols(); j++) {
            Position pos(i, j);
            char tile = board.getTile(pos);
            if (!Rules::isWall(tile) && !Rules::isLava(tile))
                minCost = std::min(minCost, board.getCost(pos));
        }
    }
    return (minCost == INT_MAX) ? 1 : minCost;
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

// H1: Manhattan ke checkpoint
int AStarHeuristic::h1(const Board& board, const State& state) {
    int next = state.getNextRequiredNumber();
    if (next > board.getMaxNumber())
        return manhattanDistance(state.getPlayerPosition(), board.getGoalPosition());
    Position checkpoint = findCheckpoint(board, next);
    return manhattanDistance(state.getPlayerPosition(), checkpoint);
}

// H2: Chebyshev ke checkpoint
int AStarHeuristic::h2(const Board& board, const State& state) {
    int next = state.getNextRequiredNumber();
    Position target = (next > board.getMaxNumber())
        ? board.getGoalPosition()
        : findCheckpoint(board, next);
    int dr = std::abs(state.getPlayerPosition().getRow() - target.getRow());
    int dc = std::abs(state.getPlayerPosition().getCol() - target.getCol());
    return std::max(dr, dc);
}

// H3: Euclidean ke checkpoint
int AStarHeuristic::h3(const Board& board, const State& state) {
    int next = state.getNextRequiredNumber();
    Position target = (next > board.getMaxNumber())
        ? board.getGoalPosition()
        : findCheckpoint(board, next);
    int dr = state.getPlayerPosition().getRow() - target.getRow();
    int dc = state.getPlayerPosition().getCol() - target.getCol();
    return static_cast<int>(std::sqrt(static_cast<double>(dr * dr + dc * dc)));
}

// H4: Manhattan chain
int AStarHeuristic::h4(const Board& board, const State& state) {
    int next = state.getNextRequiredNumber();
    int maxN = board.getMaxNumber();
    if (next > maxN)
        return manhattanDistance(state.getPlayerPosition(), board.getGoalPosition());
    Position from = state.getPlayerPosition();
    int tot = 0;
    for (int cp = next; cp <= maxN; cp++) {
        Position checkpoint = findCheckpoint(board, cp);
        tot += manhattanDistance(from, checkpoint);
        from = checkpoint;
    }
    tot += manhattanDistance(from, board.getGoalPosition());
    return tot;
}

// H5: Scaling costn — mintilecost × Manhattan chain
int AStarHeuristic::h5(const Board& board, const State& state) {
    int minCost = getMinTraverseCost(board);
    int next = state.getNextRequiredNumber();
    int maxN = board.getMaxNumber();

    if (next > maxN)
        return minCost * manhattanDistance(state.getPlayerPosition(), board.getGoalPosition());

    Position prev = state.getPlayerPosition();
    int tot = 0;
    for (int cp = next; cp <= maxN; cp++) {
        Position checkpoint = findCheckpoint(board, cp);
        tot += manhattanDistance(prev, checkpoint);
        prev = checkpoint;
    }
    tot += manhattanDistance(prev, board.getGoalPosition());
    return minCost * tot;
}

int AStarHeuristic::compute(const Board& board, const State& state, const std::string& heuristic) {
    if (heuristic == "H2") return h2(board, state);
    if (heuristic == "H3") return h3(board, state);
    if (heuristic == "H4") return h4(board, state);
    if (heuristic == "H5") return h5(board, state);
    return h1(board, state);
}
