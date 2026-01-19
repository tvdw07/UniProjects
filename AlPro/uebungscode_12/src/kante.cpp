#include <simpler_gerichteter_graph/kante.hpp>

namespace SimplerGerichteterGraph {

    // Konstruktur
    Kante::Kante(const Knoten& startknoten, const Knoten& zielknoten) :
      start(startknoten), ziel(zielknoten)
    {}

    // Erhalte den Startknoten (als Referenz)
    const Knoten& Kante::get_startknoten() const {
        return start;
    }

    // Erhalte den Zielknoten (als Referenz)
    const Knoten& Kante::get_zielknoten() const {
        return ziel;
    }

    // Vergleichsoperator der die Indizes zweier Knoten vergleicht
    bool Kante::operator== (const Kante& other) const {
        return start == other.start and ziel == other.ziel;
    }

    std::string Kante::get_string() const {
        return "(" + start.get_string() + " -> " + ziel.get_string() + ")";
    }
}
