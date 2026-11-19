
#include "algorithms/greedy/ActivitySelection.hpp"

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <vector>

using namespace warehouse::algorithms;

int main() {
    // Normal case
    std::vector<Activity> activities = {
        {1, 1, 3},
        {2, 2, 5},
        {3, 4, 6},
        {4, 6, 8},
        {5, 5, 9}
    };

    auto selected = activitySelection(activities);

    assert(selected.size() == 3);
    assert(selected[0].id == 1);
    assert(selected[1].id == 3);
    assert(selected[2].id == 4);

    // Empty input
    assert(activitySelection({}).empty());

    // All overlapping
    std::vector<Activity> overlapping = {
        {1, 1, 10},
        {2, 2, 9},
        {3, 3, 8}
    };

    selected = activitySelection(overlapping);
    assert(selected.size() == 1);

    // All compatible
    std::vector<Activity> compatible = {
        {1, 1, 2},
        {2, 2, 3},
        {3, 3, 4}
    };

    selected = activitySelection(compatible);
    assert(selected.size() == 3);

    // Single activity
    selected = activitySelection({{1, 5, 10}});
    assert(selected.size() == 1);
    assert(selected[0].id == 1);

    // Invalid interval
    bool exceptionThrown = false;

    try {
        activitySelection({{1, 10, 5}});
    } catch (const std::invalid_argument&) {
        exceptionThrown = true;
    }

    assert(exceptionThrown);

    std::cout << "All Activity Selection tests passed!\n";

    return 0;
}
