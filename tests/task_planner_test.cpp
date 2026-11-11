
#include "algorithms/TaskPlanner.hpp"

#include <cassert>
#include <cmath>
#include <iostream>

using namespace warehouse;

int main() {
    Warehouse warehouse;

    // Create warehouse locations.
    warehouse.locations = {
        {"A"}, {"B"}, {"C"}, {"D"}
    };

    // One robot starts at A and can carry 10 kg.
    warehouse.robots = {
        {"R1", "A", 10.0}
    };

    // T1 is nearest, T2 is too heavy after T1,
    // and T3 should fit in the remaining capacity.
    warehouse.tasks = {
        {"T1", "B", 4.0, 8, TaskStatus::Pending},
        {"T2", "C", 7.0, 5, TaskStatus::Pending},
        {"T3", "D", 5.0, 6, TaskStatus::Pending}
    };

    // Build the warehouse graph.
    Graph graph;
    graph.addConnection("A", "B", 2.0);
    graph.addConnection("B", "C", 1.0);
    graph.addConnection("B", "D", 3.0);

    // Run the greedy planner.
    const TaskPlan plan =
        TaskPlanner::planGreedy(warehouse, graph, "R1");

    // Verify the plan.
    assert(plan.feasible);
    assert(plan.taskIds.size() == 2);
    assert(plan.taskIds[0] == "T1");
    assert(plan.taskIds[1] == "T3");

    assert(std::abs(plan.totalTaskWeightKg - 9.0) < 1e-9);
    assert(std::abs(plan.totalDistance - 5.0) < 1e-9);

    // Verify invalid robot ID handling.
    const TaskPlan missingRobot =
        TaskPlanner::planGreedy(warehouse, graph, "R99");

    assert(!missingRobot.feasible);

    std::cout << "Task Planner tests: PASS\n";
    std::cout << "Selected tasks: ";

    for (const auto& taskId : plan.taskIds) {
        std::cout << taskId << " ";
    }

    std::cout << "\nTotal distance: "
              << plan.totalDistance << '\n';

    std::cout << "Total task weight: "
              << plan.totalTaskWeightKg << " kg\n";

    return 0;
}
