#include "solver/SolverInput.hpp"

SolverInput::SolverInput()
    : board(),
      initialState() {}

SolverInput::SolverInput(const Board& board, const State& initialState)
    : board(board),
      initialState(initialState) {}

const Board& SolverInput::getBoard() const {
    return board;
}

const State& SolverInput::getInitialState() const {
    return initialState;
}
