#include <simpler_gerichteter_graph/knoten.hpp>

namespace SimplerGerichteterGraph{

    // Konstruktur
    Knoten::Knoten(const index_t knotenindex) :
      idx(knotenindex)
    {}

    Knoten::Knoten (const Knoten& other) :
      idx(other.idx) {}

    Knoten& Knoten::operator=(const Knoten& other) {
        const_cast<index_t&>(this->idx) = other.idx;
        return *this;
    }

    // Vergleichsoperator der die Indizes zweier Knoten vergleicht
    bool Knoten::operator== (const Knoten& other) const {
        return idx == other.idx;
    }

    // Index erhalten
    index_t Knoten::get_index_t() const {
        return idx;
    }

    // Erhakte Stringdarstellung des Knoten
    std::string Knoten::get_string() const {
        return std::to_string(idx);
    }
}
