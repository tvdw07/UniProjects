// Zur Blatt 02 Aufgabe 02

#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>


// Druckt einen Text aus
void drucke_text(const std::string &text);

// Druckt eine Ganzzahl aus
void drucke_ganzzahl(int ganzzahl);

// Druckt eine Kommazahl aus
void drucke_kommazahl(double gleitkommazahl);

// Liest ein Wort von der Konsole ein
std::string lies_wort();

// Liest eine Ganzzahl von der Konsole ein
int lies_ganzzahl();

// Liest eine Gleitkommazahl von der Konsole ein
double lies_gleitkommazahl();



void ablauf_diagramm_1() {
    drucke_text("Ablaufdiagramm 1 gestartet.");

    //Programm fragt nach einer Ganzzahl
    drucke_text("Bitte geben Sie eine Ganzzahl zum quadrieren ein:");
    int x = lies_ganzzahl();

    //Programm quadriert x und gibt das Ergebnis aus
    x *= x;
    drucke_ganzzahl(x);
}

void ablauf_diagramm_2() {
    drucke_text("Ablaufdiagramm 2 gestartet.");

    int y;
    //Programm empfängt eine Zahl >= 0
    while (true) {
        drucke_text("Bitte geben Sie eine Zahl >= 0 ein:");
        y = lies_ganzzahl();

        if (y >= 0) {
            break;
        }
        drucke_text("Fehler: Die Zahl ist kleiner als 0.");
    }

    //Programm subtrahiert 2 so oft wie möglich von y
    while (y > 1) {
        y = y - 2;
    }

    //Das ergebnis ist 1 bei ungerader Eingabe und 0 bei gerader Eingabe
    drucke_ganzzahl(y);

}

//Programm druckt 0 aus, wen x ungerade ist und 1, wenn x gerade ist
void programm_1() {
    drucke_text("Programm 1 gestartet.");

    // Programm empfängt eine Ganzzahl // Muss ich prüfen ob es eine Ganzzahl ist?
    drucke_text("Bitte geben Sie eine Ganzzahl ein:");
    int x = lies_ganzzahl();

    x = x + 17;
    x = x * 3;
    x = x % 2;

    drucke_ganzzahl(x);


}


void programm_2() {
    std::string text = "Das Modul Algorithmen und Programmierung ist so konzipert, dass die Konzepte und Methodiken\
der objektorientierten Programmierung im Vordergrund stehen. Die hier vorgestellte\
Programmiersprache C++ nimmt die Rolle einer Beispielprogrammiersprache ein, in der die\
diskutierten Konzepte (meist mehr, selten weniger) nachvollziehbar und effizient umgesetzt\
sind. Das Modul soll so verstanden werden, dass es den Teilnehmer:innen im Anschluss an\
dieses Modul leicht fällt andere, objektorientierte Programmiersprachen zu erlernen. Hier soll\
man sich nur noch die Frage stellen: Wie setzten andere Programmiersprachen die wesentlichen\
Konzepte um und was gibt es bei der Sprachsyntax zu beachten?";

    drucke_text("Programm 2 gestartet.");

    //Abfrage ob eigener Text eingegeben werden soll
    drucke_text("Druecke y wenn du selber einen Text eingeben moechtest, oder n um den vorgegebenen Text zu verwenden:");
    std::string eingabe = lies_wort();

    if (eingabe == "y") {
        drucke_text("Bitte gib deinen Text ein:");
        text = lies_wort();
    } else if (eingabe == "n") {
        drucke_text("Verwende vorgegebenen Text.");
    } else {
        drucke_text("Ungültige Eingabe. Verwende vorgegebenen Text.");
    }



    int s = 0;

    // Programm geht jeden Charakter im Text durch und zählt die Satzzeichen
    for (char c : text) {
        if (c == '.' or c == '!' or c == '?') {
            s++;
        }
    }

    // Programm gibt die Anzahl der Satzzeichen aus
    drucke_ganzzahl(s);


}


void programm_3() {
    std::vector<int> zahlen = {42, 69, 420, 99, -444, 9014};
    drucke_text("Programm 3 gestartet.");

    // Programm geht jede Zahl in der Liste durch und summiert die Zahlen
    int summe = 0;

    for (int i : zahlen) {
        summe += i;
    }

    // Programm berechnet den Durchschnitt der Zahlen und gibt diesen aus
    double average = summe / (double) zahlen.size();

    drucke_kommazahl(average);

}


void programm_4() {
    drucke_text("Programm 4 gestartet.");

    // Programm empfängt eine Ganzzahl
    drucke_text("Bitte geben Sie eine Ganzzahl ein:");
    int x = lies_ganzzahl();

    double result = pow(x, 218);



    drucke_kommazahl(result);

}


int main() {
    ablauf_diagramm_1();
    ablauf_diagramm_2();
    programm_1();
    programm_2();
    programm_3();
    programm_4();
}


/************************************/
/*                                  */
/*   MAGIE   MAGIE   MAGIE  MAGIE   */
/*                                  */
/************************************/

void drucke_text(const std::string &text) {
    std::cout << text << std::endl;
}

void drucke_ganzzahl(int ganzzahl) {
    std::cout << std::to_string(ganzzahl) << std::endl;
}

void drucke_kommazahl(double gleitkommazahl) {
    std::cout << std::to_string(gleitkommazahl) << std::endl;
}

std::string lies_wort() {
    std::string wort;
    std::cin >> wort;
    return wort;
}

int lies_ganzzahl() {
    std::string wort;
    std::cin >> wort;
    int ganzzahl = 0;
    try {
        ganzzahl = std::stoi(wort);
    } catch (const std::invalid_argument &e) {
        ganzzahl = -1000;
    }
    return ganzzahl;
}

double lies_gleitkommazahl() {
    std::string wort;
    std::cin >> wort;
    double lies_gleitkommazahl = 0.0;
    try {
        lies_gleitkommazahl = std::stod(wort);
    } catch (const std::invalid_argument &e) {
        lies_gleitkommazahl = NAN; // See https://en.cppreference.com/w/cpp/numeric/math/NAN
    }
    return lies_gleitkommazahl;
}

