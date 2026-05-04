#pragma once

#include <string>
#include "model/Position.hpp"

class State {
    private : 
        Position playerPosition;
        int nextRequiredNumber;
        int totalCost;
        std::string moves;

    public : 
        State();
        State(
            const Position& playerPosition,
            int nextRequiredNumber,
            int totalCost, 
            const std::string& moves
        );

        const Position& getPlayerPosition() const;
        int getNextRequiredNumber() const;
        int getTotalCost() const;
        const std::string& getMoves() const;

        void setPlayerPosition(const Position& position);
        void setNextRequiredNumber(int number);
        void setTotalCost(int cost);
        void setMoves(const std::string& moves);

        State withMove(
            const Position& newPosition,
            int newNextRequiredNumber,
            int addedCost,
            char moveChar
        ) const;

        bool operator==(const State& other) const;
};