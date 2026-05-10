#pragma once

#include "raylib.h"

#include <string>

class TextInput {
    private:
        Rectangle bounds;
        const char* placeholder;
        int fontSize;
        int maxLength;

    public:
        TextInput(Rectangle bounds, const char* placeholder, int fontSize, int maxLength);

        void update(std::string& text) const;
        void draw(const std::string& text) const;
};
