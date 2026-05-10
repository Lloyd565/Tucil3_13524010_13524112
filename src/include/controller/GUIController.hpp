#pragma once

#include <string>

enum class GUIActiveScreen {
    MainMenu,
    LoadGame
};

class GUIController {
    private:
        GUIActiveScreen activeScreen;
        std::string loadFileName;
        bool exitRequested;

    public:
        GUIController();

        GUIActiveScreen getActiveScreen() const;
        const std::string& getLoadFileName() const;
        std::string& getLoadFileName();
        bool shouldExit() const;

        void openLoadGame();
        void requestExit();
        void submitLoadGame();
};
