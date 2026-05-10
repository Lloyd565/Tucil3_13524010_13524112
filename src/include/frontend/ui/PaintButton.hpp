#pragma once

#include "raylib.h"

class PaintButton {
    private:
        Rectangle bounds;

    public:
        PaintButton(Rectangle bounds);

        bool isClicked() const;
        void draw() const;
};
