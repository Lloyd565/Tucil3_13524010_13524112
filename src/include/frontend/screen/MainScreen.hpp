#pragma once

#include "frontend/ui/BoardView.hpp"

class MainScreen {
    private:
        BoardView boardView;

    public:
        MainScreen();
        void draw() const;
};
