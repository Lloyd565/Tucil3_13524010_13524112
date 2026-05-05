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
#include <stdexcept>
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

static string normalizeHeuristic(const string& heuristic) {
    string lowerHeuristic = toLower(heuristic);

    if (lowerHeuristic == "h1") return "H1";
    if (lowerHeuristic == "h2") return "H2";
    if (lowerHeuristic == "h3") return "H3";

    return "";
}

static bool promptYesNo(const string& prompt) {
    string answer;

    while (true) {
        cout << prompt << '\n';
        if (!(cin >> answer)) return false;

        string lowerAnswer = toLower(answer);

        if (lowerAnswer == "ya") return true;
        if (lowerAnswer == "tidak") return false;

        cerr << "Input tidak valid. Masukkan Ya atau Tidak.\n";
    }
}

static Board promptBoard() {
    string inputPath;

    while (true) {
        cout << "Masukan file input : \n";
        if (!(cin >> inputPath)) throw runtime_error("Gagal membaca file input");

        try {
            return InputParser::parseFile(inputPath);
        }
        catch (const exception& error) {
            cerr << error.what() << '\n';
            cerr << "Silakan masukan file input yang valid.\n";
        }
    }
}

static string promptAlgorithm() {
    string algorithmInput;
    string algorithmName;

    while (algorithmName == "") {
        cout << "Algoritma apa yang anda pilih? (UCS/GBFS/A*)\n";
        if (!(cin >> algorithmInput)) throw runtime_error("Gagal membaca algoritma");

        algorithmName = normalizeAlgorithm(algorithmInput);

        if (algorithmName == "") cerr << "Algoritma tidak valid.\n";
    }

    return algorithmName;
}

static string promptHeuristic() {
    string heuristicInput;
    string heuristicName;

    while (heuristicName == "") {
        cout << "Heuristic apa yang anda pilih? (H1/H2/H3)\n";
        if (!(cin >> heuristicInput)) throw runtime_error("Gagal membaca heuristic");

        heuristicName = normalizeHeuristic(heuristicInput);

        if (heuristicName == "") cerr << "Heuristic tidak valid.\n";
    }

    return heuristicName;
}

int main() {
    string algorithmName;
    string heuristicName = "-";

    try {
        Board board = promptBoard();
        algorithmName = promptAlgorithm();

        if (algorithmName == "A*" || algorithmName == "GBFS") {
            heuristicName = promptHeuristic();
        }

        State initialState(board.getStartPosition(), 1, 0, "");
        SolverInput solverInput(board, initialState);
        SolverResult result;

        cout << "Algoritma " << algorithmName << " digunakan.\n";

        if (algorithmName == "UCS") result = UCS::solve(solverInput);
        else if (algorithmName == "GBFS") result = GBFS::solve(solverInput);
        else result = AStar::solve(solverInput);

        Printer::printSolution(board, result);

        if (promptYesNo("Apakah Anda ingin melakukan playback? (Ya/Tidak) : ")) {
            Playback::run(board, result);
        }

        if (promptYesNo("Apakah Anda ingin menyimpan solusi? (Ya/Tidak) : ")) {
            string outputPath;

            cout << "Masukan file output : \n";
            cin >> outputPath;

            bool isSaved = SolutionWriter::save(
                outputPath,
                board,
                result,
                algorithmName,
                heuristicName
            );

            if (isSaved) cout << "Solusi disimpan pada " << outputPath << '\n';
        }
    }
    catch (const exception& error) {
        cerr << error.what() << '\n';
        return 1;
    }

    return 0;
}
