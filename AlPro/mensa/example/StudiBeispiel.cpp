#include <iostream>
#include <set>
#include <map>
#include "../include/Studi.hpp"
#include "../include/StudiCmp.hpp"

using Studimenge = std::set<Studi, StudiCmp>;
using Mensaabbildung = std::map<Studi, Studimenge, StudiCmp>;

int main() {

    //Menge
    Studi alex("Alex", "Mueller", Geburtstagsdatum(2000,1,10), 1.75);
    alex.setMatrikelnummer(100001);

    Studi tobi("Tobi", "Schmidt", Geburtstagsdatum(2000,2,15), 1.80);
    tobi.setMatrikelnummer(100002);

    Studi pascal("Pascal", "Weber", Geburtstagsdatum(2000,3,20), 1.78);
    pascal.setMatrikelnummer(100003);

    Studimenge menge{alex, tobi, pascal};

    std::cout << "Studimenge:\n";
    for (const auto& s : menge)
        std::cout << s.getVorname() << " (" << s.getMatrikelnummer() << ")\n";


    //Mensaabbildung
    Studi lisa("Lisa","Fischer",Geburtstagsdatum(2000,4,25),1.65);
    lisa.setMatrikelnummer(100004);

    Studi frauke("Frauke","Schneider",Geburtstagsdatum(2000,5,30),1.70);
    frauke.setMatrikelnummer(100005);

    Studi fatma("Fatma","Kaya",Geburtstagsdatum(2000,6,5),1.68);
    fatma.setMatrikelnummer(100006);

    Mensaabbildung mensa;

    mensa[alex]   = {tobi, pascal, lisa};
    mensa[frauke] = {tobi, alex, fatma};
    mensa[lisa]   = {frauke, alex};

    std::cout << "\nMensa-Beziehungen:\n";

    // Ausgabe der Mensaabbildung
    for (const auto& [studi, gruppe] : mensa) {
        std::cout << studi.getVorname() << " geht mit: ";
        for (const auto& g : gruppe)
            std::cout << g.getVorname() << " ";
        std::cout << "\n";
    }

    return 0;
}
