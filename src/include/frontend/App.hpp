#pragma once

#include "controller/GUIController.hpp"
#include "frontend/core/Window.hpp"
#include "frontend/screen/ConfigScreen.hpp"
#include "frontend/screen/LoadScreen.hpp"
#include "frontend/screen/MainScreen.hpp"
#include "frontend/screen/NewGameScreen.hpp"
#include "frontend/screen/SaveScreen.hpp"
#include "frontend/screen/SolutionScreen.hpp"

class App {
    private:
        GUIController controller;
        Window window;
        MainScreen mainScreen;
        NewGameScreen newGameScreen;
        LoadScreen loadScreen;
        ConfigScreen configScreen;
        SolutionScreen solutionScreen;
        SaveScreen saveScreen;

    public:
        App();
        void run();
};
