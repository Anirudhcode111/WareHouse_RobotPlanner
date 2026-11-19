#pragma once

#include <string>
#include <vector>

namespace warehouse::algorithms {

struct KnapsackItem {
    std::string id;
    int weight;
    long long value;
};

struct KnapsackResult {
    long long maximumValue = 0;
    int totalWeight = 0;
    std::vector<std::string> selectedItems;
};

KnapsackResult solveKnapsack(
    const std::vector<KnapsackItem>& items,
    int capacity
);

}