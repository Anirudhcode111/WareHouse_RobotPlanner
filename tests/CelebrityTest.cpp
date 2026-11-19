#include "algorithms/divide_conquer/Celebrity.hpp"

#include <cassert>
#include <iostream>
#include <vector>

using warehouse::algorithms::findCelebrity;

int main() {
    // Celebrity is person 2
    std::vector<std::vector<int>> case1 = {
        {0, 1, 1},
        {0, 0, 1},
        {0, 0, 0}
    };
    assert(findCelebrity(case1) == 2);

    // No celebrity
    std::vector<std::vector<int>> case2 = {
        {0, 1, 0},
        {0, 0, 1},
        {1, 0, 0}
    };
    assert(findCelebrity(case2) == -1);

    // Single person
    std::vector<std::vector<int>> case3 = {
        {0}
    };
    assert(findCelebrity(case3) == 0);

    // Empty matrix
    std::vector<std::vector<int>> case4;
    assert(findCelebrity(case4) == -1);

    // Celebrity is person 0
    std::vector<std::vector<int>> case5 = {
        {0, 0, 0},
        {1, 0, 1},
        {1, 1, 0}
    };
    assert(findCelebrity(case5) == 0);

    // Invalid non-square matrix
    std::vector<std::vector<int>> case6 = {
        {0, 1},
        {1}
    };
    assert(findCelebrity(case6) == -1);

    std::cout << "All Celebrity Problem tests passed!\n";

    return 0;
}