
#include "algorithms/TaskPlanner.hpp"
#include "algorithms/BranchAndBoundPlanner.hpp"
#include "algorithms/WarehouseGraphBuilder.hpp"

#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

using namespace warehouse;

int main() {
    Warehouse warehouse;

    warehouse.locations = {
        {"A"}, {"B"}, {"C"}, {"D"}
    };

    warehouse.robots = {
        {"R1", "A", 20.0}
    };

    warehouse.tasks = {
        {"T1", "B", 5.0, 8, TaskStatus::Pending},
        {"T2", "D", 7.5, 5, TaskStatus::Pending}
    };

    warehouse.connections = {
        {"A", "B", 2.0},
        {"A", "C", 5.0},
        {"B", "C", 1.0},
        {"B", "D", 7.0},
        {"C", "D", 1.0}
    };

    warehouse.obstacles = {
        {"B", "C"}
    };

    const Graph graph = WarehouseGraphBuilder::build(warehouse);

    // Run Greedy on the warehouse's pending tasks.
    const TaskPlan greedy =
        TaskPlanner::planGreedy(warehouse, graph, "R1");

    assert(greedy.feasible);
    assert(greedy.taskIds.size() == 2);

    // Run exact ordering on the SAME task set.
    const std::vector<std::string> taskIds = {"T1", "T2"};

    const TaskPlan exact =
        BranchAndBoundPlanner::planExact(
            warehouse, graph, "R1", taskIds
        );

    assert(exact.feasible);
    assert(exact.taskIds.size() == 2);

    // Both plans must have the same total carried weight.
    assert(std::abs(
        greedy.totalTaskWeightKg - exact.totalTaskWeightKg
    ) < 1e-9);

    // Exact ordering must not be worse than Greedy for this
    // same fixed set of tasks and distance objective.
    assert(exact.totalDistance <= greedy.totalDistance + 1e-9);

    std::cout << "Greedy vs Branch & Bound comparison\n";
    std::cout << "-----------------------------------\n";

    std::cout << "Greedy order: ";
    for (const auto& id : greedy.taskIds) {
        std::cout << id << " ";
    }
    std::cout << "\nGreedy distance: " << greedy.totalDistance << '\n';

    std::cout << "Exact order: ";
    for (const auto& id : exact.taskIds) {
        std::cout << id << " ";
    }
    std::cout << "\nExact distance: " << exact.totalDistance << '\n';

    std::cout << "\nComparison test: PASS\n";
    return 0;
}
