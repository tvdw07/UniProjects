//
// Created by Tim on 18.11.2025.
//

#include <stdlib.h>
#include <iostream>
#include "mergesort.h"
#include <vector>

int main() {
    // Generate a random array of 10 integers
    std::vector<int> arr = generateRandomArray(10);

    // Print the unsorted array
    std::cout << "Unsortiertes Array: ";
    printArray(arr);

    // Sort the array using Mergesort
    mergeSort(arr);

    // Print the sorted array
    std::cout << "Sortiertes Array:   ";
    printArray(arr);

    return EXIT_SUCCESS;
}

