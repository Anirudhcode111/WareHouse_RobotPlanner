
#include "algorithms/TaskPlanner.hpp"
#include "domain/Warehouse.hpp"
#include "algorithms/Graph.hpp"

#include <cassert>
#include <cmath>
#include <iostream>
#include <set>

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
        {"T2", "C", 7.5, 5, TaskStatus::Pending},
        {"T3", "C", 12.0, 20, TaskStatus::Pending},
        {"T4", "D", 3.0, 100, TaskStatus::Completed}
    };

    Graph graph;

    graph.addLocation("A");
    graph.addLocation("B");
    graph.addLocation("C");
    graph.addLocation("D");

    graph.addConnection("A", "B", 2.0);
    graph.addConnection("A", "C", 5.0);
    graph.addConnection("B", "C", 1.0);

    // T4 is completed and must not be selected.
    // T1 + T3 gives priority 28 with weight 17 kg.
    // T2 + T3 gives priority 25 with weight 19.5 kg.
    const TaskPlan result =
        TaskPlanner::planOptimal(warehouse, graph, "R1");

    assert(result.feasible);
    assert(result.taskIds.size() == 2);

    const std::set<std::string> selected(
        result.taskIds.begin(),
        result.taskIds.end()
    );

    assert(selected.count("T1") == 1);
    assert(selected.count("T3") == 1);
    assert(selected.count("T2") == 0);
    assert(selected.count("T4") == 0);

    assert(std::fabs(result.totalTaskWeightKg - 17.0) < 1e-9);
    assert(std::fabs(result.totalDistance - 3.0) < 1e-9);

    assert(!result.visitedLocations.empty());
    assert(result.visitedLocations.front() == "A");
    assert(result.visitedLocations.back() == "C");

    std::cout << "Optimal task planner integration: PASS\n";
    return 0;
}
