#include <simpler_gerichteter_graph/gerichteter_graph.hpp>

#include <iostream>

int main() {
    SimplerGerichteterGraph::GerichteterGraph g;

    g.knoten_einfuegen(1);
    g.knoten_einfuegen(2);
    g.knoten_einfuegen(42);
    g.drucke();

    g.kante_einfuegen(1,2);
    g.kante_einfuegen(2,42);
    g.drucke();

    g.kante_einfuegen(2,5);
    g.kante_einfuegen(42,1);
    g.kante_einfuegen(42,2);
    g.kante_einfuegen(42,3);
    g.drucke();

    for(const auto& e : g.get_auslaufende(42)) {
        std::cout << e.get_string() << " ";
    }
    std::cout << std::endl;

    for(const auto& e : g.get_auslaufende(1000)) {
        std::cout << e.get_string() << " ";
    }
    std::cout << std::endl;

    std::cout << "Breitensuche ab 1: ";
    for (const auto& k : g.breitensuche(1)) {
        std::cout << k.get_string() << " ";
    }
    std::cout << std::endl;

    std::cout << "Tiefensuche ab 1: ";
    for (const auto& k : g.tiefensuche(1)) {
        std::cout << k.get_string() << " ";
    }
    std::cout << std::endl;

    g.kante_entfernen(42,1);
    g.knoten_entfernen(1);
    g.drucke();
}
