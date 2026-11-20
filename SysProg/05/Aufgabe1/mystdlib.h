#ifndef MYSTDLIB_H
#define MYSTDLIB_H

#include <stddef.h>
#include "mystd-def.h"

/* Reserviert size Bytes aus dem statischen 1-MiB-Speicherbereich. */
void *mymalloc(size_t size);

/* Gibt einen zuvor mit mymalloc reservierten Block wieder frei. */
void myfree(void *ptr);

/* Reserviert count * size Bytes und setzt alle Bytes auf null. */
void *mycalloc(size_t count, size_t size);

/* Aendert die Groesse eines reservierten Blocks; NULL bedeutet Fehler/freigegeben. */
void *myrealloc(void *ptr, size_t size);

#endif
