#include "controller/GUIController.hpp"

#include <algorithm>
#include <queue>

GUIController::GUIController()
    : activeScreen(GUIActiveScreen::MainMenu),
      loadFileName(),
      saveFileName(),
      paintRows(6),
      paintCols(6),
      paintBoard(6, std::string(6, '*')),
      selectedPaintTile('X'),
      nextPaintNumber(0),
      paintReturnScreen(GUIActiveScreen::MainMenu),
      savedPaintRows(6),
      savedPaintCols(6),
      savedPaintBoard(6, std::string(6, '*')),
      savedSelectedPaintTile('X'),
      savedNextPaintNumber(0),
      newGameMessage(),
      selectedAlgorithm(),
      selectedHeuristic(),
      configMessage(),
      playbackPath(),
      solutionMoves(),
      solutionCost(0),
      solutionIterations(0),
      solutionExecutionTime(0),
      playbackIndex(0),
      playbackProgress(0.0f),
      playbackPlaying(true),
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

const std::string& GUIController::getSaveFileName() const {
    return saveFileName;
}

std::string& GUIController::getSaveFileName() {
    return saveFileName;
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

const std::vector<std::pair<int, int>>& GUIController::getPlaybackPath() const {
    return playbackPath;
}

const std::string& GUIController::getSolutionMoves() const {
    return solutionMoves;
}

int GUIController::getSolutionCost() const {
    return solutionCost;
}

int GUIController::getSolutionIterations() const {
    return solutionIterations;
}

long long GUIController::getSolutionExecutionTime() const {
    return solutionExecutionTime;
}

int GUIController::getPlaybackIndex() const {
    return playbackIndex;
}

float GUIController::getPlaybackProgress() const {
    return playbackProgress;
}

bool GUIController::isPlaybackPlaying() const {
    return playbackPlaying;
}

bool GUIController::shouldExit() const {
    return exitRequested;
}

void GUIController::openNewGame() {
    paintReturnScreen = activeScreen;

    if (activeScreen == GUIActiveScreen::MainMenu) {
        resetPaintBoard();
    }
    else {
        savePaintSnapshot();
    }

    activeScreen = GUIActiveScreen::NewGame;
}

void GUIController::openLoadGame() {
    activeScreen = GUIActiveScreen::LoadGame;
}

void GUIController::openConfig() {
    activeScreen = GUIActiveScreen::Config;
    configMessage.clear();
}

void GUIController::openSolution() {
    activeScreen = GUIActiveScreen::Solution;
}

void GUIController::openSave() {
    activeScreen = GUIActiveScreen::Save;
}

void GUIController::openMainMenu() {
    activeScreen = GUIActiveScreen::MainMenu;
}

void GUIController::returnToMainMenuFromConfig() {
    selectedAlgorithm.clear();
    selectedHeuristic.clear();
    configMessage.clear();
    playbackPath.clear();
    solutionMoves.clear();
    solutionCost = 0;
    solutionIterations = 0;
    solutionExecutionTime = 0;
    playbackIndex = 0;
    playbackProgress = 0.0f;
    playbackPlaying = false;
    resetPaintBoard();
    activeScreen = GUIActiveScreen::MainMenu;
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

void GUIController::cancelNewGame() {
    if (paintReturnScreen == GUIActiveScreen::Config) {
        restorePaintSnapshot();
    }
    else {
        resetPaintBoard();
    }

    activeScreen = paintReturnScreen;
}

void GUIController::cancelLoadGame() {
    loadFileName.clear();
    activeScreen = GUIActiveScreen::MainMenu;
}

void GUIController::cancelSave() {
    saveFileName.clear();
    activeScreen = GUIActiveScreen::Solution;
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

    preparePreviewSolution();
    activeScreen = GUIActiveScreen::Solution;
}

void GUIController::updatePlayback(float deltaTime) {
    if (!playbackPlaying || playbackPath.size() <= 1) return;
    if (playbackIndex >= (int) playbackPath.size() - 1) return;

    playbackProgress += deltaTime * 2.5f;
    while (playbackProgress >= 1.0f && playbackIndex < (int) playbackPath.size() - 1) {
        playbackProgress -= 1.0f;
        playbackIndex++;
    }

    if (playbackIndex >= (int) playbackPath.size() - 1) {
        playbackIndex = playbackPath.size() - 1;
        playbackProgress = 0.0f;
        playbackPlaying = false;
    }
}

void GUIController::pausePlayback() {
    playbackPlaying = false;
}

void GUIController::resumePlayback() {
    if (playbackIndex < (int) playbackPath.size() - 1) playbackPlaying = true;
}

void GUIController::stopPlayback() {
    playbackPlaying = false;
    playbackIndex = 0;
    playbackProgress = 0.0f;
}

void GUIController::stepPlaybackBack() {
    playbackPlaying = false;
    playbackProgress = 0.0f;
    if (playbackIndex > 0) playbackIndex--;
}

void GUIController::stepPlaybackForward() {
    playbackPlaying = false;
    playbackProgress = 0.0f;
    if (playbackIndex < (int) playbackPath.size() - 1) playbackIndex++;
}

void GUIController::saveSolution() {
    // Solution writing will live here.
    openSolution();
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

void GUIController::resetPaintBoard() {
    paintRows = 6;
    paintCols = 6;
    paintBoard = std::vector<std::string>(paintRows, std::string(paintCols, '*'));
    selectedPaintTile = 'X';
    nextPaintNumber = 0;
    newGameMessage.clear();
}

void GUIController::savePaintSnapshot() {
    savedPaintRows = paintRows;
    savedPaintCols = paintCols;
    savedPaintBoard = paintBoard;
    savedSelectedPaintTile = selectedPaintTile;
    savedNextPaintNumber = nextPaintNumber;
}

void GUIController::restorePaintSnapshot() {
    paintRows = savedPaintRows;
    paintCols = savedPaintCols;
    paintBoard = savedPaintBoard;
    selectedPaintTile = savedSelectedPaintTile;
    nextPaintNumber = savedNextPaintNumber;
    newGameMessage.clear();
}

void GUIController::preparePreviewSolution() {
    const int rows = paintRows;
    const int cols = paintCols;
    std::pair<int, int> start = {0, 0};
    std::pair<int, int> goal = {rows - 1, cols - 1};
    bool hasStart = false;
    bool hasGoal = false;

    for (int row = 0 ; row < rows ; row++) {
        for (int col = 0 ; col < cols ; col++) {
            if (paintBoard[row][col] == 'Z') {
                start = {row, col};
                hasStart = true;
            }
            else if (paintBoard[row][col] == 'O') {
                goal = {row, col};
                hasGoal = true;
            }
        }
    }

    playbackPath.clear();
    solutionMoves.clear();
    solutionIterations = 0;

    if (!hasStart) {
        playbackPath.push_back(start);
        solutionCost = 0;
        solutionExecutionTime = 0;
        playbackIndex = 0;
        playbackProgress = 0.0f;
        playbackPlaying = false;
        return;
    }

    std::vector<std::vector<int>> visited(rows, std::vector<int>(cols, 0));
    std::vector<std::vector<std::pair<int, int>>> parent(rows, std::vector<std::pair<int, int>>(cols, {-1, -1}));
    std::vector<std::vector<char>> parentMove(rows, std::vector<char>(cols, '\0'));
    std::queue<std::pair<int, int>> queue;
    const int rowDelta[4] = {-1, 1, 0, 0};
    const int colDelta[4] = {0, 0, -1, 1};
    const char moveChars[4] = {'U', 'D', 'L', 'R'};

    queue.push(start);
    visited[start.first][start.second] = 1;

    while (!queue.empty()) {
        std::pair<int, int> current = queue.front();
        queue.pop();
        solutionIterations++;

        if (hasGoal && current == goal) break;

        for (int i = 0 ; i < 4 ; i++) {
            const int nextRow = current.first + rowDelta[i];
            const int nextCol = current.second + colDelta[i];

            if (nextRow < 0 || nextRow >= rows || nextCol < 0 || nextCol >= cols) continue;
            if (visited[nextRow][nextCol]) continue;
            if (paintBoard[nextRow][nextCol] == 'X' || paintBoard[nextRow][nextCol] == 'L') continue;

            visited[nextRow][nextCol] = 1;
            parent[nextRow][nextCol] = current;
            parentMove[nextRow][nextCol] = moveChars[i];
            queue.push({nextRow, nextCol});
        }
    }

    if (!hasGoal || !visited[goal.first][goal.second]) {
        playbackPath.push_back(start);
        solutionCost = 0;
        solutionExecutionTime = solutionIterations;
        playbackIndex = 0;
        playbackProgress = 0.0f;
        playbackPlaying = false;
        return;
    }

    std::pair<int, int> current = goal;
    while (current != start) {
        playbackPath.push_back(current);
        solutionMoves.push_back(parentMove[current.first][current.second]);
        current = parent[current.first][current.second];
    }
    playbackPath.push_back(start);

    std::reverse(playbackPath.begin(), playbackPath.end());
    std::reverse(solutionMoves.begin(), solutionMoves.end());

    solutionCost = solutionMoves.size();
    solutionExecutionTime = std::max(1, solutionIterations / 2);
    playbackIndex = 0;
    playbackProgress = 0.0f;
    playbackPlaying = playbackPath.size() > 1;
}
