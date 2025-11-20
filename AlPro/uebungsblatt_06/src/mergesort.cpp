//
// Created by Tim on 18.11.2025.
//

#include "mergesort.h"
#include <iostream>
#include <random>


//Youtube: Algorithmen und Datenstrukturen - Mergesort (Merge Sort) - YouTube (Pseudocode)

// Führt zwei sortierte Teilarrays zusammen
void merge(std::vector<int> &arr, const std::size_t left, const std::size_t mid, const std::size_t right) {
    // Erstelle temporäre Arrays für linkes und rechtes Teilarray
    const std::vector<int> leftArr(arr.begin() + left, arr.begin() + mid);
    const std::vector<int> rightArr(arr.begin() + mid, arr.begin() + right);

    std::size_t i = 0;                // Index im linken Array
    std::size_t j = 0;                // Index im rechten Array
    std::size_t k = left;             // Index im Originalarray

    // 2. Merge-Schritt: kleinstes Element zurückschreiben
    while (i < leftArr.size() && j < rightArr.size()) {
        if (leftArr[i] <= rightArr[j]) { // stabil
            arr[k++] = leftArr[i++];
        } else {
            arr[k++] = rightArr[j++];
        }
    }

    // 3. Restliche Elemente aus leftArr (falls vorhanden)
    while (i < leftArr.size()) {
        arr[k++] = leftArr[i++];
    }

    // 4. Restliche Elemente aus rightArr (falls vorhanden)
    while (j < rightArr.size()) {
        arr[k++] = rightArr[j++];
    }
}


// Rekursive Mergesort-Funktion
void mergeSortRecursive(std::vector<int> &arr, const std::size_t left, const std::size_t right) {
    // 1. Basisfall: wenn left >= right - 1, ist das Array bereits sortiert
    if (left >= right - 1) return;

    // 2. Rekursionsfall:
    // Berechne die Mitte (mid)
    const std::size_t mid = left + (right - left) / 2;

    //Sortiere die linke Hälfte rekursiv
    mergeSortRecursive(arr, left, mid);

    //Sortiere die rechte Hälfte rekursiv
    mergeSortRecursive(arr, mid, right);

    //Führe beide Hälften zusammen (merge)
    merge(arr, left, mid, right);
}

// Wrapper-Funktion für Mergesort
void mergeSort(std::vector<int> &arr) {
    if (arr.empty()) {
        return;
    }
    mergeSortRecursive(arr, 0, arr.size());
}

// Erstellt ein Array zufälliger Ganzzahlen im Bereich [0, 99]
std::vector<int> generateRandomArray(const int size) {
    std::vector<int> arr(size);
    std::mt19937 rng(std::random_device{}());           // Zufallszahlengenerator
    std::uniform_int_distribution<int> dist(0, 99);     // Wertebereich

    for (auto &x : arr) {
        x = dist(rng); // Zufallswert eintragen
    }

    return arr;
}

// Gibt alle Elemente des Arrays aus
void printArray(const std::vector<int> &arr) {
    for (const int i : arr) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

