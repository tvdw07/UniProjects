#include "../include/simpler_graph.h"
#include <iostream>

int main() {
    // Beispiel 1: Kleiner Graph mit 4 Knoten
    std::cout << "\nBeispiel 1: Graph mit 4 Knoten" << std::endl;
    SimplerGraph graph1(4);

    std::cout << "\nLeerer Graph:" << std::endl;
    graph1.ausdrucken();

    // Kanten hinzufügen
    std::cout << "\nFuege Kanten hinzu:" << std::endl;
    graph1.kanteHinzufuegen(0, 1);
    std::cout << "  Kante 0 -> 1 hinzugefuegt" << std::endl;
    graph1.kanteHinzufuegen(0, 2);
    std::cout << "  Kante 0 -> 2 hinzugefuegt" << std::endl;
    graph1.kanteHinzufuegen(1, 2);
    std::cout << "  Kante 1 -> 2 hinzugefuegt" << std::endl;
    graph1.kanteHinzufuegen(2, 3);
    std::cout << "  Kante 2 -> 3 hinzugefuegt" << std::endl;
    graph1.kanteHinzufuegen(3, 0);
    std::cout << "  Kante 3 -> 0 hinzugefuegt (Zyklus!)" << std::endl;

    graph1.ausdrucken();

    // Kante entfernen
    std::cout << "\nEntferne Kante 1 -> 2:" << std::endl;
    graph1.kanteEntfernen(1, 2);
    graph1.ausdrucken();

    // Kante prüfen
    std::cout << "\nPruefe Kanten:" << std::endl;
    std::cout << "  Hat Kante 0 -> 1? " << (graph1.hatKante(0, 1) ? "Ja" : "Nein") << std::endl;
    std::cout << "  Hat Kante 1 -> 2? " << (graph1.hatKante(1, 2) ? "Ja" : "Nein") << std::endl;
    std::cout << "  Hat Kante 1 -> 0? " << (graph1.hatKante(1, 0) ? "Ja" : "Nein") << std::endl;

    // Beispiel 2: Vollständiger gerichteter Graph
    std::cout << "\n\nBeispiel 2: Vollstaendiger Graph mit 3 Knoten" << std::endl;
    SimplerGraph graph2(3);

    std::cout << "\nFuege alle moeglichen Kanten hinzu:" << std::endl;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (i != j) {
                graph2.kanteHinzufuegen(i, j);
            }
        }
    }
    graph2.ausdrucken();

    return 0;
}
