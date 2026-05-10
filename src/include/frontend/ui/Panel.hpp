#ifndef FRONTEND_UI_PANEL_HPP
#define FRONTEND_UI_PANEL_HPP

#include <string>

class Panel {
public:
    Panel(float x, float y, float width, float height, const std::string& text);

    void draw() const;

private:
    float x;
    float y;
    float width;
    float height;
    std::string text;
};

#endif
