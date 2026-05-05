#pragma once 

#include <string>
#include <vector>

class Validator {
    public :    
        static bool validateDimension(int row, int col);
        static bool validateGridSize(const std::vector<std::string>& gridLines, int row, int col);
        static bool validateCostSize(const std::vector<std::vector<int>>& costLines, int row, int col);
        static bool validateSymbols(const std::vector<std::string>& gridLines);
        static bool validatePlayerAndGoal(const std::vector<std::string>& gridLines);
        static bool validateNumbers(const std::vector<std::string>& gridLines);
        static bool validateAll(int row, int col, const std::vector<std::string>& gridLines, const std::vector<std::vector<int>>& costs);

    private : 
        static bool isAllowedSymbol(char c);
};