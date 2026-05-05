#pragma once 

#include "model/Board.hpp"
#include "model/Position.hpp"
#include "model/State.hpp"

class Rules {
    public : 
        static bool isWall(char tile);
        static bool isLava(char tile);
        static bool isGoal(char tile);
        static bool isNumberTile(char tile);

        static int charToNumber(char tile);

        static bool isValidNumberTile(char tile, const State& state);
        static bool isGoalState(const Board& board, const State& state);
};