#pragma once

#include <simpler_gerichteter_graph/knoten.hpp>
#include <string>

namespace SimplerGerichteterGraph {

    // Klasse Kanten
    class Kante {
    public:
        // Konstruktur
        Kante(const Knoten& startknoten, const Knoten& zielknoten);

        // Erhalte den Startknoten (als Referenz)
        const Knoten& get_startknoten() const;

        // Erhalte den Zielknoten (als Referenz)
        const Knoten& get_zielknoten() const;

        // Vergleichsoperator der die Indizes zweier Knoten vergleicht
        bool operator== (const Kante& other) const;

        // Erhalte Stringdarstellung der Kante
        std::string get_string() const;

    private:
        const Knoten start;
        const Knoten ziel;
    };

} // namespace SimplerGerichteterGraph

