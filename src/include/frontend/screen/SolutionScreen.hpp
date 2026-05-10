#pragma once

#include "controller/GUIController.hpp"
#include "frontend/ui/AnimatedBoard.hpp"

class SolutionScreen {
    private:
        AnimatedBoard animatedBoard;

    public:
        SolutionScreen();

        void update(GUIController& controller);
        void draw(const GUIController& controller) const;
};
