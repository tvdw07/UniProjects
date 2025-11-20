#include "database_functions.h"
#include <string.h>
#include <stdlib.h>

int sort_absteigend(const void* eintrag_a, const void* eintrag_b){
    const datenbank_eintrag* a = (const datenbank_eintrag*)eintrag_a;
    const datenbank_eintrag* b = (const datenbank_eintrag*)eintrag_b;
    return strcmp(b->nachname, a->nachname);
}

int sort_aufsteigend(const void* eintrag_a, const void* eintrag_b){
    const datenbank_eintrag* a = (const datenbank_eintrag*)eintrag_a;
    const datenbank_eintrag* b = (const datenbank_eintrag*)eintrag_b;
    return strcmp(a->nachname, b->nachname);
}