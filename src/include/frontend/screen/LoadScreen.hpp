#pragma once

#include "controller/GUIController.hpp"

class LoadScreen {
    public:
        LoadScreen() = default;

        void update(GUIController& controller);
        void draw(const GUIController& controller) const;
};
