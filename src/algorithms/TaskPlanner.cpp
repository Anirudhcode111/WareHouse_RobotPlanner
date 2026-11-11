
#include "algorithms/TaskPlanner.hpp"
#include "algorithms/Dijkstra.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <unordered_set>

namespace warehouse {

TaskPlan TaskPlanner::planGreedy(
    const Warehouse& warehouse,
    const Graph& graph,
    const std::string& robotId
) {
    TaskPlan plan;

    // Find the requested robot.
    const auto robotIt = std::find_if(
        warehouse.robots.begin(),
        warehouse.robots.end(),
        [&](const Robot& robot) {
            return robot.id == robotId;
        }
    );

    if (robotIt == warehouse.robots.end()) {
        plan.message = "Robot not found: " + robotId;
        return plan;
    }

    const Robot& robot = *robotIt;

    if (robot.capacityKg < 0.0 ||
        !std::isfinite(robot.capacityKg)) {
        plan.message = "Robot capacity is invalid.";
        return plan;
    }

    std::unordered_set<std::string> completedTaskIds;

    std::string currentLocation = robot.currentLocationId;
    double remainingCapacity = robot.capacityKg;

    plan.visitedLocations.push_back(currentLocation);

    // Repeatedly choose the nearest feasible pending task.
    while (true) {
        const Task* bestTask = nullptr;
        PathResult bestPath;
        double bestDistance = std::numeric_limits<double>::infinity();

        for (const auto& task : warehouse.tasks) {
            if (task.status != TaskStatus::Pending) {
                continue;
            }

            if (completedTaskIds.count(task.id) > 0) {
                continue;
            }

            if (task.weightKg < 0.0 ||
                !std::isfinite(task.weightKg)) {
                continue;
            }

            if (task.weightKg > remainingCapacity) {
                continue;
            }

            const PathResult path = Dijkstra::shortestPath(
                graph,
                currentLocation,
                task.pickupLocationId
            );

            if (!path.reachable) {
                continue;
            }

            const bool isCloser =
                path.totalDistance < bestDistance;

            const bool tieBreak =
                bestTask != nullptr &&
                path.totalDistance == bestDistance &&
                task.id < bestTask->id;

            if (isCloser || bestTask == nullptr || tieBreak) {
                bestTask = &task;
                bestPath = path;
                bestDistance = path.totalDistance;
            }
        }

        // No more reachable tasks fit within the remaining capacity.
        if (bestTask == nullptr) {
            break;
        }

        plan.totalDistance += bestPath.totalDistance;
        plan.totalTaskWeightKg += bestTask->weightKg;
        remainingCapacity -= bestTask->weightKg;

        plan.taskIds.push_back(bestTask->id);
        completedTaskIds.insert(bestTask->id);

        // Append the chosen path, avoiding a duplicate current location.
        for (std::size_t i = 1; i < bestPath.path.size(); ++i) {
            plan.visitedLocations.push_back(bestPath.path[i]);
        }

        currentLocation = bestTask->pickupLocationId;
    }

    plan.feasible = true;

    if (plan.taskIds.empty()) {
        plan.message =
            "No reachable pending tasks fit within the robot's capacity.";
    } else {
        plan.message =
            "Greedy plan created using nearest reachable tasks.";
    }

    return plan;
}

} // namespace warehouse
