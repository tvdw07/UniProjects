#pragma once
#include <cstddef>

struct Geburtstagsdatum {
    int jahr;
    int monat; // 1-12
    int tag; // 1-31

    Geburtstagsdatum(int j = 2000, const int m = 1, const int t = 1)
        : jahr(j), monat(m), tag(t) {
    }
};
