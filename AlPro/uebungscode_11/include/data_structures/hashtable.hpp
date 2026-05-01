#pragma once

#include <functional>
#include <memory>
#include <cstddef>
#include <string>

namespace data_structures {

    // In dieser Woche beginnen Sie mit der Implementierung einer Hashtable.
    // Diese Woche wird die Hashtable eine feste Anzahl von Buckets,
    // eine feste Hashfunktion und noch keine Iteratoren enthalten.

    // Anforderungen:
    // Container
    //   size()
    //
    //  HIER DIE ANDORDERUNGSLISTE VERVOLLSTÄNDIGEN
    //  SIEHE AUCH ZETTEL 8 -> include/data_structures/list.hpp
    //
    class Hashtable{
    public:
        // Konstruktor
        Hashtable();

        // Konstruktor mit benutzerdefinierter Hashfunktion
        Hashtable(const std::function<size_t(const std::string&)> &hash_function);

        // Element einfügen
        void insert(const std::string &x);

        // Gibt genau dann true zurück,
        // wenn das Element in der Hashtable enthalten ist
        bool find(const std::string& x) const;

        // Element entfernen
        void remove(const std::string& x);

        // Druckt alle Elemente der Hashtable
        void print() const;

        // Gibt Anzahl der Elemente zurück
        [[nodiscard]] size_t size() const;

        [[nodiscard]] size_t max_bucket_size() const;

    private:
        // Die verwendete Hashfunktion.
        // Um den Bucket eines Elements x zu finden,
        // kann die Hashfkt wie folgt verwendet werden
        //   hashfkt(x) % num_buckets;
        const std::function<size_t(const std::string&)> hashfkt;

        // Die aktuelle Anzahl der Buckets
        size_t num_buckets;
        // Verwenden Sie ihre Listenimplementierung oder
        std::vector<std::vector<std::string>> buckets;

        size_t element_count;

        // Passt die Größe der Hashtable an
        void resize();

        // Überprüft, ob eine Größenänderung der Hashtable notwendig ist
        [[nodiscard]] bool should_resize() const;

        // Berechnet den load factor der Hashtable
        [[nodiscard]] double load_factor() const;


        //[[nodiscard]] sagt dem Compiler:
        //„Der Rückgabewert dieser Funktion sollte nicht ignoriert werden. Wenn der Aufrufer ihn ignoriert, generiere eine Warnung.“
    };

}
