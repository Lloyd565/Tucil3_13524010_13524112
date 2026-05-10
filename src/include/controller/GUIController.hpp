#pragma once

#include <string>
#include <vector>

enum class GUIActiveScreen {
    MainMenu,
    NewGame,
    LoadGame,
    Config
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
        std::string selectedAlgorithm;
        std::string selectedHeuristic;
        std::string configMessage;
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
        const std::string& getSelectedAlgorithm() const;
        const std::string& getSelectedHeuristic() const;
        const std::string& getConfigMessage() const;
        bool shouldExit() const;

        void openNewGame();
        void openLoadGame();
        void openConfig();
        void requestExit();
        void setPaintBoardSize(int rows, int cols);
        void setSelectedPaintTile(char tile);
        void paintTile(int row, int col);
        void setSelectedAlgorithm(const std::string& algorithm);
        void setSelectedHeuristic(const std::string& heuristic);
        void submitNewGame();
        void submitLoadGame();
        void submitConfig();
};
