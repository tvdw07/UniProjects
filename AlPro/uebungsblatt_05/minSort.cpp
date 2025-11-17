//
// Created by Tim on 10.11.2025.
//

#include <iostream>
#include <ostream>
#include <vector>
#include <random>

/**
 * @brief Liefert den Index des kleinsten Elements im Bereich [start, arr.size()).
 *
 * @param arr Konstante Referenz auf das zu durchsuchende Vektor.
 * @param start Beginnindex der Suche (inklusive). Muss \< arr.size() sein.
 * @return int Index des kleinsten Elements (als int zurückgegeben, entspricht
 *             dem positionellen Index).
 *
 * @complexity O(n) relativ zur Länge des betrachteten Teilbereichs.
 */
int get_index_of_min(const std::vector<int> &arr, size_t start) {
    size_t min_index = start;
    for (size_t i = start + 1; i < arr.size(); ++i) {
        if (arr[i] < arr[min_index]) {
            min_index = i;
        }
    }
    return static_cast<int>(min_index);
}


//Tauscht die Werte von a und b
void swapAB(int &a, int &b) {
    const int temp = a;
    a = b;
    b = temp;
}

/**
 * @brief Sortiert das gegebene Vektor in‑place mittels Selektionssort (MinSort).
 *
 * Für jede Position i wird das kleinste Element im Bereich [i, end) gesucht
 * und an Position i gesetzt. Sortierung erfolgt in‑place;
 *
 * @param arr Referenz auf das zu sortierende Vektor.
 *
 * @complexity O(n^2) Laufzeit
 */
void minSort(std::vector<int> &arr) {
    for (size_t i = 0; i < arr.size(); ++i) {
        const int min_index = get_index_of_min(arr, i);
        swapAB(arr[i], arr[min_index]);
    }
}

// Hilfsfunktion zum Generieren eines Arrays mit zufälligen Ganzzahlen
std::vector<int> generateRandomArray(const int size) {
    std::vector<int> arr(size);
    std::mt19937 rng(std::random_device{}()); // moderner Zufallsgenerator
    std::uniform_int_distribution<int> dist(0, 99);
    for (auto &x: arr) x = dist(rng);
    return arr;
}


// Hilfsfunktion zum Drucken eines Arrays
void printArray(const std::vector<int> &arr) {
    for (const int i: arr) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}


int main() {
    // Beispielverwendung
    std::vector<int> arr = generateRandomArray(10);

    printArray(arr);

    minSort(arr);
    printArray(arr);
}
