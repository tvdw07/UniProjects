#include <stdio.h>

void drucke_ganzzahlen (int zahlen [], int anzahl_zahlen ) {
    for (int i = 0; i < anzahl_zahlen; i++) {
        printf("%d, ", zahlen[i]);
    }
    printf("\n");
}

int main() {
    int zahlen[] = {1,2,3,4,5,6};
    drucke_ganzzahlen(zahlen, 6);
}