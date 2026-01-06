#include <data_structures/hashtable.hpp>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

namespace data_structures {
    Hashtable::Hashtable()
        : hashfkt(std::hash<std::string>()),
          num_buckets(32) {
    }

    void Hashtable::insert(const std::string &x) {

        if (num_buckets == 0) return; //keine Buckets vorhanden, nichts tun

        //Buckets initialisieren, falls noch nicht geschehen
        if (buckets.empty()) buckets.resize(num_buckets);

        //Index des Buckets berechnen
        const size_t idx = hashfkt(x) % num_buckets;
        auto &bucket = buckets[idx];

        //Nur einfügen, wenn das Element noch nicht vorhanden ist
        if (std::ranges::find(bucket, x) == bucket.end()) {
            bucket.push_back(x);
        }
    }

    bool Hashtable::find(const std::string &x) const {
        if (num_buckets == 0 || buckets.empty()) return false; //nichts vorhanden

        //Bucket-Index berechnen
        const size_t idx = hashfkt(x) % num_buckets;
        const auto &bucket = buckets[idx];

        //true, falls Element gefunden, sonst false
        return std::ranges::find(bucket, x) != bucket.end();
    }

    void Hashtable::remove(const std::string &x) {
        if (num_buckets == 0 || buckets.empty()) return; //nichts vorhanden

        //Bucket-Index berechnen
        const size_t idx = hashfkt(x) % num_buckets;
        auto &bucket = buckets[idx];

        //Element im Bucket suchen
        const auto it = std::ranges::find(bucket, x);
        if (it != bucket.end()) bucket.erase(it);
    }

    //Gibt den Inhalt aller Buckets aus
    void Hashtable::print() const {
        if (buckets.empty()) return; //nichts zu drucken

        for (size_t i = 0; i < buckets.size(); ++i) {
            const auto &b = buckets[i];
            std::cout << "Bucket " << i << ":";

            //Alle Elemente des Buckets ausgeben
            for (const auto &s: b) std::cout << " " << s;

            std::cout << "\n";
        }
    }

    size_t Hashtable::size() const {
        if (buckets.empty()) return 0; //keine Buckets, also keine Elemente

        size_t cnt = 0;

        //Größe aller Buckets aufsummieren
        for (const auto &b: buckets) cnt += b.size();
        return cnt;
    }
}