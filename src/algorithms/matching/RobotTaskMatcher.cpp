
#include "algorithms/matching/RobotTaskMatcher.hpp"
#include "algorithms/Dijkstra.hpp"

#include <cmath>
#include <functional>
#include <stdexcept>
#include <unordered_set>
#include <vector>

namespace warehouse {

RobotTaskMatchingResult RobotTaskMatcher::match(
    const Warehouse& warehouse,
    const Graph& graph
) {
    RobotTaskMatchingResult result;

    const auto& robots = warehouse.robots;
    const auto& allTasks = warehouse.tasks;

    std::unordered_set<std::string> robotIds;
    std::unordered_set<std::string> taskIds;

    // Validate robot IDs and capacities.
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

    // Collect pending tasks and validate their data.
    std::vector<const Task*> pendingTasks;

    for (const Task& task : allTasks) {
        if (task.status != TaskStatus::Pending) {
            continue;
        }

        if (task.id.empty() ||
            !taskIds.insert(task.id).second) {
            throw std::invalid_argument(
                "Pending task IDs must be non-empty and unique."
            );
        }

        if (!std::isfinite(task.weightKg) ||
            task.weightKg < 0.0) {
            throw std::invalid_argument(
                "Task weight must be finite and non-negative."
            );
        }

        pendingTasks.push_back(&task);
    }

    const std::size_t robotCount = robots.size();
    const std::size_t taskCount = pendingTasks.size();

    if (robotCount == 0 || taskCount == 0) {
        result.unmatchedRobotCount = robotCount;
        result.unmatchedTaskCount = taskCount;
        return result;
    }

    // Build the robot-to-task feasibility graph.
    std::vector<std::vector<bool>> feasible(
        robotCount,
        std::vector<bool>(taskCount, false)
    );

    std::vector<std::vector<double>> distances(
        robotCount,
        std::vector<double>(taskCount, 0.0)
    );

    for (std::size_t r = 0; r < robotCount; ++r) {
        for (std::size_t t = 0; t < taskCount; ++t) {
            const Robot& robot = robots[r];
            const Task& task = *pendingTasks[t];

            // Capacity constraint.
            if (task.weightKg > robot.capacityKg) {
                continue;
            }

            // Reachability constraint.
            const PathResult path = Dijkstra::shortestPath(
                graph,
                robot.currentLocationId,
                task.pickupLocationId
            );

            if (!path.reachable) {
                continue;
            }

            feasible[r][t] = true;
            distances[r][t] = path.totalDistance;
        }
    }

    // taskToRobot[t] stores the robot currently assigned to task t.
    std::vector<int> taskToRobot(taskCount, -1);

    // Kuhn's DFS-based augmenting path algorithm.
    std::function<bool(int, std::vector<bool>&)> tryAssign;

    tryAssign = [&](int robotIndex, std::vector<bool>& visited) {
        for (std::size_t t = 0; t < taskCount; ++t) {
            if (!feasible[robotIndex][t] || visited[t]) {
                continue;
            }

            visited[t] = true;

            // Assign an unoccupied task or reassign its current robot.
            if (taskToRobot[t] == -1 ||
                tryAssign(taskToRobot[t], visited)) {

                taskToRobot[t] = robotIndex;
                return true;
            }
        }

        return false;
    };

    std::size_t matchedRobots = 0;

    for (std::size_t r = 0; r < robotCount; ++r) {
        std::vector<bool> visited(taskCount, false);

        if (tryAssign(static_cast<int>(r), visited)) {
            ++matchedRobots;
        }
    }

    // Convert matching into readable assignments.
    for (std::size_t t = 0; t < taskCount; ++t) {
        if (taskToRobot[t] == -1) {
            continue;
        }

        const std::size_t r =
            static_cast<std::size_t>(taskToRobot[t]);

        result.assignments.push_back({
            robots[r].id,
            pendingTasks[t]->id,
            distances[r][t]
        });
    }

    result.unmatchedRobotCount = robotCount - matchedRobots;
    result.unmatchedTaskCount = taskCount - result.assignments.size();

    return result;
}

} // namespace warehouse
