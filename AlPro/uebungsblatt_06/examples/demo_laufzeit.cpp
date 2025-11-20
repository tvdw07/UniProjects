#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <mergesort.h>
#include <schnellsort.cpp>
#include <zeitmesser.cpp>


// void (*sortFunction)(std::vector<int>&) - Funktionszeiger auf die Sortierfunktion
double measureAverageTime(void (*sortFunction)(std::vector<int>&), const int size, int iterations) {
    // adaptive iterations: bei sehr großen Arrays weniger Wiederholungen
    if (size >= 1000000 && iterations > 5) iterations = std::max(1, iterations / 4);

    double totalTime = 0.0;
    for (int i = 0; i < iterations; ++i) {
        std::vector<int> arr = generateRandomArray(size);

        Zeitmesser timer;
        timer.start();
        sortFunction(arr);
        timer.stop();
        const double t = timer.get_microseconds();
        // falls Timer 0 oder negativ (unwahrscheinlich), ignoriere Iteration
        if (t > 0.0) totalTime += t;
        else iterations = std::max(1, iterations - 1);
    }
    return totalTime / static_cast<double>(std::max(1, iterations));
}


// void (*sortFunction)(std::vector<int>&) - Funktionszeiger auf die Sortierfunktion -> Ermöglicht eine Methode für beide Sortieralgorithmen
void testAlgorithm(void (*sortFunction)(std::vector<int>&),
                   const std::string& algorithmName,
                   const std::vector<int>& sizes,
                   const int iterations) {

    std::cout << "\n=== Test: " << algorithmName << " ===\n";
    std::cout << std::left << std::setw(12) << "N"
              << std::setw(14) << "Zeit (μs)"
              << std::setw(12) << "μs/El" << "\n";
    std::cout << "--------------------------------\n";

    std::vector<double> times;
    // Reserviere Speicher für die Zeiten
    times.reserve(sizes.size());

    for (const int size : sizes) {
        double avgTime = measureAverageTime(sortFunction, size, iterations);
        times.push_back(avgTime);

        const double timePerElement = avgTime / static_cast<double>(size);

        std::cout << std::left << std::setw(12) << size
                  << std::setw(14) << std::fixed << std::setprecision(2) << avgTime
                  << std::setw(12) << std::fixed << std::setprecision(6) << timePerElement
                  << "\n";
    }
    std::cout << "--------------------------------\n";
}

int main() {
    std::cout << "===========================================\n";
    std::cout << "  Laufzeitanalyse: Mergesort vs Schnellsort\n";
    std::cout << "===========================================\n\n";

    // Konfiguration
    const std::vector<int> sizes = {100, 1000, 10000, 100000, 1000000, 5000000, 10000000};  // bis 10M möglich
    constexpr int iterations = 10;  // Basis-Wiederholungen pro Größe

    // Ausgabe der Testparameter
    std::cout << "Testparameter: " << sizes.size() << " Messgrößen, max N = " << sizes.back()
              << ", Iterationen (Basis) = " << iterations << "\n";

    // Führe Tests aus
    testAlgorithm(mergeSort, "Mergesort", sizes, iterations);
    testAlgorithm(schnellsort, "Schnellsort (Quicksort)", sizes, iterations);

    return 0;
}
