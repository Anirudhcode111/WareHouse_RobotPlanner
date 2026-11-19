#include "algorithms/dp/Knapsack.hpp"

#include <stdexcept>
#include <vector>

namespace warehouse::algorithms {

KnapsackResult solveKnapsack(
    const std::vector<KnapsackItem>& items,
    int capacity
) {
    if (capacity < 0) {
        throw std::invalid_argument("Capacity cannot be negative.");
    }

    for (const auto& item : items) {
        if (item.weight < 0) {
            throw std::invalid_argument("Item weight cannot be negative.");
        }

        if (item.value < 0) {
            throw std::invalid_argument("Item value cannot be negative.");
        }
    }

    const int n = static_cast<int>(items.size());

    // DP table: rows represent items, columns represent capacities.
    std::vector<std::vector<long long>> dp(
        n + 1,
        std::vector<long long>(capacity + 1, 0)
    );

    // Build the DP table.
    for (int i = 1; i <= n; ++i) {
        const auto& item = items[i - 1];

        for (int w = 0; w <= capacity; ++w) {

            // Option 1: Exclude current item.
            dp[i][w] = dp[i - 1][w];

            // Option 2: Include current item.
            if (item.weight <= w) {
                const long long includeValue =
                    item.value + dp[i - 1][w - item.weight];

                if (includeValue > dp[i][w]) {
                    dp[i][w] = includeValue;
                }
            }
        }
    }

    // Reconstruct selected items.
    KnapsackResult result;
    result.maximumValue = dp[n][capacity];

    int remainingCapacity = capacity;

    for (int i = n; i >= 1; --i) {
        if (dp[i][remainingCapacity] !=
            dp[i - 1][remainingCapacity]) {

            const auto& item = items[i - 1];

            result.selectedItems.push_back(item.id);
            result.totalWeight += item.weight;
            remainingCapacity -= item.weight;
        }
    }

    return result;
}

}