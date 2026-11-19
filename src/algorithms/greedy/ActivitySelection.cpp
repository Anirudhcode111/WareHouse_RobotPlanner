#include "algorithms/greedy/ActivitySelection.hpp"

#include <algorithm>
#include <stdexcept>

namespace warehouse::algorithms {

std::vector<Activity> activitySelection(
    const std::vector<Activity>& activities
) {
    std::vector<Activity> sorted = activities;

    for (const auto& activity : sorted) {
        if (activity.start > activity.finish) {
            throw std::invalid_argument(
                "Activity start time cannot exceed finish time."
            );
        }
    }

    // Sort activities by finish time.
    std::sort(sorted.begin(), sorted.end(),
        [](const Activity& a, const Activity& b) {
            if (a.finish != b.finish)
                return a.finish < b.finish;

            return a.start < b.start;
        }
    );

    std::vector<Activity> selected;

    for (const auto& activity : sorted) {
        if (selected.empty() ||
            activity.start >= selected.back().finish) {

            selected.push_back(activity);
        }
    }

    return selected;
}

}