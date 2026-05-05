#include "parser/InputParser.hpp"
#include "solver/SolverInput.hpp"
#include "solver/astar/AStar.hpp"
#include "solver/gbfs/GBFS.hpp"
#include "solver/ucs/UCS.hpp"
#include "output/Printer.hpp"
#include "output/Playback.hpp"
#include "output/SolutionWriter.hpp"

#include <algorithm>
#include <cctype>
#include <exception>
#include <iostream>
#include <string>

using namespace std;

static string toLower(string text) {
    transform(
        text.begin(),
        text.end(),
        text.begin(),
        [](unsigned char c) {
            return tolower(c);
        }
    );

    return text;
}

static string normalizeAlgorithm(const string& algorithm) {
    string lowerAlgorithm = toLower(algorithm);

    if (lowerAlgorithm == "ucs") return "UCS";
    if (lowerAlgorithm == "gbfs") return "GBFS";
    if (lowerAlgorithm == "a*") return "A*";

    return "";
}

static bool isYes(const string& answer) {
    string lowerAnswer = toLower(answer);

    return lowerAnswer == "ya";
}

int main() {
    string inputPath;
    string algorithmInput;
    string algorithmName;
    string heuristicName = "-";

    cout << "Masukan file input : \n";
    cin >> inputPath;

    cout << "Algoritma apa yang anda pilih? (UCS/GBFS/A*)\n";
    cin >> algorithmInput;

    algorithmName = normalizeAlgorithm(algorithmInput);

    if (algorithmName == "") {
        cerr << "Algoritma tidak valid.\n";
        return 1;
    }

    if (algorithmName == "A*" || algorithmName == "GBFS") {
        cout << "Heuristic apa yang anda pilih? (H1/H2/H3)\n";
        cin >> heuristicName;
    }

    try {
        Board board = InputParser::parseFile(inputPath);
        State initialState(board.getStartPosition(), 1, 0, "");
        SolverInput solverInput(board, initialState);
        SolverResult result;

        cout << "Algoritma " << algorithmName << " digunakan.\n";

        if (algorithmName == "UCS") result = UCS::solve(solverInput);
        else if (algorithmName == "GBFS") result = GBFS::solve(solverInput);
        else result = AStar::solve(solverInput);

        Printer::printSolution(board, result);

        string playbackChoice;
        cout << "Apakah Anda ingin melakukan playback? (Ya/Tidak) : \n";
        cin >> playbackChoice;

        if (isYes(playbackChoice)) Playback::run(board, result);

        string saveChoice;
        cout << "Apakah Anda ingin menyimpan solusi? (Ya/Tidak) : \n";
        cin >> saveChoice;

        if (isYes(saveChoice)) {
            string outputPath;

            cout << "Masukan file output : \n";
            cin >> outputPath;

            SolutionWriter::save(
                outputPath,
                board,
                result,
                algorithmName,
                heuristicName
            );

            cout << "Solusi disimpan pada " << outputPath << '\n';
        }
    }
    catch (const exception& error) {
        cerr << error.what() << '\n';
        return 1;
    }

    return 0;
}
