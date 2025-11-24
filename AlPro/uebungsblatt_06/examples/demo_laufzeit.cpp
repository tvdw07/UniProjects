#include <cmath>
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

    // Ausgabe in Tabellenform
    std::cout << "\n=== Test: " << algorithmName << " ===\n";
    std::cout << std::left << std::setw(12) << "N"
              << std::setw(14) << "Zeit (μs)"
              << std::setw(12) << "μs/El"
              << std::setw(18) << "μs / (n log2 n)"
              << "\n";
    std::cout << "-------------------------------------------------------------\n";

    std::vector<double> times;
    // Reserviere Speicher für die Zeiten da wenn wir den speicher nicht reservieren, die werte immer wieder neu alloziert werden bzw ne kopiert werden müssen, da vector dynamisch wächst
    times.reserve(sizes.size());

    for (const int size : sizes) {
        // Messe die durchschnittliche Zeit für die aktuelle Größe
        double avgTime = measureAverageTime(sortFunction, size, iterations);
        times.push_back(avgTime);

        // Zeit / n
        const double timePerElement = avgTime / static_cast<double>(size);

        // Zeit / ( n * log2(n) )
        const double timePerNLogN =
            avgTime / ( static_cast<double>(size) * std::log2(static_cast<double>(size)) );

        std::cout << std::left << std::setw(12) << size
                  << std::setw(14) << std::fixed << std::setprecision(2) << avgTime
                  << std::setw(12) << std::fixed << std::setprecision(6) << timePerElement
                  << std::setw(18) << std::fixed << std::setprecision(9) << timePerNLogN
                  << "\n";
    }
    std::cout << "-------------------------------------------------------------\n";
}


int main() {
    std::cout << "===========================================\n";
    std::cout << "  Laufzeitanalyse: Mergesort vs Schnellsort\n";
    std::cout << "===========================================\n\n";

    // Konfiguration
    const std::vector<int> sizes = {100, 1000, 10000, 100000, 1000000, 5000000, 10000000, 100000000};  // bis 100M möglich
    constexpr int iterations = 10;  // Basis-Wiederholungen pro Größe

    // Ausgabe der Testparameter
    std::cout << "Testparameter: " << sizes.size() << " Messgrößen, max N = " << sizes.back()
              << ", Iterationen (Basis) = " << iterations << "\n";

    // Führe Tests aus
    testAlgorithm(mergeSort, "Mergesort", sizes, iterations);
    testAlgorithm(schnellsort, "Schnellsort (Quicksort)", sizes, iterations);

    return 0;
}

//Beispielausgabe:

/*===========================================
Laufzeitanalyse: Mergesort vs Schnellsort
===========================================

Testparameter: 8 Messgr├Â├ƒen, max N = 100000000, Iterationen (Basis) = 10

=== Test: Mergesort ===
N           Zeit (╬╝s)    ╬╝s/El      ╬╝s / (n log2 n)
-------------------------------------------------------------
100         18.70         0.187000    0.028146305
1000        214.90        0.214900    0.021563782
10000       2213.00       0.221300    0.016654485
100000      23648.00      0.236480    0.014237515
1000000     243292.50     0.243292    0.012206390
5000000     1270179.50    0.254036    0.011415550
10000000    2581013.00    0.258101    0.011099462
100000000   27361897.50   0.273619    0.010295940
-------------------------------------------------------------

=== Test: Schnellsort (Quicksort) ===
N           Zeit (╬╝s)    ╬╝s/El      ╬╝s / (n log2 n)
-------------------------------------------------------------
100         6.10          0.061000    0.009181415
1000        82.90         0.082900    0.008318462
10000       924.20        0.092420    0.006955298
100000      11465.40      0.114654    0.006902859
1000000     136938.50     0.136938    0.006870433
5000000     777139.00     0.155428    0.006984421
10000000    1669194.00    0.166919    0.007178249
100000000   19171461.50   0.191715    0.007213981
-------------------------------------------------------------

Process finished with exit code 0
*/
//Wenn sich ms / (n log2 n) bei steigenden n stabilisiert, deutet das auf eine O(n log n) Laufzeit hin.






//Bonus: Zeige, das Minsort eine O(n^2) Laufzeit hat und O(1) Speicherverbrauch
/*
Laufzeitanalyse O(n²):
- Ebene 1: Suche Minimum von n Elementen → n Vergleiche
- Ebene 2: Suche Minimum von (n-1) Elementen → n-1 Vergleiche
- ...
- Ebene n-1: Suche Minimum von 1 Element → 1 Vergleich

Gesamtanzahl Vergleiche:
T(n) = n + (n-1) + (n-2) + ... + 1
= n*(n+1)/2
= O(n²)

Speicheraufwand O(1):
- Minsort arbeitet in-place:
- nur Indizes (i, j, minIndex) und eine temporäre Variable zum Tauschen
- Keine dynamischen Arrays oder Rekursion
- Daher:
Speicherbedarf S(n) = O(1)
*/