#pragma once

#include "controller/GUIController.hpp"
#include "frontend/ui/BoardCanvas.hpp"
#include "frontend/ui/TileLegend.hpp"

class ConfigScreen {
    private:
        BoardCanvas boardCanvas;
        TileLegend tileLegend;

    public:
        ConfigScreen();

        void update(GUIController& controller);
        void draw(const GUIController& controller) const;
};
