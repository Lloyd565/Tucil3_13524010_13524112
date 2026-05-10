#pragma once

#include <string>

class Window {
    private:
        int width;
        int height;
        int windowedX;
        int windowedY;
        std::string title;

        void toggleFullscreen();

    public:
        Window(int width, int height, const std::string& title, int targetFps);
        ~Window();

        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        bool shouldClose() const;
        void handleInput();
        void beginDrawing() const;
        void endDrawing() const;
};
