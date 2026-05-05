#include "output/Playback.hpp"
#include "output/Printer.hpp"

#include <iostream>
#include <limits>
#include <string>
#include <vector>

using namespace std;

void Playback::run(const Board& board, const SolverResult& result) {
    if (!result.isFound()) {
        cout << "Tidak ada solusi untuk playback.\n";
        return;
    }

    const vector<State>& steps = result.getSolutionSteps();

    if (steps.empty()) {
        cout << "Tidak ada step solusi untuk playback.\n";
        return;
    }

    int currentStep = 0;
    char command;

    while (true) {
        cout << "\n==============================\n";
        showStep(board, result, currentStep);
        cout << "==============================\n";

        cout << "[n] next | [p] previous | [j] jump | [q] quit\n";
        cout << "Input: ";
        cin >> command;

        if (command == 'n' || command == 'N') {
            if (currentStep + 1 < static_cast<int>(steps.size())) currentStep++;
            else cout << "Sudah berada di step terakhir.\n";
        }
        else if (command == 'p' || command == 'P') {
            if (currentStep - 1 >= 0) currentStep--;
            else cout << "Sudah berada di step awal.\n";
        }
        else if (command == 'j' || command == 'J') {
            int targetStep;

            cout << "Masukkan nomor step 0 - "
                 << static_cast<int>(steps.size()) - 1
                 << ": ";

            if (!(cin >> targetStep)) {
                cout << "Input step tidak valid.\n";

                cin.clear();
                cin.ignore(
                    numeric_limits<streamsize>::max(),
                    '\n'
                );

                continue;
            }

            if (
                targetStep >= 0 &&
                targetStep < static_cast<int>(steps.size())
            ) currentStep = targetStep;
            else cout << "Nomor step di luar range.\n";
        }
        else if (command == 'q' || command == 'Q') break;
        else cout << "Command tidak dikenal.\n";
    }
}

void Playback::showStep(
    const Board& board,
    const SolverResult& result,
    int index
) {
    const vector<State>& steps = result.getSolutionSteps();
    const string& moves = result.getMoves();

    if (index < 0 || index >= static_cast<int>(steps.size())) {
        cout << "Step tidak valid.\n";
        return;
    }

    if (index == 0) {
        cout << "Initial\n";
        Printer::printBoard(board, steps[index]);
        return;
    }

    char moveChar = '?';

    if (index - 1 < static_cast<int>(moves.size())) moveChar = moves[index - 1];

    cout << "Step " << index << " : " << moveChar << '\n';
    Printer::printBoard(board, steps[index]);
}
