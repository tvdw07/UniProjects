#include <iostream>
#include <vector>
#include <string>

// (1) Funktion zum Drucken eines int-Arrays
// Übergabe: const reference, um unnötige Kopien zu vermeiden
// Iteration: const reference, da Werte nur gelesen, nicht verändert werden
void print_array(const std::vector<int>& arr) {
    for (const int& i : arr) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

// (2) Funktion, die alle Werte durch ihren Betrag ersetzt
// Übergabe: reference, da das Array verändert werden soll
// Iteration: reference, da wir Werte im Array direkt anpassen wollen
void make_absolute(std::vector<int>& arr, int value) {
    for (int& i : arr) {
        if (i == value) {
            i = std::abs(i);
        }

    }
}

// (3) Funktion zum Drucken eines string-Arrays (Überladen der Ersten Funktion)
// Übergabe: const reference, da das Array nicht verändert wird
// Iteration: const reference, um Kopien von Strings zu vermeiden zudem nicht nötig für ausgabe
void print_array(const std::vector<std::string>& arr) {
    for (const std::string& s : arr) {
        std::cout << s << std::endl;
    }
}

// (4) Funktion, die weitere Werte einliest und anfügt
// Übergabe: reference, da das Array erweitert werden soll
// Keine Rückgabe nötig, da der Eingriff direkt auf dem übergebenen Objekt passiert
void append_values(std::vector<int>& arr) {
    int value;
    std::cout << "Bitte ganze Zahlen eingeben (42 beendet die Eingabe): ";
    while (std::cin >> value) {
        if (value == 42) break;
        arr.push_back(value);
    }
}

// Beispielmain zum Testen aller Funktionen
int main() {
    std::vector nums = {-5, 0, 3, -9};
    const std::vector<std::string> words = {"Hallo", "Welt", "C++"};

    std::cout << "Array vorher: ";
    print_array(nums);

    make_absolute(nums, -5);
    std::cout << "Nach Betrag-Umwandlung: ";
    print_array(nums);

    std::cout << "\nString-Array:\n";
    print_array(words);

    append_values(nums);
    std::cout << "\nArray nach dem Hinzufuegen: ";
    print_array(nums);

    return EXIT_SUCCESS;
}
