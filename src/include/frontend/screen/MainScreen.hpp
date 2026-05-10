#ifndef FRONTEND_SCREEN_MAINSCREEN_HPP
#define FRONTEND_SCREEN_MAINSCREEN_HPP

#include "frontend/ui/Panel.hpp"

class MainScreen {
public:
    MainScreen();

    void draw() const;

private:
    Panel panel;
};

#endif
