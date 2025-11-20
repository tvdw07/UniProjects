#ifndef DATABASE
#define DATABASE

#include <stdint.h>

typedef struct {
  char* vorname;
  char* nachname;
  char* matrikelnummer;
} datenbank_eintrag;

void schreibe_eintrag(
  datenbank_eintrag* eintrag_ptr,
  char* vorname,
  char* nachname,
  char* matrikelnummer
);

void drucke_datenbank(
  const datenbank_eintrag* datenbank,
  int32_t anz_eintraege
);

#endif