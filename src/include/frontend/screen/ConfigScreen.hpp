#pragma once

#include "controller/GUIController.hpp"
#include "frontend/ui/BoardCanvas.hpp"

class ConfigScreen {
    private:
        BoardCanvas boardCanvas;

    public:
        ConfigScreen();

        void update(GUIController& controller);
        void draw(const GUIController& controller) const;
};
