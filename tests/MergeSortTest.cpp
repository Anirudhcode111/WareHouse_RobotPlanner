
#include "algorithms/sorting/MergeSort.hpp"

#include <cassert>
#include <iostream>
#include <vector>

using warehouse::algorithms::mergeSort;

void testCase(std::vector<int> input, const std::vector<int>& expected) {
    mergeSort(input);
    assert(input == expected);
}

int main() {
    testCase({5, 2, 8, 1, 3}, {1, 2, 3, 5, 8});
    testCase({1}, {1});
    testCase({}, {});
    testCase({9, 7, 5, 3, 1}, {1, 3, 5, 7, 9});
    testCase({1, 2, 3, 4, 5}, {1, 2, 3, 4, 5});
    testCase({4, 2, 4, 1, 2}, {1, 2, 2, 4, 4});
    testCase({-5, 3, -1, 0, 8}, {-5, -1, 0, 3, 8});

    std::cout << "All Merge Sort tests passed!\n";
    return 0;
}
