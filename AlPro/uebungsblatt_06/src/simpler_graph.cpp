#include "../include/simpler_graph.h"
#include <iostream>

// Konstruktor: Initialisiert den Graphen mit n Knoten
SimplerGraph::SimplerGraph(const int n) : anzahlKnoten(n) {
    // Erstelle eine n x n Matrix, initial mit false gefüllt (keine Kanten)
    adjazenzmatrix.resize(n, std::vector<bool>(n, false));
}

// Fügt eine gerichtete Kante hinzu
bool SimplerGraph::kanteHinzufuegen(const int von, const int nach) {
    if (!istGueltigerKnoten(von) || !istGueltigerKnoten(nach)) {
        return false;  // Ungültige Knoten
    }
    adjazenzmatrix[von][nach] = true;
    return true;
}

// Entfernt eine gerichtete Kante
bool SimplerGraph::kanteEntfernen(const int von, const int nach) {
    if (!istGueltigerKnoten(von) || !istGueltigerKnoten(nach)) {
        return false;  // Ungültige Knoten
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

// Hilfsfunktion: Prüft Knotengültigkeit
bool SimplerGraph::istGueltigerKnoten(const int knoten) const {
    return knoten >= 0 && knoten < anzahlKnoten;
}

