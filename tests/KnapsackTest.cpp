#include "algorithms/dp/Knapsack.hpp"

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <vector>

using namespace warehouse::algorithms;

int main() {

    // Test 1: Standard Knapsack problem.
    {
        std::vector<KnapsackItem> items = {
            {"A", 10, 60},
            {"B", 20, 100},
            {"C", 30, 120}
        };

        auto result = solveKnapsack(items, 50);

        assert(result.maximumValue == 220);
        assert(result.totalWeight == 50);
        assert(result.selectedItems.size() == 2);
    }

    // Test 2: Zero capacity.
    {
        std::vector<KnapsackItem> items = {
            {"A", 5, 50}
        };

        auto result = solveKnapsack(items, 0);

        assert(result.maximumValue == 0);
        assert(result.selectedItems.empty());
    }

    // Test 3: Empty item list.
    {
        auto result = solveKnapsack({}, 20);

        assert(result.maximumValue == 0);
        assert(result.totalWeight == 0);
        assert(result.selectedItems.empty());
    }

    // Test 4: Item heavier than capacity.
    {
        std::vector<KnapsackItem> items = {
            {"A", 15, 100}
        };

        auto result = solveKnapsack(items, 10);

        assert(result.maximumValue == 0);
        assert(result.selectedItems.empty());
    }

    // Test 5: Zero-weight item.
    {
        std::vector<KnapsackItem> items = {
            {"A", 0, 50},
            {"B", 5, 40}
        };

        auto result = solveKnapsack(items, 5);

        assert(result.maximumValue == 90);
        assert(result.totalWeight == 5);
        assert(result.selectedItems.size() == 2);
    }

    // Test 6: Negative capacity.
    {
        bool exceptionThrown = false;

        try {
            solveKnapsack({}, -5);
        }
        catch (const std::invalid_argument&) {
            exceptionThrown = true;
        }

        assert(exceptionThrown);
    }

    // Test 7: Negative item weight.
    {
        bool exceptionThrown = false;

        try {
            solveKnapsack({{"A", -2, 50}}, 10);
        }
        catch (const std::invalid_argument&) {
            exceptionThrown = true;
        }

        assert(exceptionThrown);
    }

    // Test 8: Negative item value.
    {
        bool exceptionThrown = false;

        try {
            solveKnapsack({{"A", 2, -50}}, 10);
        }
        catch (const std::invalid_argument&) {
            exceptionThrown = true;
        }

        assert(exceptionThrown);
    }

    std::cout << "All Knapsack tests passed!\n";

    return 0;
}