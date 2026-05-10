#include "frontend/core/Window.hpp"

#include "raylib.h"

Window::Window(int width, int height, const std::string& title, int targetFps)
    : width(width), height(height), windowedX(0), windowedY(0), title(title) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(width, height, title.c_str());
    windowedX = GetWindowPosition().x;
    windowedY = GetWindowPosition().y;
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

void Window::handleInput() {
    if (IsKeyPressed(KEY_F11)) {
        toggleFullscreen();
    }
}

void Window::beginDrawing() const {
    BeginDrawing();
}

void Window::endDrawing() const {
    EndDrawing();
}

void Window::toggleFullscreen() {
    if (IsWindowFullscreen()) {
        ToggleFullscreen();
        SetWindowSize(width, height);
        SetWindowPosition(windowedX, windowedY);
        return;
    }

    windowedX = GetWindowPosition().x;
    windowedY = GetWindowPosition().y;
    width = GetScreenWidth();
    height = GetScreenHeight();

    const int monitor = GetCurrentMonitor();
    SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
    ToggleFullscreen();
}
