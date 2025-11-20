#pragma once

//
// Created by Tim on 18.11.2025.
//

#include <vector>

/**
 * @brief Gibt den Inhalt des Vektors auf der Standardausgabe aus.
 *
 * @param arr Konstante Referenz auf den auszugebenden Vektor.
 */
void printArray(const std::vector<int> &arr);

/**
 * @brief Erzeugt einen Vektor mit zufälligen Ganzzahlen.
 *
 * @param size Gewünschte Länge des zurückgegebenen Vektors.
 * @return Vektor mit zufälligen int-Werten der Länge size.
 */
std::vector<int> generateRandomArray(int size);

/**
 * @brief Sortiert den Vektor aufsteigend mittels Mergesort.
 *
 * @param arr Referenz auf den zu sortierenden Vektor.
 */
void mergeSort(std::vector<int> &arr);

/**
 * @brief Sortiert den Vektor rekursiv mittels Mergesort.
 *
 * @param arr Referenz auf den zu sortierenden Vektor.
 * @param left Startindex des zu sortierenden Bereichs.
 * @param right Endindex des zu sortierenden Bereichs (exklusive).
 */
void mergeSortRecursive(std::vector<int> &arr, std::size_t left, std::size_t right);

/**
 * @brief Führt zwei sortierte Teilarrays zusammen (Merge).
 *
 * @param arr Referenz auf den Vektor, der die Teilarrays enthält.
 * @param left Startindex des linken Teilarrays.
 * @param mid Endindex des linken Teilarrays / Startindex des rechten.
 * @param right Endindex des rechten Teilarrays (exklusive).
 */
void merge(std::vector<int> &arr, std::size_t left, std::size_t mid, std::size_t right);
