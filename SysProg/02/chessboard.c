#include <stdio.h>
#include <stdlib.h>
#include "chessboard.h"

int init_board(struct board_t* b, int n, int x, int y) {
    if (b == NULL || n <= 0 || x < 0 || y < 0 || x >= n || y >= n) {
        return 1; // Invalid parameters
    }

    b->n = n;
    b->x = x;
    b->y = y;
    b->count = 1;

    b->fields = malloc(n * sizeof(int*));

    if (b->fields == NULL) {
        return 2; // Fehler Speicherreservierung
    }

    for (int i = 0; i < n; i++) {

        b->fields[i] = calloc(n, sizeof(int));

        if (b->fields[i] == NULL) {
            // Fehler Speicherreservierung, bereits reservierten Speicher freigeben
            for (int j = 0; j < i; j++) {
                free(b->fields[j]);
            }

            free(b->fields);
            return 2; // Fehler Speicherreservierung
        }
    }

    //Startfeld als besucht makieren
    b->fields[y][x] = 1;

    return 0;
}

int isfree(struct board_t* b, int x, int y) {
    int new_x = b->x + x;
    int new_y = b->y + y;

    // Prüfen, ob die Zielposition innerhalb des Schachbretts liegt
    if (new_x < 0 || new_y < 0 || new_x >= b->n || new_y >= b->n) {
        return 0;
    }

    // Prüfen, ob das Zielfeld bereits besucht wurde
    if (b->fields[new_y][new_x] != 0) {
        return 0;
    }

    return 1;
}

void new_jump(struct board_t* b, int x, int y) {
    b->x += x;
    b->y += y;
    b->count++;
    b->fields[b->y][b->x] = b->count;

}

void remove_jump(struct board_t* b, int x, int y) {
    b->fields[b->y][b->x] = 0;
    b->x -= x;
    b->y -= y;
    b->count--;
}

int visited_fields(struct board_t* b) {
    return b->count;
}

void print_board(struct board_t* b) {
    for (int i = 0; i < b->n; i++) {

        // Horizontale Trennlinie
        for (int j = 0; j < b->n; j++) {
            printf("+---");
        }
        printf("+\n");

        // Feldwerte
        for (int j = 0; j < b->n; j++) {
            printf("+%3i", b->fields[i][j]);
        }
        printf("+\n");
    }

    // Untere Trennlinie
    for (int j = 0; j < b->n; j++) {
        printf("+---");
    }
    printf("+\n");
}

void free_board(struct board_t* b) {
    for (int i = 0; i < b->n; i++) {
        free(b->fields[i]);
    }

    free(b->fields);
    b->fields = NULL;
}
