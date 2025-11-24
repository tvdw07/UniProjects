//
// Created by tim on 13.11.25.
//
#include "tuerme_von_Hanoi.h"
#include <iostream>
#include <limits>

// --- Private Hilfsmethoden ---

//Checks if a tower is legal (no bigger disk on a smaller one)
bool TowersOfHanoi::is_legal(const std::vector<int> &arr) const {
    // Größtmöglicher int als Startwert für den vorherigen (oben liegenden) Ring
    int last = std::numeric_limits<int>::max();

    // Gehe alle Scheiben im Turm von oben nach unten durch
    for (int i : arr) {
        // Wenn die aktuelle Scheibe größer ist als die vorherige, ist der Zustand illegal
        if (i > last) return false;
        // Aktuelle Scheibe wird zur vorherigen für den nächsten Durchlauf
        last = i;
    }
    return true;
}

//Method to print the Tower on the console
void TowersOfHanoi::print_tower(const std::vector<int> &tower, const char *name) const {
    std::cout << name << ": [";
    for (size_t i = 0; i < tower.size(); ++i) {
        // Komma-Trenner vor allen Elementen außer dem ersten (i wird als bool interpretiert)
        if (i) std::cout << ", ";
        std::cout << tower[i];
    }
    std::cout << "]\n";
}

// Versucht, eine Scheibe von einem Turm auf einen anderen zu bewegen
// Gibt true zurück, wenn der Zug legal war und ausgeführt wurde
bool TowersOfHanoi::move_disk(std::vector<int> &from, std::vector<int> &to) const {
    // Kein Ring vorhanden, nichts zu tun
    if (from.empty()) return false;

    // Nehme die oberste Scheibe vom Quellturm (back -> oberste)
    int disk = from.back();
    from.pop_back();
    // Lege die Scheibe temporär auf den Zielturm
    to.push_back(disk);

    // Wenn der Zielzustand dadurch illegal wird, Rückgängig machen
    if (!is_legal(to)) {
        // entferne die Scheibe wieder vom Ziel
        to.pop_back();
        // lege sie zurück auf den Quellturm
        from.push_back(disk);
        return false;
    }
    // Zug erfolgreich
    return true;
}

bool TowersOfHanoi::solve(std::vector<int> &source, std::vector<int> &target, std::vector<int> &helper, const int n, const bool print_steps) {
    // Basisfall: keine Scheiben zu bewegen
    if (n <= 0)
        return true;

    // Schritt 1: n-1 Scheiben von source nach helper mittels target
    if (!solve(source, helper, target, n - 1, print_steps))
        return false;

    // Schritt 2: oberste Scheibe von source nach target bewegen
    if (!move_disk(source, target))
        return false;

    if (print_steps)
        print_game_state();

    // Schritt 3: n-1 Scheiben von helper nach target mittels source
    if (!solve(helper, target, source, n - 1, print_steps))
        return false;

    return true;
}

// --- Public Methoden ---

TowersOfHanoi::TowersOfHanoi(const int tower_height) {
    setup_game(tower_height);
}

// Setzt das Spiel zurück und füllt Turm 1 mit Ringen in absteigender Reihenfolge
void TowersOfHanoi::setup_game(int tower_height) {
    // Speichere Höhe und leere alle Türme
    height = tower_height;
    tower1.clear();
    tower2.clear();
    tower3.clear();

    // Fülle Turm1: größte Zahl unten, kleinste oben
    for (int i = height - 1; i >= 0; --i) {
        tower1.push_back(i);
    }
}

// Prüft, ob der gesamte Spielzustand legal ist (alle Türme legal)
bool TowersOfHanoi::is_game_state_legal() const {
    return is_legal(tower1) && is_legal(tower2) && is_legal(tower3);
}

// Gibt den aktuellen Spielzustand (alle drei Türme) aus
void TowersOfHanoi::print_game_state() const {
    // Leerzeile zur Trennung
    std::cout << "\n";
    // Einzelne Türme ausgeben
    print_tower(tower1, "Tower 1");
    print_tower(tower2, "Tower 2");
    print_tower(tower3, "Tower 3");
}

bool TowersOfHanoi::solve(const bool print_steps) {
    return solve(tower1, tower3, tower2, height, print_steps);
}
