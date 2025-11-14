//
// Created by tim on 13.11.25.
//
#include <iostream>
#include <vector>
#include <limits>
#include <cstdlib>


class TowersOfHanoi {
private:
    //private Classvariables
    std::vector<int> tower1;
    std::vector<int> tower2;
    std::vector<int> tower3;
    int height;

    //Checks if a tower is in a legal state
    //Das const vor den { bedeutet, dass die Methode keine Membervariablen der Klasse verändert.
    bool is_legal(const std::vector<int> &arr) const {
        int last = std::numeric_limits<int>::max();
        for (const int i: arr) {
            if (i > last) return false;
            last = i;
        }
        return true;
    }

    //Prints a single tower
    void print_tower(const std::vector<int> &tower, const char *name) const {
        std::cout << name << ": [";
        for (size_t i = 0; i < tower.size(); ++i) {
            if (i) std::cout << ", ";
            std::cout << tower[i];
        }
        std::cout << "]\n";
    }

    //Moves a disk from one tower to another, returns false if move is illegal (and reverses it)
    bool move_disk(std::vector<int> &from, std::vector<int> &to) {
        if (from.empty()) return false;
        const int disk = from.back();
        from.pop_back();
        to.push_back(disk);

        if (!is_legal(to)) {
            // Undo illegal move
            to.pop_back();
            from.push_back(disk);
            return false;
        }
        return true;
    }

    //Solves the game recursively
    bool solve(std::vector<int> &source, std::vector<int> &target, std::vector<int> &helper, const int n,
               const bool print_steps) {
        if (n <= 0) return true;

        // 1. Move n-1 from source to helper
        if (!solve(source, helper, target, n - 1, print_steps)) return false;

        // 2. Move n-th disk to target
        if (!move_disk(source, target)) return false;

        if (print_steps) print_game_state();

        // 3. Move n-1 from helper to target
        if (!solve(helper, target, source, n - 1, print_steps)) return false;

        return true;
    }

public:
    //Constructor with tower height
    TowersOfHanoi(const int tower_height) {
        setup_game(tower_height);
    }

    //Sets up the game with a given tower height
    void setup_game(const int tower_height) {
        height = tower_height;
        tower1.clear();
        tower2.clear();
        tower3.clear();

        for (int i = height - 1; i >= 0; --i) {
            tower1.push_back(i);
        }
    }

    //Checks if the current game state is legal
    bool is_game_state_legal() const {
        return is_legal(tower1) && is_legal(tower2) && is_legal(tower3);
    }

    //Prints the current game state
    void print_game_state() const {
        std::cout << std::endl;
        print_tower(tower1, "Tower 1");
        print_tower(tower2, "Tower 2");
        print_tower(tower3, "Tower 3");
    }

    //Solves the game, optionally printing each step
    bool solve(const bool print_steps = false) {
        return solve(tower1, tower3, tower2, height, print_steps);
    }
};



int main() {
    // Create a Towers of Hanoi game with 4 disks
    TowersOfHanoi game(4);

    std::cout << "Initial State:";
    game.print_game_state();

    std::cout << "\nSolve Tuerme von Hanoi...\n";

    if (game.solve(true)) {
        std::cout << "Solved Successfully!\n";
    } else {
        std::cout << "Error while solving!\n";
    }

    game.print_game_state();

    std::cout << "State legal: " << (game.is_game_state_legal() ? "true" : "false") << "\n";

    return EXIT_SUCCESS;
}
