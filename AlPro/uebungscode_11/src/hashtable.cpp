#include <data_structures/hashtable.hpp>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>
#include <iostream>

namespace data_structures {
    Hashtable::Hashtable()
        : hashfkt(std::hash<std::string>()),
          num_buckets(32),
          buckets(num_buckets),
          element_count(0) {
    }


    Hashtable::Hashtable(const std::function<size_t(const std::string &)> &hash_function)
        : hashfkt(hash_function),
          num_buckets(32),
          buckets(num_buckets),
          element_count(0) {
    }


    void Hashtable::insert(const std::string &x) {
        if (num_buckets == 0) [[unlikely]] return; //keine Buckets vorhanden, nichts tun

        //Index des Buckets berechnen
        const size_t idx = hashfkt(x) % num_buckets;
        auto &bucket = buckets[idx];

        //Nur einfügen, wenn das Element noch nicht vorhanden ist
        if (std::ranges::find(bucket, x) == bucket.end()) {
            bucket.push_back(x);

            //Elementzähler erhöhen
            element_count++;

            if (should_resize())
                resize();
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
        if (num_buckets == 0 || buckets.empty()) [[unlikely]] return; //nichts vorhanden

        //Bucket-Index berechnen
        const size_t idx = hashfkt(x) % num_buckets;
        auto &bucket = buckets[idx];

        //Element im Bucket suchen
        const auto it = std::ranges::find(bucket, x);
        if (it != bucket.end()) {
            //Element gefunden, entfernen
            bucket.erase(it);

            //Elementzähler verringern
            element_count--;

            if (should_resize())
                resize();
        }
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
        if (buckets.empty()) [[unlikely]] return 0; //keine Buckets, also keine Elemente
        return element_count;
    }

    //Berechnet den load factor der Hashtable
    double Hashtable::load_factor() const {
        if (num_buckets == 0) [[unlikely]] return 0.0;
        //Load factor = Anzahl Elemente / Anzahl Buckets
        return static_cast<double>(size()) / num_buckets;
    }

    //Überprüft, ob eine Größenänderung der Hashtable notwendig ist wenn der load factor zu hoch oder zu niedrig ist
    bool Hashtable::should_resize() const {
        const double lf = load_factor();
        //Vergrößern, wenn load factor > 0.75
        //Verkleinern, wenn load factor < 0.25 und mehr als 8 Buckets vorhanden sind
        return lf > 0.75 || (lf < 0.25 && num_buckets > 8);
    }


    //Passt die Größe der Hashtable an
    void Hashtable::resize() {
        std::cout << "Resizing hashtable. Current load factor: " << load_factor() << "\n";
        const double lf = load_factor();

        //Neue Bucket-Anzahl initialisieren
        size_t new_bucket_count = num_buckets;

        //Neue Bucket-Anzahl bestimmen
        if (lf > 0.75)
            new_bucket_count = num_buckets * 2;
        else if (lf < 0.25 && num_buckets > 8)
            new_bucket_count = num_buckets / 2;
        else
            [[unlikely]] return;

        std::vector<std::vector<std::string> > new_buckets(new_bucket_count);

        //Alle Elemente in die neuen Buckets umverteilen
        for (const auto &bucket: buckets) {
            for (const auto &s: bucket) {
                const size_t new_idx = hashfkt(s) % new_bucket_count;
                new_buckets[new_idx].push_back(s);
            }
        }

        //Neue Buckets übernehmen
        buckets = std::move(new_buckets);
        num_buckets = new_bucket_count;
    }

    //Gibt die maximale Bucket-Größe zurück
    size_t Hashtable::max_bucket_size() const {
        size_t max = 0;
        for (const auto& b : buckets)
            if (b.size() > max)
                max = b.size();
        return max;
    }

}
