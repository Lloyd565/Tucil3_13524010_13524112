#include "output/Printer.hpp"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

void Printer::printBoard(const Board& board, const State& state) {
    Position playerPosition = state.getPlayerPosition();

    for (int i = 0; i < board.getRows(); i++) {
        for (int j = 0; j < board.getCols(); j++) {
            Position currentPosition(i, j);

            if (currentPosition == playerPosition) cout << 'Z';
            else {
                char tile = board.getTile(currentPosition);

                if (tile == 'Z') cout << '*';
                else cout << tile;
            }
        }

        cout << '\n';
    }
}

void Printer::printSolution(const Board& board, const SolverResult& result) {
    if (!result.isFound()) {
        cout << "Solusi tidak ditemukan.\n";
        cout << "Cost dari Solusi : -\n";
        cout << "Waktu eksekusi: "
             << result.getExecutionTime()
             << " ms\n";
        cout << "Banyak iterasi yang dilakukan: "
             << result.getIterations()
             << " iterasi\n";
        return;
    }

    cout << "Solusi Yang Ditemukan : "
         << result.getMoves()
         << '\n';

    cout << "Cost dari Solusi : "
         << result.getTotalCost()
         << '\n';

    cout << '\n';

    printFullStep(board, result);

    cout << "\nWaktu eksekusi: "
         << result.getExecutionTime()
         << " ms\n";

    cout << "Banyak iterasi yang dilakukan: "
         << result.getIterations()
         << " iterasi\n";
}

void Printer::printFullStep(
    const Board& board,
    const SolverResult& result
) {
    const vector<State>& steps = result.getSolutionSteps();
    const string& moves = result.getMoves();

    if (steps.empty()) return;

    cout << "Initial\n";
    printBoard(board, steps[0]);
    cout << '\n';

    for (int i = 1; i < static_cast<int>(steps.size()); i++) {
        char moveChar = '?';

        if (i - 1 < static_cast<int>(moves.size())) moveChar = moves[i - 1];

        printStep(board, steps[i], i, moveChar);
        cout << '\n';
    }
}

void Printer::printStep(
    const Board& board,
    const State& state,
    int stepNumber,
    char move
) {
    cout << "Step " << stepNumber << " : " << move << '\n';
    printBoard(board, state);
}
