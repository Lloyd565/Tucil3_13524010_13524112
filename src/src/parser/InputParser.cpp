#include "parser/InputParser.hpp"
#include "parser/Validator.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>

using namespace std;

Board InputParser::parseFile(const std::string& path) {
    ifstream file(path);
    if (!file.is_open()) throw runtime_error("Failed to open file");

    int rows;
    int cols;

    if (!(file >> rows >> cols)) throw runtime_error("Invalid input format : missing board dimensions");

    vector<string> gridLines;

    for (int i = 0 ; i < rows ; i++) {
        string line;

        if (!(file >> line)) throw runtime_error("Invalid input format : missing board row");

        gridLines.push_back(line);
    }

    vector<vector<int>> costs(rows, vector<int>(cols));

    for (int i = 0 ; i < rows ; i++) {
        for (int j = 0 ; j < cols ; j++) {
            if (!(file >> costs[i][j])) throw runtime_error("Invalid input format : missing cost");
        }
    }

    if (!Validator::validateAll(rows, cols, gridLines, costs)) throw runtime_error("Invalid input file");

    vector<vector<char>> grid(rows, vector<char>(cols));

    Position startPosition;
    Position goalPosition;

    for (int i = 0 ; i < rows ; i++) {
        for (int j = 0 ; i < cols ; j++) {
            char tile = gridLines[i][j];
            grid[i][j] = tile;

            if (tile == 'Z') startPosition = Position(i, j);
            else if (tile == 'O') goalPosition = Position(i, j);
        }
    }

    int maxNumber = findMaxNumber(gridLines);


    return Board(
        rows, cols, grid, costs, startPosition, goalPosition, maxNumber
    );
}

int InputParser::findMaxNumber(const std::vector<std::string>& gridLines) {
    int maxNumber = -1;
    for (const string& line : gridLines) {
        for (char c : line) {
            if (c >= '0' && c <= '9') {
                int number = c - '0';
                if (number > maxNumber) maxNumber = number;
            }
        }
    }

    return maxNumber;
}
