#include <schnellsort.cpp>

#include <vector>
#include <algorithm>


void schnellsort(std::vector<int>& v) {
    // Wir verwenden die Standardbibliothek zum sortieren.
    std::sort(v.begin(), v.end(), [](const int x, const int y) { return x < y; });
}
