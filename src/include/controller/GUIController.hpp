#pragma once

#include <string>
#include <vector>

enum class GUIActiveScreen {
    MainMenu,
    NewGame,
    LoadGame
};

class GUIController {
    private:
        GUIActiveScreen activeScreen;
        std::string loadFileName;
        int paintRows;
        int paintCols;
        std::vector<std::string> paintBoard;
        char selectedPaintTile;
        int nextPaintNumber;
        std::string newGameMessage;
        bool exitRequested;

        int countPaintTile(char tile) const;
        void clearPaintTile(char tile);

    public:
        GUIController();

        GUIActiveScreen getActiveScreen() const;
        const std::string& getLoadFileName() const;
        std::string& getLoadFileName();
        int getPaintRows() const;
        int getPaintCols() const;
        const std::vector<std::string>& getPaintBoard() const;
        char getSelectedPaintTile() const;
        const std::string& getNewGameMessage() const;
        bool shouldExit() const;

        void openNewGame();
        void openLoadGame();
        void requestExit();
        void setPaintBoardSize(int rows, int cols);
        void setSelectedPaintTile(char tile);
        void paintTile(int row, int col);
        void submitNewGame();
        void submitLoadGame();
};
