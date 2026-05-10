#include "controller/GUIController.hpp"

GUIController::GUIController()
    : activeScreen(GUIActiveScreen::MainMenu),
      loadFileName(),
      exitRequested(false) {}

GUIActiveScreen GUIController::getActiveScreen() const {
    return activeScreen;
}

const std::string& GUIController::getLoadFileName() const {
    return loadFileName;
}

std::string& GUIController::getLoadFileName() {
    return loadFileName;
}

bool GUIController::shouldExit() const {
    return exitRequested;
}

void GUIController::openLoadGame() {
    activeScreen = GUIActiveScreen::LoadGame;
}

void GUIController::requestExit() {
    exitRequested = true;
}

void GUIController::submitLoadGame() {
    // Board parsing and SolverInput creation will live here.
}
