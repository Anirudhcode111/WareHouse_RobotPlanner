
#include "algorithms/BranchAndBoundPlanner.hpp"
#include "algorithms/Dijkstra.hpp"

#include <algorithm>
#include <cmath>
#include <functional>
#include <limits>
#include <unordered_map>
#include <unordered_set>

namespace warehouse {

TaskPlan BranchAndBoundPlanner::planExact(
    const Warehouse& warehouse,
    const Graph& graph,
    const std::string& robotId,
    const std::vector<std::string>& taskIds
) {
    TaskPlan result;

    const auto robotIt = std::find_if(
        warehouse.robots.begin(),
        warehouse.robots.end(),
        [&](const Robot& robot) {
            return robot.id == robotId;
        }
    );

    if (robotIt == warehouse.robots.end()) {
        result.message = "Robot not found: " + robotId;
        return result;
    }

    const Robot& robot = *robotIt;

    if (robot.capacityKg < 0.0 ||
        !std::isfinite(robot.capacityKg)) {
        result.message = "Robot capacity is invalid.";
        return result;
    }

    // Reject duplicate task IDs and invalid task requests.
    std::unordered_set<std::string> requestedIds;
    std::unordered_map<std::string, const Task*> taskById;

    for (const auto& task : warehouse.tasks) {
        taskById[task.id] = &task;
    }

    std::vector<const Task*> selectedTasks;

    for (const auto& id : taskIds) {
        if (!requestedIds.insert(id).second) {
            result.message = "Duplicate task ID: " + id;
            return result;
        }

        const auto it = taskById.find(id);

        if (it == taskById.end()) {
            result.message = "Task not found: " + id;
            return result;
        }

        const Task* task = it->second;

        if (task->status != TaskStatus::Pending) {
            result.message = "Task is not pending: " + id;
            return result;
        }

        if (task->weightKg < 0.0 ||
            !std::isfinite(task->weightKg)) {
            result.message = "Invalid task weight: " + id;
            return result;
        }

        selectedTasks.push_back(task);
    }

    // The exact ordering search is intentionally limited to small
    // task sets because the search space grows factorially.
    constexpr std::size_t MAX_EXACT_TASKS = 10;

    if (selectedTasks.size() > MAX_EXACT_TASKS) {
        result.message =
            "Exact planner supports at most 10 tasks per search.";
        return result;
    }

    double totalWeight = 0.0;

    for (const Task* task : selectedTasks) {
        totalWeight += task->weightKg;
    }

    if (totalWeight > robot.capacityKg) {
        result.message =
            "Selected tasks exceed robot carrying capacity.";
        return result;
    }

    // Cache shortest-path results between relevant locations.
    std::vector<std::string> locations;
    locations.push_back(robot.currentLocationId);

    for (const Task* task : selectedTasks) {
        locations.push_back(task->pickupLocationId);
    }

    const std::size_t n = selectedTasks.size();

    std::vector<std::vector<double>> distances(
        n + 1,
        std::vector<double>(n, 
            std::numeric_limits<double>::infinity())
    );

    std::vector<std::vector<std::vector<std::string>>> paths(
        n + 1,
        std::vector<std::vector<std::string>>(n)
    );

    for (std::size_t from = 0; from <= n; ++from) {
        for (std::size_t to = 0; to < n; ++to) {
            if (from == to + 1) {
                distances[from][to] = 0.0;
                paths[from][to] = {locations[from]};
                continue;
            }

            const PathResult path = Dijkstra::shortestPath(
                graph, locations[from], locations[to + 1]
            );

            if (path.reachable) {
                distances[from][to] = path.totalDistance;
                paths[from][to] = path.path;
            }
        }
    }

    // All tasks must be reachable from the robot's starting point.
    for (std::size_t i = 0; i < n; ++i) {
        if (!std::isfinite(distances[0][i])) {
            result.message =
                "At least one selected task is unreachable: " +
                selectedTasks[i]->id;
            return result;
        }
    }

    if (n == 0) {
        result.feasible = true;
        result.message = "No tasks requested; empty plan is valid.";
        result.visitedLocations.push_back(robot.currentLocationId);
        return result;
    }

    double bestDistance = std::numeric_limits<double>::infinity();
    std::vector<std::size_t> bestOrder;
    std::vector<std::size_t> currentOrder;
    std::vector<bool> used(n, false);

    // Branch and bound:
    // prune any partial route whose cost is already no better
    // than the best complete route found so far.
    std::function<void(std::size_t, double)> search =
        [&](std::size_t currentIndex, double currentDistance) {
            if (currentDistance >= bestDistance) {
                return;
            }

            if (currentOrder.size() == n) {
                bestDistance = currentDistance;
                bestOrder = currentOrder;
                return;
            }

            std::vector<std::size_t> candidates;

            for (std::size_t i = 0; i < n; ++i) {
                if (!used[i] && std::isfinite(distances[currentIndex][i])) {
                    candidates.push_back(i);
                }
            }

            // Explore shorter next legs first to find a good bound early.
            std::sort(
                candidates.begin(),
                candidates.end(),
                [&](std::size_t a, std::size_t b) {
                    if (distances[currentIndex][a] !=
                        distances[currentIndex][b]) {
                        return distances[currentIndex][a] <
                               distances[currentIndex][b];
                    }

                    return selectedTasks[a]->id <
                           selectedTasks[b]->id;
                }
            );

            for (const std::size_t next : candidates) {
                const double nextDistance =
                    currentDistance + distances[currentIndex][next];

                if (nextDistance >= bestDistance) {
                    continue;
                }

                used[next] = true;
                currentOrder.push_back(next);

                search(next + 1, nextDistance);

                currentOrder.pop_back();
                used[next] = false;
            }
        };

    // Search starts at index 0 (the robot's current location).
    search(0, 0.0);

    if (bestOrder.empty()) {
        result.message =
            "No complete ordering exists for the selected tasks.";
        return result;
    }

    result.feasible = true;
    result.totalDistance = bestDistance;
    result.totalTaskWeightKg = totalWeight;

    std::string currentLocation = robot.currentLocationId;
    result.visitedLocations.push_back(currentLocation);

    for (const std::size_t taskIndex : bestOrder) {
        const Task* task = selectedTasks[taskIndex];

        result.taskIds.push_back(task->id);

        const PathResult path = Dijkstra::shortestPath(
            graph,
            currentLocation,
            task->pickupLocationId
        );

        for (std::size_t i = 1; i < path.path.size(); ++i) {
            result.visitedLocations.push_back(path.path[i]);
        }

        currentLocation = task->pickupLocationId;
    }

    result.message =
        "Exact minimum-distance ordering found for selected tasks.";

    return result;
}

} // namespace warehouse
