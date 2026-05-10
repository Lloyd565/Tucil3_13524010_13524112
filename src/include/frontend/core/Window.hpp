#ifndef FRONTEND_CORE_WINDOW_HPP
#define FRONTEND_CORE_WINDOW_HPP

#include <string>

class Window {
public:
    Window(int width, int height, const std::string& title, int targetFps);
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    bool shouldClose() const;
    void beginDrawing() const;
    void endDrawing() const;

private:
    int width;
    int height;
    std::string title;
};

#endif
