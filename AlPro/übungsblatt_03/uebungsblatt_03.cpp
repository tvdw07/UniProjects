//
// Created by Tim on 01.11.2025.
//

//Quelle Taylor Reihe: https://www.lernhelfer.de/schuelerlexikon/mathematik-abitur/artikel/taylor-entwicklung-einiger-trigonometrischer-funktionen

#include <cmath>
#include <iostream>

// Potenzfunktion mit Rekursion
double power(const double base, const double exponent) {
    if (exponent == 0) return 1.0;
    return base * power(base, exponent - 1);
}

// Fakultät mit Rekursion
double fakultaet(const double n) {
    if (n <= 1) return 1;
    return n * fakultaet(n - 1);
}

// Rekursive Taylor-Approximation für sin(x)
double taylor_sin(const double x, const int n = 10) {
    if (n == 0) {
        return x; // erster Term: x^1 / 1!
    }
    // aktueller Term: (-1)^n * x^(2n+1) / (2n+1)!
    const double term = power(-1, n) * power(x, 2 * n + 1) / fakultaet(2 * n + 1);
    return term + taylor_sin(x, n - 1);
}

// Rekursive Taylor-Approximation für cos(x)
double taylor_cos(const double x, const int n = 10) {
    if (n == 0) {
        return 1; // erster Term: x^0 / 0! = 1
    }
    // aktueller Term: (-1)^n * x^(2n) / (2n)!
    const double term = power(-1, n) * power(x, 2 * n) / fakultaet(2 * n);
    return term + taylor_cos(x, n - 1);
}

// Tangens (aus Sinus/Cosinus)
double taylor_tan(const double x, const int n = 10) {
    // Berechnung von sin(x) und cos(x)
    const double sinx = taylor_sin(x, n);
    const double cosx = taylor_cos(x, n);

    // Überprüfung, ob cos(x) nahe 0 ist
    if (std::abs(cosx) < 1e-9) {
        return NAN;
    }

    // Tangens als sin(x) / cos(x)
    return sinx / cosx;
}

// Kotangens (aus Sinus/Cosinus)
double taylor_cot(const double x, const int n = 10) {
    // Berechnung von sin(x) und cos(x)
    const double sinx = taylor_sin(x, n);
    const double cosx = taylor_cos(x, n);

    // Überprüfung, ob sin(x) nahe 0 ist
    if (std::abs(sinx) < 1e-9) {
        return NAN;
    }

    // Kotangens als cos(x) / sin(x)
    return cosx / sinx;
}

double sqrt(const double x) {
    //Wenn x negativ ist, wird NaN zurückgegeben
    if (x < 0) return NAN;

    double t = x;
    int iterations = 0;

    // Beendet schleife, wenn 100 Iterationen erreicht sind oder die Änderung sehr klein ist
    while (iterations < 100 && t > 1e-9) {
        t = 0.5 * (t + x / t);
        iterations++;
    }
    return t;
}

int main() {
    double x;
    int n;

    std::cout << "Geben Sie den Wert fuer x ein: ";
    std::cin >> x;

    std::cout << "Geben Sie die Anzahl der Terme n ein: ";
    std::cin >> n;

    std::cout << "\n sin(" << x << ") = " << taylor_sin(x, n)
            << "\n cos(" << x << ") = " << taylor_cos(x, n)
            << "\n tan(" << x << ") = " << taylor_tan(x, n)
            << "\n cot(" << x << ") = " << taylor_cot(x, n) << std::endl;

    std::cout << "\n sqrt(" << x << ") = " << sqrt(x) << std::endl;
}
