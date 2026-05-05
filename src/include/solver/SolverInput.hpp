#pragma once 

#include "model/Board.hpp"
#include "model/State.hpp"

class SolverInput {
    private : 
        Board board;
        State initialState;

    public : 
        SolverInput();
        SolverInput(const Board& board, const State& initialState);

        const Board& getBoard() const;
        const State& getInitialState() const;
};