#include "../include/simpler_graph.h"
#include <iostream>

int main() {
    // Beispiel 1: Kleiner gerichteter Graph mit einem Zyklus
    std::cout << "\nBeispiel 1: Graph mit 4 Knoten" << std::endl;
    SimplerGraph graph1(4);
    graph1.kanteHinzufuegen(0, 1);
    graph1.kanteHinzufuegen(0, 3);
    graph1.kanteHinzufuegen(1, 2);
    graph1.kanteHinzufuegen(2, 3);
    graph1.kanteHinzufuegen(3, 0); // Zyklus
    graph1.ausdrucken();

    const std::vector<int> bfs = graph1.BFS(0);
    std::cout << "BFS ab Knoten 0: ";
    for (const int knoten : bfs) {
        std::cout << knoten << " ";
    }

    const std::vector<int> dfs = graph1.DFS(0);
    std::cout << "\nDFS ab Knoten 0: ";
    for (const int knoten : dfs) {
        std::cout << knoten << " ";
    }


    return EXIT_SUCCESS;
}
