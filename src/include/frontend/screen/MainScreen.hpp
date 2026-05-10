#pragma once

#include "controller/GUIController.hpp"
#include "frontend/ui/BoardView.hpp"

class MainScreen {
    private:
        BoardView boardView;

    public:
        MainScreen();
        void update(GUIController& controller);
        void draw() const;
};
