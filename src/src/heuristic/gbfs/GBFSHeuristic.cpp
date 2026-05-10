#include "heuristic/gbfs/GBFSHeuristic.hpp"
#include "game/Rules.hpp"

#include <climits>
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

static int scanLineCost(const Board& board, const Position& from, const Position& to) {
    int dr = 0, dc = 0;
    if (from.getRow() != to.getRow())
        dr = (to.getRow() > from.getRow()) ? 1 : -1;
    else
        dc = (to.getCol() > from.getCol()) ? 1 : -1;

    int cost = 0;
    int r = from.getRow(), c = from.getCol();
    while (r != to.getRow() || c != to.getCol()) {
        r += dr; c += dc;
        Position p(r, c);
        char tile = board.getTile(p);
        if (Rules::isWall(tile) || Rules::isLava(tile)) return INT_MAX;
        cost += board.getCost(p);
    }
    return cost;
}

static int sampleLPathCost(const Board& board, const Position& from, const Position& to, int fallback) {
    if (from.getRow() == to.getRow() && from.getCol() == to.getCol()) return 0;

    if (from.getRow() == to.getRow() || from.getCol() == to.getCol()) {
        int c = scanLineCost(board, from, to);
        return (c == INT_MAX) ? fallback : c;
    }
    int best = INT_MAX;
    {
        Position mid(from.getRow(), to.getCol());
        int c1 = scanLineCost(board, from, mid);
        if (c1 != INT_MAX) {
            int c2 = scanLineCost(board, mid, to);
            if (c2 != INT_MAX) best = std::min(best, c1 + c2);
        }
    }
    {
        Position mid(to.getRow(), from.getCol());
        int c1 = scanLineCost(board, from, mid);
        if (c1 != INT_MAX) {
            int c2 = scanLineCost(board, mid, to);
            if (c2 != INT_MAX) best = std::min(best, c1 + c2);
        }
    }

    return (best != INT_MAX) ? best : 3 * fallback;
}


int GBFSHeuristic::compute(const Board& board, const State& state) {
    int minCost = getMinTraverseCost(board);
    int next = state.getNextRequiredNumber();
    int maxN = board.getMaxNumber();

    Position prev = state.getPlayerPosition();
    int total = 0;

    for (int cp = next; cp <= maxN; cp++) {
        Position checkpoint = findCheckpoint(board, cp);
        int fallback = minCost * manhattanDistance(prev, checkpoint);
        total += sampleLPathCost(board, prev, checkpoint, fallback);
        prev = checkpoint;
    }

    int fallback = minCost * manhattanDistance(prev, board.getGoalPosition());
    total += sampleLPathCost(board, prev, board.getGoalPosition(), fallback);
    return total;
}
