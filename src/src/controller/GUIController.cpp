#include "controller/GUIController.hpp"

#include "model/Board.hpp"
#include "model/Position.hpp"
#include "model/State.hpp"
#include "output/SolutionWriter.hpp"
#include "parser/InputParser.hpp"
#include "solver/SolverInput.hpp"
#include "solver/SolverResult.hpp"
#include "solver/astar/AStar.hpp"
#include "solver/gbfs/GBFS.hpp"
#include "solver/idastar/IDAStar.hpp"
#include "solver/ucs/UCS.hpp"

#include <algorithm>
#include <exception>
#include <stdexcept>

static Board buildBoardFromPaintState(
    const std::vector<std::string>& paintBoard,
    int rows,
    int cols
) {
    std::vector<std::vector<char>> grid(rows, std::vector<char>(cols, '*'));
    std::vector<std::vector<int>> costs(rows, std::vector<int>(cols, 1));
    Position startPosition;
    Position goalPosition;
    int maxNumber = -1;

    for (int row = 0 ; row < rows ; row++) {
        for (int col = 0 ; col < cols ; col++) {
            char tile = paintBoard[row][col];

            if (tile == 'Z') {
                startPosition = Position(row, col);
                grid[row][col] = '*';
            }
            else {
                grid[row][col] = tile;
            }

            if (tile == 'O') goalPosition = Position(row, col);
            if (tile >= '0' && tile <= '9') maxNumber = std::max(maxNumber, tile - '0');
        }
    }

    return Board(rows, cols, grid, costs, startPosition, goalPosition, maxNumber);
}

GUIController::GUIController()
    : activeScreen(GUIActiveScreen::MainMenu),
      loadFileName(),
      saveFileName(),
      loadMessage(),
      saveMessage(),
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
      paintBoardDirty(true),
      savedPaintBoardDirty(true),
      newGameMessage(),
      selectedAlgorithm(),
      selectedHeuristic(),
      currentBoard(),
      currentResult(),
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

const std::string& GUIController::getLoadMessage() const {
    return loadMessage;
}

const std::string& GUIController::getSaveMessage() const {
    return saveMessage;
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
    loadMessage.clear();
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
    saveMessage.clear();
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
    paintBoardDirty = true;
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

    paintBoardDirty = true;
    newGameMessage.clear();
}

void GUIController::setSelectedAlgorithm(const std::string& algorithm) {
    selectedAlgorithm = algorithm;
    configMessage.clear();

    if (algorithm != "A*" && algorithm != "GBFS" && algorithm != "IDA*") {
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
    loadMessage.clear();
    activeScreen = GUIActiveScreen::MainMenu;
}

void GUIController::cancelSave() {
    saveFileName.clear();
    saveMessage.clear();
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
    try {
        Board board = InputParser::parseFile(loadFileName);

        paintRows = board.getRows();
        paintCols = board.getCols();
        paintBoard = std::vector<std::string>(paintRows, std::string(paintCols, '*'));

        for (int row = 0 ; row < paintRows ; row++) {
            for (int col = 0 ; col < paintCols ; col++) {
                paintBoard[row][col] = board.getTile(Position(row, col));
            }
        }

        paintBoard[board.getStartPosition().getRow()][board.getStartPosition().getCol()] = 'Z';
        selectedPaintTile = 'X';
        nextPaintNumber = std::min(board.getMaxNumber() + 1, 10);
        currentBoard = board;
        paintBoardDirty = false;
        loadMessage.clear();
        openConfig();
    }
    catch (const std::exception& error) {
        loadMessage = error.what();
    }
}

void GUIController::submitConfig() {
    if (selectedAlgorithm.empty()) {
        configMessage = "Choose an algorithm first.";
        return;
    }

    if ((selectedAlgorithm == "A*" || selectedAlgorithm == "GBFS" || selectedAlgorithm == "IDA*") && selectedHeuristic.empty()) {
        configMessage = "Choose a heuristic for this algorithm.";
        return;
    }

    try {
        if (paintBoardDirty || currentBoard.getRows() != paintRows || currentBoard.getCols() != paintCols) {
            currentBoard = buildBoardFromPaintState(paintBoard, paintRows, paintCols);
            paintBoardDirty = false;
        }

        const std::string heuristic = selectedHeuristic.empty() ? "H1" : selectedHeuristic;
        State initialState(currentBoard.getStartPosition(), 0, 0, "");
        SolverInput solverInput(currentBoard, initialState);
        SolverResult result;

        if (selectedAlgorithm == "UCS") result = UCS::solve(solverInput);
        else if (selectedAlgorithm == "GBFS") result = GBFS::solve(solverInput, heuristic);
        else if (selectedAlgorithm == "IDA*") result = IDAStar::solve(solverInput, heuristic);
        else result = AStar::solve(solverInput, heuristic);

        loadSolutionFromResult(result);
        configMessage.clear();
        activeScreen = GUIActiveScreen::Solution;
    }
    catch (const std::exception& error) {
        configMessage = error.what();
    }
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
    try {
        if (saveFileName.empty()) {
            throw std::runtime_error("Output file name cannot be empty.");
        }

        const std::string heuristic = selectedHeuristic.empty() ? "-" : selectedHeuristic;

        if (!SolutionWriter::save(saveFileName, currentBoard, currentResult, selectedAlgorithm, heuristic)) {
            throw std::runtime_error("Failed to save solution.");
        }

        saveMessage.clear();
        openSolution();
    }
    catch (const std::exception& error) {
        saveMessage = error.what();
    }
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
    paintBoardDirty = true;
    newGameMessage.clear();
}

void GUIController::savePaintSnapshot() {
    savedPaintRows = paintRows;
    savedPaintCols = paintCols;
    savedPaintBoard = paintBoard;
    savedSelectedPaintTile = selectedPaintTile;
    savedNextPaintNumber = nextPaintNumber;
    savedPaintBoardDirty = paintBoardDirty;
}

void GUIController::restorePaintSnapshot() {
    paintRows = savedPaintRows;
    paintCols = savedPaintCols;
    paintBoard = savedPaintBoard;
    selectedPaintTile = savedSelectedPaintTile;
    nextPaintNumber = savedNextPaintNumber;
    paintBoardDirty = savedPaintBoardDirty;
    newGameMessage.clear();
}

void GUIController::loadSolutionFromResult(const SolverResult& result) {
    currentResult = result;
    playbackPath.clear();

    for (const State& step : result.getSolutionSteps()) {
        const Position& position = step.getPlayerPosition();
        playbackPath.push_back({position.getRow(), position.getCol()});
    }

    if (playbackPath.empty() && currentBoard.getRows() > 0 && currentBoard.getCols() > 0) {
        const Position& start = currentBoard.getStartPosition();
        playbackPath.push_back({start.getRow(), start.getCol()});
    }

    solutionMoves = result.isFound() ? result.getMoves() : "No solution";
    solutionCost = result.getTotalCost();
    solutionIterations = result.getIterations();
    solutionExecutionTime = result.getExecutionTime();
    playbackIndex = 0;
    playbackProgress = 0.0f;
    playbackPlaying = result.isFound() && playbackPath.size() > 1;
}
