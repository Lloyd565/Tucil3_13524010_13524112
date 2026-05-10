#pragma once

#include "raylib.h"

class SizeConfig {
    private:
        Rectangle bounds;

        Rectangle getRowsMinusBounds() const;
        Rectangle getRowsPlusBounds() const;
        Rectangle getColsMinusBounds() const;
        Rectangle getColsPlusBounds() const;

    public:
        SizeConfig(Rectangle bounds);

        int getRowDelta() const;
        int getColDelta() const;
        void draw(int rows, int cols) const;
};
