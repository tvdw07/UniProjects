//
// Created by Tim on 17.11.2025.
//
#pragma once

#include <vector>

/**
 * @brief Liefert den Index des kleinsten Elements im Vektor ab einem Startindex.
 *
 * @param arr Konstante Referenz auf den zu durchsuchenden Vektor.
 * @param start Startindex (inklusive) im Vektor, ab dem gesucht wird.
 * @return Index des kleinsten Elements im Bereich [start, arr.size()).
 */
int get_index_of_min(const std::vector<int> &arr, size_t start);

/**
 * @brief Vertauscht die beiden übergebenen Ganzzahlen.
 *
 * @param a Referenz auf das erste Element.
 * @param b Referenz auf das zweite Element.
 */
void swapAB(int &a, int &b);

/**
 * @brief Sortiert den Vektor in-place aufsteigend mittels MinSort.
 *
 * @param arr Referenz auf den zu sortierenden Vektor; das Ergebnis liegt in-place in arr.
 */
void minSort(std::vector<int> &arr);

/**
 * @brief Erzeugt einen Vektor mit zufälligen Ganzzahlen.
 *
 * @param size Gewünschte Länge des zurückgegebenen Vektors.
 * @return Vektor mit zufälligen int-Werten der Länge \p size.
 */
std::vector<int> generateRandomArray(int size);

/**
 * @brief Gibt den Inhalt des Vektors auf der Standardausgabe aus.
 *
 * @param arr Konstante Referenz auf den auszugebenden Vektor.
 */
void printArray(const std::vector<int> &arr);