#pragma once

#include "controller/GUIController.hpp"

class NewGameScreen {
    public:
        NewGameScreen() = default;

        void update(GUIController& controller);
        void draw(const GUIController& controller) const;
};
