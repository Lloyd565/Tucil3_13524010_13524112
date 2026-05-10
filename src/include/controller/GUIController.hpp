#pragma once

#include "model/Board.hpp"
#include "solver/SolverResult.hpp"

#include <string>
#include <utility>
#include <vector>

enum class GUIActiveScreen {
    MainMenu,
    NewGame,
    LoadGame,
    Config,
    Solution,
    Save
};

class GUIController {
    private:
        GUIActiveScreen activeScreen;
        std::string loadFileName;
        std::string saveFileName;
        std::string loadMessage;
        std::string saveMessage;
        int paintRows;
        int paintCols;
        std::vector<std::string> paintBoard;
        char selectedPaintTile;
        int nextPaintNumber;
        GUIActiveScreen paintReturnScreen;
        int savedPaintRows;
        int savedPaintCols;
        std::vector<std::string> savedPaintBoard;
        char savedSelectedPaintTile;
        int savedNextPaintNumber;
        bool paintBoardDirty;
        bool savedPaintBoardDirty;
        std::string newGameMessage;
        std::string selectedAlgorithm;
        std::string selectedHeuristic;
        Board currentBoard;
        SolverResult currentResult;
        std::string configMessage;
        std::vector<std::pair<int, int>> playbackPath;
        std::string solutionMoves;
        int solutionCost;
        int solutionIterations;
        long long solutionExecutionTime;
        int playbackIndex;
        float playbackProgress;
        bool playbackPlaying;
        bool exitRequested;

        int countPaintTile(char tile) const;
        void clearPaintTile(char tile);
        void resetPaintBoard();
        void savePaintSnapshot();
        void restorePaintSnapshot();
        void loadSolutionFromResult(const SolverResult& result);

    public:
        GUIController();

        GUIActiveScreen getActiveScreen() const;
        const std::string& getLoadFileName() const;
        std::string& getLoadFileName();
        const std::string& getSaveFileName() const;
        std::string& getSaveFileName();
        const std::string& getLoadMessage() const;
        const std::string& getSaveMessage() const;
        int getPaintRows() const;
        int getPaintCols() const;
        const std::vector<std::string>& getPaintBoard() const;
        char getSelectedPaintTile() const;
        const std::string& getNewGameMessage() const;
        const std::string& getSelectedAlgorithm() const;
        const std::string& getSelectedHeuristic() const;
        const std::string& getConfigMessage() const;
        const std::vector<std::pair<int, int>>& getPlaybackPath() const;
        const std::string& getSolutionMoves() const;
        int getSolutionCost() const;
        int getSolutionIterations() const;
        long long getSolutionExecutionTime() const;
        int getPlaybackIndex() const;
        float getPlaybackProgress() const;
        bool isPlaybackPlaying() const;
        bool shouldExit() const;

        void openNewGame();
        void openLoadGame();
        void openConfig();
        void openSolution();
        void openSave();
        void openMainMenu();
        void returnToMainMenuFromConfig();
        void requestExit();
        void setPaintBoardSize(int rows, int cols);
        void setSelectedPaintTile(char tile);
        void paintTile(int row, int col);
        void setSelectedAlgorithm(const std::string& algorithm);
        void setSelectedHeuristic(const std::string& heuristic);
        void cancelNewGame();
        void cancelLoadGame();
        void cancelSave();
        void submitNewGame();
        void submitLoadGame();
        void submitConfig();
        void updatePlayback(float deltaTime);
        void pausePlayback();
        void resumePlayback();
        void stopPlayback();
        void stepPlaybackBack();
        void stepPlaybackForward();
        void saveSolution();
};
