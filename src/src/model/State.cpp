#include "model/State.hpp"

using namespace std;

State::State()
    : playerPosition(),
      nextRequiredNumber(1),
      totalCost(0),
      moves("") {}

State::State(
    const Position& playerPosition,
    int nextRequiredNumber,
    int totalCost,
    const string& moves
) : playerPosition(playerPosition),
    nextRequiredNumber(nextRequiredNumber),
    totalCost(totalCost),
    moves(moves) {}

const Position& State::getPlayerPosition() const {
    return playerPosition;
}

int State::getNextRequiredNumber() const {
    return nextRequiredNumber;
}

int State::getTotalCost() const {
    return totalCost;
}

const string& State::getMoves() const {
    return moves;
}

void State::setPlayerPosition(const Position& position) {
    playerPosition = position;
}

void State::setNextRequiredNumber(int number) {
    nextRequiredNumber = number;
}

void State::setTotalCost(int cost) {
    totalCost = cost;
}

void State::setMoves(const string& moves) {
    this->moves = moves;
}

State State::withMove(
    const Position& newPosition,
    int newNextRequiredNumber,
    int addedCost,
    char moveChar
) const {
    return State(
        newPosition,
        newNextRequiredNumber,
        totalCost + addedCost,
        moves + moveChar
    );
}

bool State::operator==(const State& other) const {
    return playerPosition == other.playerPosition &&
           nextRequiredNumber == other.nextRequiredNumber;
}
