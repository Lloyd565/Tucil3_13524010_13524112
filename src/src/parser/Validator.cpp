#include "parser/Validator.hpp"

using namespace std;

bool Validator::validateDimension(int row, int col) {
    return row > 0 && col > 0;
}

bool Validator::validateGridSize(
    const vector<string>& gridLines,
    int row,
    int col
) {
    if ((int) gridLines.size() != row) return false;
    for (const string& line : gridLines) {
        if ((int) line.size() != col) return false;
    }
    return true;
}

bool Validator::validateCostSize(
    const vector<vector<int>>& costLines,
    int row,
    int col
) {
    if ((int) costLines.size() != row) return false;
    for (const vector<int>& line : costLines) {
        if ((int) line.size() != col) return false;
    }
    return true;
}

bool Validator::validateSymbols(const vector<string>& gridLines) {
    for (const string& line : gridLines) {
        for (char c : line) {
            if (!isAllowedSymbol(c)) return false;
        }
    }
    return true;
}

bool Validator::validatePlayerAndGoal(const vector<string>& gridLines) {
    int playerCount = 0;
    int goalCount = 0;
    for (const string& line : gridLines) {
        for (char c : line) {
            if (c == 'Z') playerCount++;
            else if (c == 'O') goalCount++;
        }
    }
    return playerCount == 1 && goalCount == 1;
}

bool Validator::validateNumbers(const vector<string>& gridLines) {
    bool count[10] = {false};
    for (const string& line : gridLines) {
        for (char c : line) {
            if (c >= '0' && c <= '9') count[c - '0']++;
        }
    }

    for (int i = 0 ; i < 10 ; i ++) {
        if (count[i] > 1) return false;
    }

    int maxNumber = -1;
    for (int i = 0 ; i < 10 ; i++) {
        if (count[i] == 1) maxNumber = i;
    }

    if (maxNumber == -1) return true;

    for (int i = 0 ; i < maxNumber ; i++) {
        if (count[i] != 1) return false;
    }

    return true;
}

bool Validator::validateAll(
    int row,
    int col,
    const vector<string>& gridLines,
    const vector<vector<int>>& costs
) {
    return validateDimension(row, col) &&
            validateGridSize(gridLines, row, col) && 
            validateCostSize(costs, row, col) && 
            validateSymbols(gridLines) && 
            validatePlayerAndGoal(gridLines) &&
            validateNumbers(gridLines);

}

bool Validator::isAllowedSymbol(char c) {
    return c == '*' ||
           c == 'X' ||
           c == 'L' ||
           c == 'Z' ||
           c == 'O' ||
           (c >= '0' && c <= '9');
}
