#include "database.h"

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    #include "database_functions.h"


    // Funktion zum Schreiben eines Datenbankeintrags
    // Allokiert Speicher für die Strings und kopiert die übergebenen Werte
    void schreibe_eintrag(
      datenbank_eintrag* eintrag_ptr,
      char* vorname,
      char* nachname,
      char* matrikelnummer)
    {
      // Anmerkung: p->x ist eine Abkuerzung von (*(p)).x

      // Speicher allokieren (strlen+1 für das Null-Terminierungszeichen)
      eintrag_ptr->vorname       = malloc(strlen(vorname)+1);
      eintrag_ptr->nachname      = malloc(strlen(nachname)+1);
      eintrag_ptr->matrikelnummer = malloc(strlen(matrikelnummer)+1);

      // Strings in den allokierten Speicher kopieren
      strcpy(eintrag_ptr->vorname,       vorname);
      strcpy(eintrag_ptr->nachname,      nachname);
      strcpy(eintrag_ptr->matrikelnummer, matrikelnummer);
    }


    // Funktion zum Ausgeben aller Datenbankeinträge
    void drucke_datenbank(
      const datenbank_eintrag* datenbank,
      int32_t anz_eintraege)
    {
      // Anmerkung: p->x ist eine Abkuerzung von (*(p)).x

      // Durch alle Einträge iterieren
      while (anz_eintraege > 0) {
        // Einen Eintrag ausgeben
        printf("%s, %s: %s\n", datenbank->vorname, datenbank->nachname, datenbank->matrikelnummer);

        // Zum nächsten Eintrag weitergehen
        datenbank++;
        --anz_eintraege;
      }
    }

    // Funktion zum Freigeben des Speichers aller Datenbankeinträge
    void loesche_datenbank(datenbank_eintrag* datenbank, int32_t anz_eintraege) {
      // Durch alle Einträge iterieren
      for (int32_t i = 0; i < anz_eintraege; i++) {
        // Speicher für jeden String freigeben
        free(datenbank[i].vorname);
        free(datenbank[i].nachname);
        free(datenbank[i].matrikelnummer);
      }
    }


    int main()
    {
      // Array für bis zu 100 Datenbankeinträge
      datenbank_eintrag datenbank[100];
      int32_t datenbank_eintraege = 0;

      // 5 Einträge in die Datenbank schreiben
      schreibe_eintrag(datenbank+0, "Ada",    "Lovelace", "3819921");
      schreibe_eintrag(datenbank+1, "Alan",   "Turing",   "2898805");
      schreibe_eintrag(datenbank+2, "Ingo",   "Mueller",  "2413158");
      schreibe_eintrag(datenbank+3, "Ilse",   "Marquardt",  "2031691");
      schreibe_eintrag(datenbank+4, "Stefan", "Sadat",    "3305217");

      datenbank_eintraege = 5;

      // Unsortierte Datenbank ausgeben
      printf("Unsortiert:\n");
      drucke_datenbank(datenbank, datenbank_eintraege);

      // Aufsteigend nach Nachname sortieren und ausgeben
      qsort(datenbank, datenbank_eintraege, sizeof(datenbank_eintrag), sort_aufsteigend);
      printf("\nAufsteigend sortiert:\n");
      drucke_datenbank(datenbank, datenbank_eintraege);

      // Absteigend nach Nachname sortieren und ausgeben
      qsort(datenbank, datenbank_eintraege, sizeof(datenbank_eintrag), sort_absteigend);
      printf("\nAbsteigend sortiert:\n");
      drucke_datenbank(datenbank, datenbank_eintraege);

      // Speicher freigeben
      loesche_datenbank(datenbank, datenbank_eintraege);
    }