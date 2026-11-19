#include "algorithms/divide_conquer/Celebrity.hpp"

namespace warehouse::algorithms {

int findCelebrity(const std::vector<std::vector<int>>& knows) {
    int n = static_cast<int>(knows.size());

    if (n == 0) {
        return -1;
    }

    // Validate that the matrix is square.
    for (const auto& row : knows) {
        if (static_cast<int>(row.size()) != n) {
            return -1;
        }
    }

    // Step 1: Find a possible celebrity.
    int candidate = 0;

    for (int i = 1; i < n; ++i) {
        if (knows[candidate][i] == 1) {
            candidate = i;
        }
    }

    // Step 2: Verify the candidate.
    for (int i = 0; i < n; ++i) {
        if (i == candidate) {
            continue;
        }

        if (knows[candidate][i] == 1 || knows[i][candidate] != 1) {
            return -1;
        }
    }

    return candidate;
}

}