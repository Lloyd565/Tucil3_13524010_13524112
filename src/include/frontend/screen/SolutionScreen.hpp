#pragma once

#include "controller/GUIController.hpp"
#include "frontend/ui/AnimatedBoard.hpp"
#include "frontend/ui/TileLegend.hpp"

class SolutionScreen {
    private:
        AnimatedBoard animatedBoard;
        TileLegend tileLegend;

    public:
        SolutionScreen();

        void update(GUIController& controller);
        void draw(const GUIController& controller) const;
};
