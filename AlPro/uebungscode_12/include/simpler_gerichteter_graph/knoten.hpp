#pragma once
#include <string>

namespace SimplerGerichteterGraph {

    // Wir definieren durch welchen Datentyp der Knotenindex angegeben werden soll.
    using index_t = size_t;

    // Klasse Knoten
    class Knoten {
    public:
        // Konstruktur
        Knoten(const index_t knotenindex);

        Knoten (const Knoten& other);

        Knoten& operator=(const Knoten& other);

        // Index erhalten
        index_t get_index_t() const;

        // Vergleichsoperator der die Indizes zweier Knoten vergleicht
        bool operator== (const Knoten& other) const;

        // Erhakte Stringdarstellung des Knoten
        std::string get_string() const;

    private:
        const index_t idx; // Der Index wird zur Instanziierung festgelegt.
    };

}
