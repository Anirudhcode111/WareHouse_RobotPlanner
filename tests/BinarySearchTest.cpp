
#include "algorithms/sorting/BinarySearch.hpp"

#include <cassert>
#include <iostream>
#include <vector>

using warehouse::algorithms::binarySearchIterative;

int main() {
    std::vector<int> data = {1, 3, 5, 7, 9, 11};

    assert(binarySearchIterative(data, 7) == 3);
    assert(binarySearchIterative(data, 1) == 0);
    assert(binarySearchIterative(data, 11) == 5);
    assert(binarySearchIterative(data, 6) == -1);

    std::vector<int> duplicates = {1, 2, 2, 2, 4, 5};
    assert(binarySearchIterative(duplicates, 2) == 1);

    std::vector<int> empty;
    assert(binarySearchIterative(empty, 5) == -1);

    std::vector<int> negative = {-9, -5, -2, 0, 4};
    assert(binarySearchIterative(negative, -5) == 1);

    std::cout << "All Iterative Binary Search tests passed!\n";
    return 0;
}
