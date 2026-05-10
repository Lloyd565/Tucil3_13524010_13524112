#pragma once

#include "raylib.h"

class TileLegend {
    private:
        Rectangle bounds;

        void drawItem(int index, Color color, const char* label, const char* marker = nullptr) const;

    public:
        TileLegend(Rectangle bounds);

        void setBounds(Rectangle bounds);
        void draw() const;
};
