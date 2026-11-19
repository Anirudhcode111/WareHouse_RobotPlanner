
#include "algorithms/TaskPlanner.hpp"

#include "algorithms/BranchAndBoundPlanner.hpp"
#include "algorithms/Dijkstra.hpp"
#include "algorithms/dp/Knapsack.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <unordered_set>
#include <vector>

namespace warehouse {
namespace {

constexpr long double WEIGHT_SCALE = 1000.0L;
constexpr long double PRECISION_TOLERANCE = 1e-7L;
constexpr std::size_t MAX_DP_CELLS = 5'000'000;
constexpr std::size_t MAX_EXACT_TASKS = 10;

// Converts kilograms to integer grams without silently rounding.
bool kilogramsToGrams(double kilograms, int& grams) {
    if (!std::isfinite(kilograms) || kilograms < 0.0) {
        return false;
    }

    const long double scaled =
        static_cast<long double>(kilograms) * WEIGHT_SCALE;

    const long double rounded = std::round(scaled);

    if (std::fabs(scaled - rounded) > PRECISION_TOLERANCE) {
        return false;
    }

    if (rounded > std::numeric_limits<int>::max()) {
        return false;
    }

    grams = static_cast<int>(rounded);
    return true;
}

} // namespace

TaskPlan TaskPlanner::planOptimal(
    const Warehouse& warehouse,
    const Graph& graph,
    const std::string& robotId
) {
    TaskPlan failure;

    // 1. Find robot.
    const auto robotIt = std::find_if(
        warehouse.robots.begin(),
        warehouse.robots.end(),
        [&](const Robot& robot) {
            return robot.id == robotId;
        }
    );

    if (robotIt == warehouse.robots.end()) {
        failure.message = "Robot not found: " + robotId;
        return failure;
    }

    const Robot& robot = *robotIt;

    int capacityGrams = 0;

    if (!kilogramsToGrams(robot.capacityKg, capacityGrams)) {
        failure.message =
            "Robot capacity must be non-negative and representable "
            "to 0.001 kg precision.";
        return failure;
    }

    // 2. Validate task IDs and prepare reachable pending tasks.
    std::unordered_set<std::string> allTaskIds;
    std::vector<algorithms::KnapsackItem> items;

    for (const Task& task : warehouse.tasks) {
        if (task.id.empty() || !allTaskIds.insert(task.id).second) {
            failure.message =
                "Warehouse contains an empty or duplicate task ID.";
            return failure;
        }

        if (task.status != TaskStatus::Pending) {
            continue;
        }

        int weightGrams = 0;

        if (!kilogramsToGrams(task.weightKg, weightGrams)) {
            failure.message =
                "Task weight must be non-negative and representable "
                "to 0.001 kg precision: " + task.id;
            return failure;
        }

        if (task.priority < 0) {
            failure.message =
                "Task priority cannot be negative: " + task.id;
            return failure;
        }

        // Ignore tasks that cannot be reached from this robot.
        const PathResult path = Dijkstra::shortestPath(
            graph,
            robot.currentLocationId,
            task.pickupLocationId
        );

        if (!path.reachable) {
            continue;
        }

        items.push_back({
            task.id,
            weightGrams,
            static_cast<long long>(task.priority)
        });
    }

    // 3. Protect the 2D DP table from excessive memory allocation.
    const std::size_t rows = items.size() + 1;
    const std::size_t columns =
        static_cast<std::size_t>(capacityGrams) + 1;

    if (rows > MAX_DP_CELLS / columns) {
        failure.message =
            "Knapsack DP table exceeds the configured memory limit.";
        return failure;
    }

    // 4. Select maximum-priority tasks within robot capacity.
    const algorithms::KnapsackResult selection =
        algorithms::solveKnapsack(items, capacityGrams);

    // 5. Existing exact route planner supports at most 10 tasks.
    if (selection.selectedItems.size() > MAX_EXACT_TASKS) {
        failure.message =
            "Knapsack selected more than 10 tasks; exact route "
            "optimization currently supports at most 10.";
        return failure;
    }

    // 6. Optimize the visiting order for the selected task IDs.
    TaskPlan result = BranchAndBoundPlanner::planExact(
        warehouse,
        graph,
        robotId,
        selection.selectedItems
    );

    if (!result.feasible) {
        return result;
    }

    result.message =
        "Optimal task selection by Knapsack and minimum-distance "
        "ordering by Branch and Bound completed.";

    return result;
}

} // namespace warehouse
