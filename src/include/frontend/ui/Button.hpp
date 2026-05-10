#pragma once

#include "raylib.h"

class Button {
    private:
        const char* text;
        Rectangle bounds;

    public:
        Button(const char* text, Rectangle bounds);

        bool isClicked() const;
        void draw() const;
        void draw(bool isSelected) const;
};
