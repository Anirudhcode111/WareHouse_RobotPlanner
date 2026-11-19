#include "algorithms/divide_conquer/MaximumSubarray.hpp"

#include <cassert>
#include <iostream>
#include <vector>

using warehouse::algorithms::maximumSubarraySum;

int main() {
    // Normal case
    assert(maximumSubarraySum({-2, 1, -3, 4, -1, 2, 1, -5, 4}) == 6);

    // All positive
    assert(maximumSubarraySum({1, 2, 3, 4}) == 10);

    // All negative
    assert(maximumSubarraySum({-5, -2, -8, -1}) == -1);

    // Single element
    assert(maximumSubarraySum({7}) == 7);

    // Empty array
    assert(maximumSubarraySum({}) == 0);

    // Mixed positive and negative
    assert(maximumSubarraySum({-2, -3, 4, -1, -2, 1, 5, -3}) == 7);

    // Zero values
    assert(maximumSubarraySum({0, 0, 0}) == 0);

    std::cout << "All Maximum Subarray tests passed!\n";

    return 0;
}