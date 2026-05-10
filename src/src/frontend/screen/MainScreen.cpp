#include "frontend/screen/MainScreen.hpp"

#include "frontend/config/FrontendConfig.hpp"
#include "raylib.h"

MainScreen::MainScreen()
    : panel(
        FrontendConfig::WINDOW_WIDTH / 2.0f - 220.0f,
        FrontendConfig::WINDOW_HEIGHT / 2.0f - 80.0f,
        440.0f,
        160.0f,
        "Hello, World!"
    ) {}

void MainScreen::draw() const {
    ClearBackground(Color{238, 241, 245, 255});
    panel.draw();
}
