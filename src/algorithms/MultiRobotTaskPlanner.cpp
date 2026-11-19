
#include "algorithms/MultiRobotTaskPlanner.hpp"

#include "algorithms/BranchAndBoundPlanner.hpp"
#include "algorithms/Dijkstra.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <unordered_set>
#include <vector>

namespace warehouse {

MultiRobotTaskPlanningResult MultiRobotTaskPlanner::plan(
    const Warehouse& warehouse,
    const Graph& graph
) {
    MultiRobotTaskPlanningResult result;

    const auto& robots = warehouse.robots;

    std::vector<const Task*> pendingTasks;

    std::unordered_set<std::string> robotIds;
    std::unordered_set<std::string> taskIds;

    // 1. Validate robots.
    for (const Robot& robot : robots) {
        if (robot.id.empty() ||
            !robotIds.insert(robot.id).second) {
            throw std::invalid_argument(
                "Robot IDs must be non-empty and unique."
            );
        }

        if (!std::isfinite(robot.capacityKg) ||
            robot.capacityKg < 0.0) {
            throw std::invalid_argument(
                "Robot capacity must be finite and non-negative."
            );
        }
    }

    // 2. Collect and validate pending tasks.
    for (const Task& task : warehouse.tasks) {
        if (task.id.empty() ||
            !taskIds.insert(task.id).second) {
            throw std::invalid_argument(
                "Task IDs must be non-empty and unique."
            );
        }

        if (task.status != TaskStatus::Pending) {
            continue;
        }

        if (!std::isfinite(task.weightKg) ||
            task.weightKg < 0.0 ||
            task.priority < 0) {
            throw std::invalid_argument(
                "Invalid task weight or priority: " + task.id
            );
        }

        pendingTasks.push_back(&task);
    }

    const std::size_t robotCount = robots.size();
    const std::size_t taskCount = pendingTasks.size();

    if (taskCount == 0) {
        result.feasible = true;
        result.message = "No pending tasks. Nothing to allocate.";
        return result;
    }

    // 3. Store feasible robot-task pairs and travel distances.
    std::vector<std::vector<double>> distances(
        robotCount,
        std::vector<double>(
            taskCount,
            std::numeric_limits<double>::infinity()
        )
    );

    std::vector<std::vector<std::size_t>> eligibleRobots(taskCount);

    for (std::size_t t = 0; t < taskCount; ++t) {
        const Task& task = *pendingTasks[t];

        for (std::size_t r = 0; r < robotCount; ++r) {
            const Robot& robot = robots[r];

            if (task.weightKg > robot.capacityKg) {
                continue;
            }

            const PathResult path = Dijkstra::shortestPath(
                graph,
                robot.currentLocationId,
                task.pickupLocationId
            );

            if (!path.reachable) {
                continue;
            }

            distances[r][t] = path.totalDistance;
            eligibleRobots[t].push_back(r);
        }
    }

    // 4. Process high-priority and constrained tasks first.
    std::vector<std::size_t> taskOrder(taskCount);

    for (std::size_t i = 0; i < taskCount; ++i) {
        taskOrder[i] = i;
    }

    std::stable_sort(
        taskOrder.begin(),
        taskOrder.end(),
        [&](std::size_t a, std::size_t b) {
            const Task& first = *pendingTasks[a];
            const Task& second = *pendingTasks[b];

            if (first.priority != second.priority) {
                return first.priority > second.priority;
            }

            if (eligibleRobots[a].size() !=
                eligibleRobots[b].size()) {
                return eligibleRobots[a].size() <
                       eligibleRobots[b].size();
            }

            if (first.weightKg != second.weightKg) {
                return first.weightKg > second.weightKg;
            }

            return first.id < second.id;
        }
    );

    // 5. Allocate tasks while respecting cumulative capacity.
    std::vector<std::vector<std::string>> assignedTasks(robotCount);

    std::vector<double> usedCapacity(robotCount, 0.0);

    for (const std::size_t taskIndex : taskOrder) {
        const Task& task = *pendingTasks[taskIndex];

        std::size_t bestRobot = robotCount;
        double bestDistance = std::numeric_limits<double>::infinity();
        double bestRemainingCapacity =
            std::numeric_limits<double>::infinity();

        for (const std::size_t robotIndex : eligibleRobots[taskIndex]) {
            const Robot& robot = robots[robotIndex];

            if (assignedTasks[robotIndex].size() >= 10) {
                continue;
            }

            const double newUsedCapacity =
                usedCapacity[robotIndex] + task.weightKg;

            const double tolerance =
                1e-9 * std::max(1.0, robot.capacityKg);

            if (newUsedCapacity > robot.capacityKg + tolerance) {
                continue;
            }

            const double remainingCapacity =
                robot.capacityKg - newUsedCapacity;

            const double travelDistance =
                distances[robotIndex][taskIndex];

            // Prefer shorter travel distance.
            // Break ties using best-fit remaining capacity.
            if (travelDistance < bestDistance ||
                (travelDistance == bestDistance &&
                 remainingCapacity < bestRemainingCapacity) ||
                (travelDistance == bestDistance &&
                 remainingCapacity == bestRemainingCapacity &&
                 bestRobot < robotCount &&
                 robot.id < robots[bestRobot].id)) {

                bestRobot = robotIndex;
                bestDistance = travelDistance;
                bestRemainingCapacity = remainingCapacity;
            }
        }

        if (bestRobot == robotCount) {
            result.unassignedTaskIds.push_back(task.id);
            continue;
        }

        assignedTasks[bestRobot].push_back(task.id);
        usedCapacity[bestRobot] += task.weightKg;
    }

    // 6. Optimize the route of each robot's assigned tasks.
    for (std::size_t r = 0; r < robotCount; ++r) {
        if (assignedTasks[r].empty()) {
            continue;
        }

        TaskPlan robotPlan = BranchAndBoundPlanner::planExact(
            warehouse,
            graph,
            robots[r].id,
            assignedTasks[r]
        );

        if (!robotPlan.feasible) {
            for (const std::string& taskId : assignedTasks[r]) {
                result.unassignedTaskIds.push_back(taskId);
            }

            continue;
        }

        result.assignedTaskCount += robotPlan.taskIds.size();

        result.robotPlans.push_back({
            robots[r].id,
            robotPlan
        });
    }

    // 7. Final result.
    result.unassignedTaskCount =
        result.unassignedTaskIds.size();

    result.feasible = (result.unassignedTaskCount == 0);

    if (result.feasible) {
        result.message =
            "All pending tasks were allocated successfully.";
    } else {
        result.message =
            "Partial allocation completed. Some tasks remain unassigned.";
    }

    return result;
}

} // namespace warehouse
