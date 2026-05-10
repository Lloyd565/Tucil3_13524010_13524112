#ifndef FRONTEND_APP_HPP
#define FRONTEND_APP_HPP

#include "frontend/core/Window.hpp"
#include "frontend/screen/MainScreen.hpp"

class App {
public:
    App();

    void run();

private:
    Window window;
    MainScreen mainScreen;
};

#endif
