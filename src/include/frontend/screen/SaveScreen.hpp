#pragma once

#include "controller/GUIController.hpp"

class SaveScreen {
    public:
        SaveScreen() = default;

        void update(GUIController& controller);
        void draw(const GUIController& controller) const;
};
