#include "frontend/ui/TextInput.hpp"

TextInput::TextInput(Rectangle bounds, const char* placeholder, int fontSize, int maxLength)
    : bounds(bounds),
      placeholder(placeholder),
      fontSize(fontSize),
      maxLength(maxLength) {}

void TextInput::update(std::string& text) const {
    int key = GetCharPressed();

    while (key > 0) {
        if (key >= 32 && key <= 126 && (int) text.size() < maxLength) {
            text += static_cast<char>(key);
        }

        key = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE) && !text.empty()) {
        text.pop_back();
    }
}

void TextInput::draw(const std::string& text) const {
    const std::string shownText = text.empty() ? placeholder : text;
    const Color inputTextColor = text.empty() ? Color{132, 139, 152, 255} : Color{31, 35, 45, 255};

    DrawRectangleRounded(bounds, 0.08f, 12, Color{246, 248, 252, 255});
    DrawRectangleRoundedLines(bounds, 0.08f, 12, Color{109, 118, 136, 255});

    BeginScissorMode(
        static_cast<int>(bounds.x + 20.0f),
        static_cast<int>(bounds.y),
        static_cast<int>(bounds.width - 40.0f),
        static_cast<int>(bounds.height)
    );
    DrawText(
        shownText.c_str(),
        static_cast<int>(bounds.x + 20.0f),
        static_cast<int>(bounds.y + (bounds.height - fontSize) / 2.0f),
        fontSize,
        inputTextColor
    );
    EndScissorMode();

    if (!text.empty() && ((GetTime() * 2.0) - static_cast<int>(GetTime() * 2.0)) < 0.5) {
        const int textWidth = MeasureText(text.c_str(), fontSize);
        const float cursorX = bounds.x + 24.0f + textWidth;
        if (cursorX < bounds.x + bounds.width - 20.0f) {
            DrawRectangleRec(
                Rectangle{cursorX, bounds.y + 18.0f, 2.0f, bounds.height - 36.0f},
                Color{31, 35, 45, 255}
            );
        }
    }
}
