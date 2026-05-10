#include "frontend/screen/LoadScreen.hpp"

#include "frontend/ui/Button.hpp"
#include "frontend/ui/TextInput.hpp"
#include "raylib.h"

#include <algorithm>

static float scaleX() {
    return std::max(800, GetScreenWidth()) / 1280.0f;
}

static float scaleY() {
    return std::max(600, GetScreenHeight()) / 720.0f;
}

static Rectangle getInputBounds() {
    const float screenWidth = GetScreenWidth();
    const float screenHeight = GetScreenHeight();
    const float inputWidth = screenWidth * 0.48f;
    const float inputHeight = 66.0f * scaleY();
    const float inputX = (screenWidth - inputWidth) / 2.0f;
    const float inputY = (screenHeight - inputHeight) / 2.0f;

    return Rectangle{inputX, inputY, inputWidth, inputHeight};
}

static Rectangle getEnterButtonBounds() {
    return Rectangle{
        GetScreenWidth() - 190.0f * scaleX(),
        GetScreenHeight() - 104.0f * scaleY(),
        140.0f * scaleX(),
        58.0f * scaleY()
    };
}

static Rectangle getBackButtonBounds() {
    return Rectangle{34.0f * scaleX(), 28.0f * scaleY(), 120.0f * scaleX(), 48.0f * scaleY()};
}

void LoadScreen::update(GUIController& controller) {
    if (Button("Back", getBackButtonBounds()).isClicked()) {
        controller.cancelLoadGame();
        return;
    }

    TextInput(getInputBounds(), "input.txt", static_cast<int>(28.0f * std::min(scaleX(), scaleY())), 120).update(controller.getLoadFileName());

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
    const int titleFontSize = static_cast<int>(44.0f * std::min(scaleX(), scaleY()));
    const int promptFontSize = static_cast<int>(24.0f * std::min(scaleX(), scaleY()));
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

    Button("Back", getBackButtonBounds()).draw();
    TextInput(inputBounds, "input.txt", static_cast<int>(28.0f * std::min(scaleX(), scaleY())), 120).draw(controller.getLoadFileName());
    Button("Enter", getEnterButtonBounds()).draw();

    if (!controller.getLoadMessage().empty()) {
        const std::string& message = controller.getLoadMessage();
        const int messageFontSize = static_cast<int>(22.0f * std::min(scaleX(), scaleY()));
        const int messageWidth = MeasureText(message.c_str(), messageFontSize);

        DrawText(
            message.c_str(),
            static_cast<int>((screenWidth - messageWidth) / 2.0f),
            static_cast<int>(inputBounds.y + inputBounds.height + 24.0f * scaleY()),
            messageFontSize,
            Color{255, 144, 144, 255}
        );
    }
}
