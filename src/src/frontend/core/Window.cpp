#include "frontend/core/Window.hpp"

#include "raylib.h"

Window::Window(int width, int height, const std::string& title, int targetFps)
    : width(width), height(height), title(title) {
    InitWindow(width, height, title.c_str());
    SetTargetFPS(targetFps);
}

Window::~Window() {
    if (IsWindowReady()) {
        CloseWindow();
    }
}

bool Window::shouldClose() const {
    return WindowShouldClose();
}

void Window::beginDrawing() const {
    BeginDrawing();
}

void Window::endDrawing() const {
    EndDrawing();
}
