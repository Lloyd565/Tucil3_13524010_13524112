#pragma once

#include <string>

class Panel {
    private:
        float x;
        float y;
        float width;
        float height;
        std::string text;

    public:
        Panel(float x, float y, float width, float height, const std::string& text);

        void draw() const;
        void setBounds(float x, float y, float width, float height);
};
