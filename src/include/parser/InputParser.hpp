#pragma once 

#include <string>
#include "model/Board.hpp"

class InputParser {
    public : 
        static Board parseFile(const std::string& path);

    private : 
        static int findMaxNumber(const std::vector<std::string>& gridLines);
};