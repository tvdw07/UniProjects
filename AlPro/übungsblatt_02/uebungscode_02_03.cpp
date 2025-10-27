// Zur Blatt 02 Aufgabe 03

#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <ctime>

// Druckt einen Text aus
void drucke_text(const std::string& text);

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

int get_current_year() {
  // Holen des aktuellen Jahres
  time_t t = time(nullptr);

  // Umwandeln in lokale Zeitstruktur
  tm* timePtr = localtime(&t);

  // Extrahieren des Jahres (tm_year gibt Jahre seit 1900 zurück)
  return timePtr->tm_year + 1900;
}


void alter_von_alan_turing_erraten() {
  // constexpr weil IDE mit const nicht zufrieden ist
  constexpr int geburtsjahr_alan_turing = 1912;
  const int current_year = get_current_year();
  const int alter_alan_turing = current_year - geburtsjahr_alan_turing;

  bool korrekt_geraten = false;
  while (!korrekt_geraten) {
    drucke_text("Bitte raten Sie, wie alt Alan Turing im Jahr " + std::to_string(current_year) + " geworden waere:");
    int benutzer_eingabe = lies_ganzzahl();

    // Überprüfen der Benutzereingabe
    if (benutzer_eingabe < alter_alan_turing) {
      drucke_text("Falsch! Alan Turing waere aelter gewesen. Versuchen Sie es erneut.");
    } else if (benutzer_eingabe > alter_alan_turing) {
      drucke_text("Falsch! Alan Turing waere juenger gewesen. Versuchen Sie es erneut.");
    } else {
      drucke_text("Korrekt! Alan Turing waere im Jahr " + std::to_string(current_year) + " " + std::to_string(alter_alan_turing) + " Jahre alt geworden.");

      korrekt_geraten = true;
    }
  }
  
}





int main() {
  alter_von_alan_turing_erraten();
}


/************************************/
/*                                  */
/*   MAGIE   MAGIE   MAGIE  MAGIE   */
/*                                  */
/************************************/

void drucke_text(const std::string& text) {
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
  } catch (const std::invalid_argument& e) {
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
  } catch (const std::invalid_argument& e) {
    lies_gleitkommazahl = NAN; // See https://en.cppreference.com/w/cpp/numeric/math/NAN
  }
  return lies_gleitkommazahl;
}
