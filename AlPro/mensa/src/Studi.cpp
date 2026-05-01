#include "Studi.hpp"

Studi::Studi(const std::string &v, const std::string &n, const Geburtstagsdatum &g, double h)
    : vorname(v), nachname(n), geburtstag(g), koerperhoehe(h), matrikelnummer(0) {
}

//Getter
const std::string &Studi::getVorname() const { return vorname; }
const std::string &Studi::getNachname() const { return nachname; }
const Geburtstagsdatum &Studi::getGeburtstag() const { return geburtstag; }
double Studi::getKoerperhoehe() const { return koerperhoehe; }
const std::set<std::string> &Studi::getInteressen() const { return interessen; }
const int Studi::getMatrikelnummer() const { return matrikelnummer; }

//Setter
void Studi::setVorname(const std::string &v) { if (!v.empty()) vorname = v; }
void Studi::setNachname(const std::string &n) { if (!n.empty()) nachname = n; }

void Studi::setGeburtstag(const Geburtstagsdatum &g) {
    if (g.monat >= 1 && g.monat <= 12 && g.tag >= 1 && g.tag <= 31)
        geburtstag = g;
}

void Studi::setKoerperhoehe(const double h) { if (h > 0) koerperhoehe = h; }
void Studi::setMatrikelnummer(int m) { if (m > 0) matrikelnummer = m; }

//Interessen
void Studi::interesseHinzufuegen(const std::string &i) { interessen.insert(i); }
bool Studi::interessePruefen(const std::string &i) const { return interessen.contains(i); }
void Studi::interesseEntfernen(const std::string &i) { interessen.erase(i); }
void Studi::alleInteressenEntfernen() { interessen.clear(); }

//Drucken
void Studi::drucken() const {
    std::cout << "Matrikelnummer: " << matrikelnummer << ", " << vorname << " " << nachname << " (" << geburtstag.tag << "." << geburtstag.monat << "." << geburtstag.jahr << "), " << koerperhoehe << " m, Interessen: ";
    for (const auto &i: interessen) std::cout << i << " ";
    std::cout << "\n";
}

//Vergleichsoperatoren
bool Studi::operator<(const Studi &other) const {
    return matrikelnummer < other.matrikelnummer;
}

bool Studi::operator>(const Studi &other) const {
    return matrikelnummer > other.matrikelnummer;
}

bool Studi::operator==(const Studi &other) const {
    return matrikelnummer == other.matrikelnummer;
}

bool Studi::operator!=(const Studi &other) const {
    return matrikelnummer != other.matrikelnummer;
}

