#pragma once

#include "controller/GUIController.hpp"
#include "frontend/core/Window.hpp"
#include "frontend/screen/ConfigScreen.hpp"
#include "frontend/screen/LoadScreen.hpp"
#include "frontend/screen/MainScreen.hpp"
#include "frontend/screen/NewGameScreen.hpp"

class App {
    private:
        GUIController controller;
        Window window;
        MainScreen mainScreen;
        NewGameScreen newGameScreen;
        LoadScreen loadScreen;
        ConfigScreen configScreen;

    public:
        App();
        void run();
};
