#include "output/SolutionWriter.hpp"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

bool SolutionWriter::save(
    const string& filePath,
    const Board& board,
    const SolverResult& result,
    const string& algorithmName,
    const string& heuristicName
) {
    ofstream out(filePath);

    if (!out.is_open()) {
        cerr << "Gagal membuka file output.\n";
        return false;
    }

    out << "Algorithm: " << algorithmName << '\n';
    out << "Heuristic: " << heuristicName << '\n';
    out << '\n';

    if (!result.isFound()) {
        out << "Solusi tidak ditemukan.\n";
        out << "Waktu eksekusi: "
            << result.getExecutionTime()
            << " ms\n";
        out << "Banyak iterasi yang dilakukan: "
            << result.getIterations()
            << " iterasi\n";

        return true;
    }

    out << "Solusi Yang Ditemukan : "
        << result.getMoves()
        << '\n';

    out << "Cost dari Solusi : "
        << result.getTotalCost()
        << '\n';

    out << '\n';

    writeSolutionSteps(out, board, result);

    out << '\n';

    out << "Waktu eksekusi: "
        << result.getExecutionTime()
        << " ms\n";

    out << "Banyak iterasi yang dilakukan: "
        << result.getIterations()
        << " iterasi\n";

    return true;
}

void SolutionWriter::writeBoard(
    ofstream& out,
    const Board& board,
    const State& state
) {
    Position playerPosition = state.getPlayerPosition();

    for (int i = 0; i < board.getRows(); i++) {
        for (int j = 0; j < board.getCols(); j++) {
            Position currentPosition(i, j);

            if (currentPosition == playerPosition) out << 'Z';
            else {
                char tile = board.getTile(currentPosition);

                if (tile == 'Z') out << '*';
                else out << tile;
            }
        }

        out << '\n';
    }
}

void SolutionWriter::writeSolutionSteps(
    ofstream& out,
    const Board& board,
    const SolverResult& result
) {
    const vector<State>& steps = result.getSolutionSteps();
    const string& moves = result.getMoves();

    if (steps.empty()) return;

    out << "Initial\n";
    writeBoard(out, board, steps[0]);
    out << '\n';

    for (int i = 1; i < static_cast<int>(steps.size()); i++) {
        char moveChar = '?';

        if (i - 1 < static_cast<int>(moves.size())) moveChar = moves[i - 1];

        out << "Step " << i << " : " << moveChar << '\n';
        writeBoard(out, board, steps[i]);
        out << '\n';
    }
}
