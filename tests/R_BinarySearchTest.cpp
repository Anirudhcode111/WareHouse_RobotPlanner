
#include "algorithms/sorting/BinarySearch.hpp"

#include <cassert>
#include <iostream>
#include <vector>

using warehouse::algorithms::binarySearchRecursive;

int main() {
    std::vector<int> data = {1, 3, 5, 7, 9, 11};

    // Basic recursive search tests
    assert(binarySearchRecursive(data, 7) == 3);
    assert(binarySearchRecursive(data, 1) == 0);
    assert(binarySearchRecursive(data, 11) == 5);
    assert(binarySearchRecursive(data, 6) == -1);

    // Duplicate values: first occurrence
    std::vector<int> duplicates = {1, 2, 2, 2, 4, 5};
    assert(binarySearchRecursive(duplicates, 2) == 1);

    // Empty array
    std::vector<int> empty;
    assert(binarySearchRecursive(empty, 5) == -1);

    // Negative values
    std::vector<int> negative = {-9, -5, -2, 0, 4};
    assert(binarySearchRecursive(negative, -5) == 1);

    std::cout << "All Recursive Binary Search tests passed!\n";

    return 0;
}
