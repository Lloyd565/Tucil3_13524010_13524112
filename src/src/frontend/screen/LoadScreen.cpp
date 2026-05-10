#include "frontend/screen/LoadScreen.hpp"

#include "frontend/ui/Button.hpp"
#include "frontend/ui/TextInput.hpp"
#include "raylib.h"

static Rectangle getInputBounds() {
    const float screenWidth = GetScreenWidth();
    const float screenHeight = GetScreenHeight();
    const float inputWidth = screenWidth * 0.48f;
    const float inputHeight = 66.0f;
    const float inputX = (screenWidth - inputWidth) / 2.0f;
    const float inputY = (screenHeight - inputHeight) / 2.0f;

    return Rectangle{inputX, inputY, inputWidth, inputHeight};
}

static Rectangle getEnterButtonBounds() {
    return Rectangle{
        GetScreenWidth() - 190.0f,
        GetScreenHeight() - 104.0f,
        140.0f,
        58.0f
    };
}

void LoadScreen::update(GUIController& controller) {
    TextInput(getInputBounds(), "input.txt", 28, 120).update(controller.getLoadFileName());

    if (Button("Enter", getEnterButtonBounds()).isClicked()) {
        controller.submitLoadGame();
    }
}

void LoadScreen::draw(const GUIController& controller) const {
    const float screenWidth = GetScreenWidth();
    const float screenHeight = GetScreenHeight();
    const Rectangle inputBounds = getInputBounds();
    const char* title = "Load Game";
    const char* prompt = "Enter file name";
    const int titleFontSize = 44;
    const int promptFontSize = 24;
    const int titleWidth = MeasureText(title, titleFontSize);
    const int promptWidth = MeasureText(prompt, promptFontSize);

    ClearBackground(Color{22, 24, 30, 255});

    DrawText(
        title,
        static_cast<int>((screenWidth - titleWidth) / 2.0f),
        static_cast<int>(screenHeight * 0.25f),
        titleFontSize,
        Color{246, 248, 252, 255}
    );
    DrawText(
        prompt,
        static_cast<int>((screenWidth - promptWidth) / 2.0f),
        static_cast<int>(inputBounds.y - 44.0f),
        promptFontSize,
        Color{204, 210, 222, 255}
    );

    TextInput(inputBounds, "input.txt", 28, 120).draw(controller.getLoadFileName());
    Button("Enter", getEnterButtonBounds()).draw();
}
