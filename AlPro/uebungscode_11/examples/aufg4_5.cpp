#include "Studi.hpp"
#include <vector>
#include <algorithm>
#include <iostream>

int main() {

    std::vector<Studi> studis = {
        {"Max", "Mustermann", {2000,5,12}, 1.80},
        {"Anna", "Schmidt", {1999,7,3}, 1.65},
        {"Lukas", "Meier", {2001,12,25}, 1.78},
        {"Laura", "Klein", {2000,1,15}, 1.70},
        {"Tim", "Gerdel", {2002,3,1}, 1.75},
        {"Mia", "Becker", {1998,11,20}, 1.68},
        {"Jonas", "Fischer", {2001,6,7}, 1.82},
        {"Lea", "Wagner", {1999,9,30}, 1.60},
        {"Paul", "Schulz", {2000,8,12}, 1.77},
        {"Sara", "Hoffmann", {2001,2,5}, 1.69},
        {"Felix", "Zimmermann", {2000,4,10}, 1.74},
        {"Emma", "Krause", {2002,5,18}, 1.66},
        {"Nina", "Schneider", {2001,7,22}, 1.72},
        {"Tim", "Lange", {1999,10,9}, 1.81},
        {"Lena", "Wolf", {2000,12,1}, 1.63},
        {"Ben", "Richter", {2001,1,28}, 1.79}
    };

    // 1) Nach Vornamen
    std::ranges::sort(studis,
                      [](const Studi &a, const Studi &b){ return a.getVorname() < b.getVorname(); });
    std::cout << "== Nach Vornamen ==\n";
    for (const auto &s : studis) s.drucken();

    // 2) Nach Nachnamen
    std::ranges::sort(studis,
                      [](const Studi &a, const Studi &b){ return a.getNachname() < b.getNachname(); });
    std::cout << "\n== Nach Nachnamen ==\n";
    for (const auto &s : studis) s.drucken();

    // 3) Nach Alter (aufsteigend)
    std::ranges::sort(studis,
                      [](const Studi &a, const Studi &b){ return a.getGeburtstag().jahr < b.getGeburtstag().jahr; });
    std::cout << "\n== Nach Alter ==\n";
    for (const auto &s : studis) s.drucken();

    // 4) Nach Körperhöhe (absteigend)
    std::ranges::sort(studis,
                      [](const Studi &a, const Studi &b){ return a.getKoerperhoehe() > b.getKoerperhoehe(); });
    std::cout << "\n== Nach Körperhöhe ==\n";
    for (const auto &s : studis) s.drucken();

    return 0;
}
