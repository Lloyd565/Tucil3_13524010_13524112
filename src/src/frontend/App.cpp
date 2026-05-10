#include "frontend/App.hpp"

#include "frontend/config/FrontendConfig.hpp"

App::App()
    : controller(),
      window(
        FrontendConfig::WINDOW_WIDTH,
        FrontendConfig::WINDOW_HEIGHT,
        FrontendConfig::WINDOW_TITLE,
        FrontendConfig::TARGET_FPS
    ),
      mainScreen(),
      newGameScreen(),
      loadScreen() {}

void App::run() {
    while (!window.shouldClose() && !controller.shouldExit()) {
        window.handleInput();

        if (controller.getActiveScreen() == GUIActiveScreen::MainMenu) {
            mainScreen.update(controller);
        }
        else if (controller.getActiveScreen() == GUIActiveScreen::NewGame) {
            newGameScreen.update(controller);
        }
        else if (controller.getActiveScreen() == GUIActiveScreen::LoadGame) {
            loadScreen.update(controller);
        }

        window.beginDrawing();

        if (controller.getActiveScreen() == GUIActiveScreen::MainMenu) mainScreen.draw();
        else if (controller.getActiveScreen() == GUIActiveScreen::NewGame) newGameScreen.draw(controller);
        else if (controller.getActiveScreen() == GUIActiveScreen::LoadGame) loadScreen.draw(controller);

        window.endDrawing();
    }
}
