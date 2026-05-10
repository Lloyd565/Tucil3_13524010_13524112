#pragma once

#include "controller/GUIController.hpp"
#include "frontend/core/Window.hpp"
#include "frontend/screen/LoadScreen.hpp"
#include "frontend/screen/MainScreen.hpp"

class App {
    private:
        GUIController controller;
        Window window;
        MainScreen mainScreen;
        LoadScreen loadScreen;

    public:
        App();
        void run();
};
