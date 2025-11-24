//
// Created by Tim on 17.11.2025.
//

#include "minsort_ip.h"
#include <iostream>
#include <random>

// Gibt den Index des kleinsten Elements ab Position 'start' zurück
int get_index_of_min(const std::vector<int> &arr, const size_t start) {
    size_t min_index = start;

    //For schleife durchsucht das Array ab 'start' nach dem kleinsten Wert (also nur den unsortieren Teil)
    for (size_t i = start + 1; i < arr.size(); ++i) {
        if (arr[i] < arr[min_index]) {
            min_index = i; // neuen Minimalwert gefunden
        }
    }
    return static_cast<int>(min_index);
}

// Tauscht zwei Integer-Werte per Referenz
void swapAB(int &a, int &b) {
    const int temp = a;
    a = b;
    b = temp;
}

// Selektionssort: sucht das Minimum im Restarray und tauscht es nach vorn
void minSort(std::vector<int> &arr) {
    for (size_t i = 0; i < arr.size(); ++i) {
        const int min_index = get_index_of_min(arr, i);
        swapAB(arr[i], arr[min_index]); // Minimum nach vorne holen
    }
}

// Erstellt ein Array zufälliger Ganzzahlen im Bereich [0, 99]
std::vector<int> generateRandomArray(const int size) {
    std::vector<int> arr(size);
    std::mt19937 rng(std::random_device{}());           // Zufallszahlengenerator
    std::uniform_int_distribution<int> dist(0, 99);     // Wertebereich

    // Fill the array with random values
    //auto stands for the type of the variable x, which is int in this case
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
