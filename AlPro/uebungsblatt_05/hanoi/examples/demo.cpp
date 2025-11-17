//
// Created by Tim on 17.11.2025.
//

#include <iostream>
#include "tuerme_von_Hanoi.h"



int main() {
    // Create a Towers of Hanoi game with 4 disks
    TowersOfHanoi game(4);

    // Print the initial game state
    std::cout << "Initial State:";
    game.print_game_state();

    std::cout << "\nSolve Tuerme von Hanoi...\n";

    // Solve the game with step printing enabled
    if (game.solve(true)) {
        std::cout << "Solved Successfully!\n";
    } else {
        std::cout << "Error while solving!\n";
    }

    // Print the final game state
    game.print_game_state();

    std::cout << "State legal: " << (game.is_game_state_legal() ? "true" : "false") << "\n";

    return EXIT_SUCCESS;
}
