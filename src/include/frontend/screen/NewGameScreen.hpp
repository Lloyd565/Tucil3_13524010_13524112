#pragma once

#include "controller/GUIController.hpp"
#include "frontend/ui/BoardCanvas.hpp"

class NewGameScreen {
    private:
        BoardCanvas boardCanvas;

    public:
        NewGameScreen();

        void update(GUIController& controller);
        void draw(const GUIController& controller) const;
};
