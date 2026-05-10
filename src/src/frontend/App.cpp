#include "frontend/App.hpp"

#include "frontend/config/FrontendConfig.hpp"

App::App()
    : window(
        FrontendConfig::WINDOW_WIDTH,
        FrontendConfig::WINDOW_HEIGHT,
        FrontendConfig::WINDOW_TITLE,
        FrontendConfig::TARGET_FPS
    ),
      mainScreen() {}

void App::run() {
    while (!window.shouldClose()) {
        window.beginDrawing();
        mainScreen.draw();
        window.endDrawing();
    }
}
