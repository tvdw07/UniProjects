#include <meine_funktionen.hpp>
#include <cmath>

// Rekursive Taylor-Approximation für sin(x) mit Abbruchbedingung
//@param x Der Winkel in Radiant
//@param n Der aktuelle Term-Index
//@param tol Die Toleranz für den Abbruch
double sinus_taylor_recursive(const double x, const int n, const double tol) {
    // Berechne den aktuellen Summanden
    double term = std::pow(-1, n) * std::pow(x, 2 * n + 1) / std::tgamma(2 * n + 2);

    // Abbruchbedingung, wenn der Term klein genug ist (aktueller term < Toleranz)
    if (std::fabs(term) < tol)
        return term;

    // Rekursiver Aufruf
    return term + sinus_taylor_recursive(x, n + 1, tol);
}


// Wrapper Funktion für Sinus mit Standardtoleranz
double sinus(const double x) {
    constexpr double default_toleranzwert = 1e-10;
    return sinus_taylor_recursive(x, 0, default_toleranzwert);
}

// Wrapper Funktion für Sinus mit benutzerdefinierter Toleranz
double sinus(const double x, const double toleranzwert) {
    return sinus_taylor_recursive(x, 0, toleranzwert);
}

// Kosinus wird als Sinus(x + π/2) berechnet
double kosinus(const double x) {
    constexpr double default_toleranzwert = 1e-10;
    return sinus_taylor_recursive(x + M_PI/2, 0, default_toleranzwert);
}

// Kosinus mit benutzerdefinierter Toleranz
double kosinus(const double x, const double toleranzwert) {
    return sinus_taylor_recursive(x + M_PI/2, 0, toleranzwert);
}

// Tangens als Sinus/Cosinus
double tangens(const double x) {
    return sinus(x) / kosinus(x);
}

// Tangens mit benutzerdefinierter Toleranz
double tangens(const double x, const double toleranzwert) {
    return sinus(x, toleranzwert) / kosinus(x, toleranzwert);
}

// Kotangens als Kosinus/Sinus
double kotangens(const double x) {
    return kosinus(x) / sinus(x);
}

// Kotangens mit benutzerdefinierter Toleranz
double kotangens(const double x, const double toleranzwert) {
    return kosinus(x, toleranzwert) / sinus(x, toleranzwert);
}

