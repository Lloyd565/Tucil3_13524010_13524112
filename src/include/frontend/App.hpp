#pragma once

#include "frontend/core/Window.hpp"
#include "frontend/screen/MainScreen.hpp"

class App {
    private:
        Window window;
        MainScreen mainScreen;

    public:
        App();
        void run();
};
