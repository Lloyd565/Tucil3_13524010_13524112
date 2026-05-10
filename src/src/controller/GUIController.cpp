#include "controller/GUIController.hpp"

#include <algorithm>

GUIController::GUIController()
    : activeScreen(GUIActiveScreen::MainMenu),
      loadFileName(),
      paintRows(6),
      paintCols(6),
      paintBoard(6, std::string(6, '*')),
      selectedPaintTile('X'),
      nextPaintNumber(0),
      newGameMessage(),
      selectedAlgorithm(),
      selectedHeuristic(),
      configMessage(),
      exitRequested(false) {}

GUIActiveScreen GUIController::getActiveScreen() const {
    return activeScreen;
}

const std::string& GUIController::getLoadFileName() const {
    return loadFileName;
}

std::string& GUIController::getLoadFileName() {
    return loadFileName;
}

int GUIController::getPaintRows() const {
    return paintRows;
}

int GUIController::getPaintCols() const {
    return paintCols;
}

const std::vector<std::string>& GUIController::getPaintBoard() const {
    return paintBoard;
}

char GUIController::getSelectedPaintTile() const {
    return selectedPaintTile;
}

const std::string& GUIController::getNewGameMessage() const {
    return newGameMessage;
}

const std::string& GUIController::getSelectedAlgorithm() const {
    return selectedAlgorithm;
}

const std::string& GUIController::getSelectedHeuristic() const {
    return selectedHeuristic;
}

const std::string& GUIController::getConfigMessage() const {
    return configMessage;
}

bool GUIController::shouldExit() const {
    return exitRequested;
}

void GUIController::openNewGame() {
    activeScreen = GUIActiveScreen::NewGame;
}

void GUIController::openLoadGame() {
    activeScreen = GUIActiveScreen::LoadGame;
}

void GUIController::openConfig() {
    activeScreen = GUIActiveScreen::Config;
    configMessage.clear();
}

void GUIController::requestExit() {
    exitRequested = true;
}

void GUIController::setPaintBoardSize(int rows, int cols) {
    rows = std::max(2, std::min(rows, 12));
    cols = std::max(2, std::min(cols, 12));

    std::vector<std::string> resizedBoard(rows, std::string(cols, '*'));
    const int copiedRows = std::min(rows, paintRows);
    const int copiedCols = std::min(cols, paintCols);

    for (int row = 0 ; row < copiedRows ; row++) {
        for (int col = 0 ; col < copiedCols ; col++) {
            resizedBoard[row][col] = paintBoard[row][col];
        }
    }

    paintRows = rows;
    paintCols = cols;
    paintBoard = resizedBoard;
    newGameMessage.clear();
}

void GUIController::setSelectedPaintTile(char tile) {
    selectedPaintTile = tile;
    newGameMessage.clear();
}

void GUIController::paintTile(int row, int col) {
    if (row < 0 || row >= paintRows || col < 0 || col >= paintCols) return;

    if (selectedPaintTile == 'N') {
        if (nextPaintNumber > 9 || (paintBoard[row][col] >= '0' && paintBoard[row][col] <= '9')) return;

        paintBoard[row][col] = static_cast<char>('0' + nextPaintNumber);
        nextPaintNumber++;
    }
    else {
        if (selectedPaintTile == 'O' || selectedPaintTile == 'Z') {
            clearPaintTile(selectedPaintTile);
        }

        paintBoard[row][col] = selectedPaintTile;
    }

    newGameMessage.clear();
}

void GUIController::setSelectedAlgorithm(const std::string& algorithm) {
    selectedAlgorithm = algorithm;
    configMessage.clear();

    if (algorithm != "A*" && algorithm != "GBFS") {
        selectedHeuristic.clear();
    }
}

void GUIController::setSelectedHeuristic(const std::string& heuristic) {
    selectedHeuristic = heuristic;
    configMessage.clear();
}

void GUIController::submitNewGame() {
    const int goalCount = countPaintTile('O');
    const int playerCount = countPaintTile('Z');

    if (goalCount != 1 || playerCount != 1) {
        newGameMessage = "Board must have exactly one goal and one player.";
        return;
    }

    openConfig();
}

void GUIController::submitLoadGame() {
    // Board parsing and SolverInput creation will live here.
    openConfig();
}

void GUIController::submitConfig() {
    if (selectedAlgorithm.empty()) {
        configMessage = "Choose an algorithm first.";
        return;
    }

    if ((selectedAlgorithm == "A*" || selectedAlgorithm == "GBFS") && selectedHeuristic.empty()) {
        configMessage = "Choose a heuristic for this algorithm.";
        return;
    }

    configMessage = "Configuration is ready. Next page will be added later.";
}

int GUIController::countPaintTile(char tile) const {
    int count = 0;

    for (const std::string& row : paintBoard) {
        for (char currentTile : row) {
            if (currentTile == tile) count++;
        }
    }

    return count;
}

void GUIController::clearPaintTile(char tile) {
    for (std::string& row : paintBoard) {
        for (char& currentTile : row) {
            if (currentTile == tile) currentTile = '*';
        }
    }
}
