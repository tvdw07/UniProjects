// language: cpp
#include "../include/simpler_graph.h"
#include <iostream>
#include <queue>
#include <stack>

// Konstruktor: Initialisiert den Graphen mit n Knoten
SimplerGraph::SimplerGraph(const int n) : anzahlKnoten(n) {
    // Erstelle eine n x n Matrix, am Anfang mit false gefüllt (keine Kanten)
    adjazenzmatrix.resize(n, std::vector<bool>(n, false));
}

// Fügt eine gerichtete Kante hinzu
bool SimplerGraph::kanteHinzufuegen(const int von, const int nach) {
    if (!istGueltigerKnoten(von) || !istGueltigerKnoten(nach)) {
        return false; // Ungültige Knoten
    }
    adjazenzmatrix[von][nach] = true;
    return true;
}

// Entfernt eine gerichtete Kante
bool SimplerGraph::kanteEntfernen(const int von, const int nach) {
    if (!istGueltigerKnoten(von) || !istGueltigerKnoten(nach)) {
        return false; // Ungültige Knoten
    }
    adjazenzmatrix[von][nach] = false;
    return true;
}

// Prüft, ob eine Kante existiert
bool SimplerGraph::hatKante(const int von, const int nach) const {
    if (!istGueltigerKnoten(von) || !istGueltigerKnoten(nach)) {
        return false;
    }
    return adjazenzmatrix[von][nach];
}

// Gibt die Anzahl der Knoten zurück
int SimplerGraph::getAnzahlKnoten() const {
    return anzahlKnoten;
}

// Gibt alle Knoten und Kanten aus
void SimplerGraph::ausdrucken() const {
    std::cout << "\n=== Graph mit " << anzahlKnoten << " Knoten ===\n" << std::endl;

    // Ausgabe der Knoten
    std::cout << "Knoten: ";
    for (int i = 0; i < anzahlKnoten; i++) {
        std::cout << i;
        if (i < anzahlKnoten - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "\n" << std::endl;

    // Ausgabe der Kanten
    std::cout << "Kanten:" << std::endl;
    bool hatKanten = false;
    for (int i = 0; i < anzahlKnoten; i++) {
        for (int j = 0; j < anzahlKnoten; j++) {
            if (adjazenzmatrix[i][j]) {
                std::cout << "  " << i << " -> " << j << std::endl;
                hatKanten = true;
            }
        }
    }

    if (!hatKanten) {
        std::cout << "  (keine Kanten vorhanden)" << std::endl;
    }

    //Ausgabe der Adjazenzmatrix
    std::cout << "\nAdjazenzmatrix:" << std::endl;
    std::cout << "   ";
    for (int j = 0; j < anzahlKnoten; j++) {
        std::cout << j << " ";
    }
    std::cout << std::endl;

    for (int i = 0; i < anzahlKnoten; i++) {
        std::cout << i << ": ";
        for (int j = 0; j < anzahlKnoten; j++) {
            std::cout << (adjazenzmatrix[i][j] ? "1" : "0") << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

std::vector<int> SimplerGraph::BFS(int startKnoten) const {
    std::vector<int> besuchteKnoten;
    if (!istGueltigerKnoten(startKnoten)) {
        return besuchteKnoten; // Ungültiger Startknoten
    }

    std::vector<bool> besucht(anzahlKnoten, false);
    std::queue<int> warteschlange;

    // Starte von startKnoten
    warteschlange.push(startKnoten);
    besucht[startKnoten] = true;

    while (!warteschlange.empty()) {
        int aktuellerKnoten = warteschlange.front();
        warteschlange.pop();
        besuchteKnoten.push_back(aktuellerKnoten);

        // Füge alle unbesuchten Nachbarn hinzu
        for (int nachbar = 0; nachbar < anzahlKnoten; nachbar++) {
            if (adjazenzmatrix[aktuellerKnoten][nachbar] && !besucht[nachbar]) {
                warteschlange.push(nachbar);
                besucht[nachbar] = true;
            }
        }
    }

    return besuchteKnoten;
}

std::vector<int> SimplerGraph::DFS(const int startKnoten) const {
    std::vector<int> besuchteKnoten;
    if (!istGueltigerKnoten(startKnoten)) {
        return besuchteKnoten; // Ungültiger Startknoten
    }

    std::vector<bool> besucht(anzahlKnoten, false);
    std::stack<int> stapel;

    stapel.push(startKnoten);
    while (!stapel.empty()) {
        int aktuellerKnoten = stapel.top();
        stapel.pop();

        if (!besucht[aktuellerKnoten]) {
            besucht[aktuellerKnoten] = true;
            besuchteKnoten.push_back(aktuellerKnoten);

            // Füge alle unbesuchten Nachbarn hinzu
            for (int nachbar = anzahlKnoten - 1; nachbar >= 0; nachbar--) {
                if (adjazenzmatrix[aktuellerKnoten][nachbar] && !besucht[nachbar]) {
                    stapel.push(nachbar);
                }
            }
        }
    }

    return besuchteKnoten;
}

// Hilfsfunktion: Prüft Knotengültigkeit
bool SimplerGraph::istGueltigerKnoten(const int knoten) const {
    return knoten >= 0 && knoten < anzahlKnoten;
}
