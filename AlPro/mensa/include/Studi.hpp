#pragma once
#include <string>
#include <set>
#include <iostream>
#include "../../uebungscode_11/include/Geburtstagsdatum.hpp"

class Studi {
private:
    std::string vorname;
    std::string nachname;
    Geburtstagsdatum geburtstag;
    double koerperhoehe;
    std::set<std::string> interessen; // Stringmenge

    int matrikelnummer;

public:
    // Konstruktor
    Studi(const std::string &v = "", const std::string &n = "",
          const Geburtstagsdatum &g = Geburtstagsdatum(), double h = 0.0);

    // --- Getter ---
    const std::string &getVorname() const;
    const std::string &getNachname() const;
    const Geburtstagsdatum &getGeburtstag() const;
    double getKoerperhoehe() const;
    const std::set<std::string> &getInteressen() const;
    const int getMatrikelnummer() const;

    // --- Setter ---
    void setVorname(const std::string &v);
    void setNachname(const std::string &n);
    void setGeburtstag(const Geburtstagsdatum &g);
    void setKoerperhoehe(double h);
    void setMatrikelnummer(int m);

    // --- Interessen ---
    void interesseHinzufuegen(const std::string &i);
    bool interessePruefen(const std::string &i) const;
    void interesseEntfernen(const std::string &i);
    void alleInteressenEntfernen();

    // --- Drucken ---
    void drucken() const;

    // --- Vergleichsoperatoren ---
    bool operator <(const Studi &other) const;
    bool operator >(const Studi &other) const;
    bool operator==(const Studi &other) const;
    bool operator!=(const Studi &other) const;
};
