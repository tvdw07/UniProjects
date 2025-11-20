#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
  char* vorname;
  char* nachname;
  char* telefonnummer;
} telefonbucheintrag;


// Funktion erhält als Eingabe einen Pointer auf das Telefonbuch-Array sowie dessen Größe
// und schreibt dieses in CSV Format ("Vorname,Nachname,Telefonnummer")
// in eine Datei "telefonbuch.csv". Falls die Datei bereits existiert, wird diese überschrieben.
void csv_schreiber(const telefonbucheintrag* telefonbuch,const int size){
  FILE* datei = fopen("telefonbuch.csv", "w");
  if (datei == NULL) {
    printf("Fehler beim Öffnen der Datei zum Schreiben.\n");
    return;
  }

  for (int i = 0; i < size; i++) {
    fprintf(datei, "%s,%s,%s\n",
            telefonbuch[i].vorname,
            telefonbuch[i].nachname,
            telefonbuch[i].telefonnummer);
  }

  fclose(datei);
  printf("Telefonbuch wurde in 'telefonbuch.csv' gespeichert.\n");
}

// Funktion erhält als Eingabe einen Pointer auf das leere Telefonbuch-Array
// und liest aus einer Datei "telefonbuch.csv" Einträge in dieses ein. Gibt
// die Anzahl der gelesenen Einträge zurück. Falls die Datei nicht existiert,
// wird 0 zurückgegeben.
int csv_leser(telefonbucheintrag* telefonbuch){
  FILE* datei = fopen("telefonbuch.csv", "r");
  if (datei == NULL) {
    printf("Datei 'telefonbuch.csv' nicht gefunden. Es werden keine Einträge geladen.\n");
    return 0;
  }

  char buffer[256];
  int count = 0;

  while (fgets(buffer, sizeof(buffer), datei) != NULL && count < 100) {
    // Entferne Zeilenumbruch
    buffer[strcspn(buffer, "\n")] = 0;

    // Speicher allokieren
    telefonbuch[count].vorname = malloc(50);
    telefonbuch[count].nachname = malloc(50);
    telefonbuch[count].telefonnummer = malloc(50);

    // CSV-Zeile parsen (Format: Vorname,Nachname,Telefonnummer)
    char* token = strtok(buffer, ",");
    if (token) strcpy(telefonbuch[count].vorname, token);

    token = strtok(NULL, ",");
    if (token) strcpy(telefonbuch[count].nachname, token);

    token = strtok(NULL, ",");
    if (token) strcpy(telefonbuch[count].telefonnummer, token);

    count++;
  }

  fclose(datei);
  return count;
}

int main()
{
  telefonbucheintrag telefonbuch[100];

  // Lese Einträge aus CSV Datei, falls vorhanden
  int size = csv_leser(telefonbuch);
  if (size > 0) {
    printf("Es wurden %d Einträge aus der Datei 'telefonbuch.csv' gelesen.\n", size);
  } 

  // Füge neue Einträge hinzu
  if (size <= 0) {
    size_t buf_size = 20;

    for (int i = 0; i < 3; i++) {
      telefonbuch[i].vorname = malloc(buf_size);
      telefonbuch[i].nachname = malloc(buf_size);
      telefonbuch[i].telefonnummer = malloc(buf_size);
      
      // Einlesen der Daten
      printf("Vorname: ");
      fgets(telefonbuch[i].vorname, 20, stdin);
      printf("Nachname: ");
      fgets(telefonbuch[i].nachname, 20, stdin);
      printf("Telefonnummer: ");
      fgets(telefonbuch[i].telefonnummer, 20, stdin);
      
      // Entferne Zeilenumbruch
      telefonbuch[i].vorname[strcspn(telefonbuch[i].vorname, "\n")] = 0;
      telefonbuch[i].nachname[strcspn(telefonbuch[i].nachname, "\n")] = 0;
      telefonbuch[i].telefonnummer[strcspn(telefonbuch[i].telefonnummer, "\n")] = 0;
    }
    size = 3;
  }

  // Leerzeile für bessere Übersicht ausgeben
  printf("\n");

  // Telefonbuch ausgeben
  for (int i = 0; i < size; i++) {
    printf("Vorname: %s, Nachname: %s, Telefonnummer: %s\n", telefonbuch[i].vorname, telefonbuch[i].nachname, telefonbuch[i].telefonnummer);
  }

  // Telefonbuch in CSV Datei schreiben
  csv_schreiber(telefonbuch, size);

  // Speicher freigeben
  for (int i = 0; i < size; i++) {
    free(telefonbuch[i].vorname);
    free(telefonbuch[i].nachname);
    free(telefonbuch[i].telefonnummer);
  }

  return 0;
}
