//
// Created by Tim on 17.11.2025.
//

#include <stdlib.h>
#include "minsort_ip.h"
#include <vector>

int main() {
    // Generate a random array of 10 integers
    std::vector<int> arr = generateRandomArray(10);

    // Print the array
    printArray(arr);

    // Sort the array using in-place MinSort
    minSort(arr);

    // Print the sorted array
    printArray(arr);

    return EXIT_SUCCESS;
}
