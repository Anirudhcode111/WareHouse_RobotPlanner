
#include "algorithms/TaskPlanner.hpp"

#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>

using namespace warehouse;

int main() {
    // Test 1: Missing robot must fail.
    {
        Warehouse w;
        w.robots = {{"R1", "A", 10.0}};

        Graph graph;
        graph.addConnection("A", "B", 2.0);

        const TaskPlan plan =
            TaskPlanner::planGreedy(w, graph, "R99");

        assert(!plan.feasible);
        std::cout << "Test 1 PASS: Missing robot rejected\n";
    }

    // Test 2: An unreachable task must not be selected.
    {
        Warehouse w;
        w.robots = {{"R1", "A", 10.0}};
        w.tasks = {
            {"T1", "C", 2.0, 5, TaskStatus::Pending}
        };

        Graph graph;
        graph.addConnection("A", "B", 1.0);
        // C has no connection to A or B.

        const TaskPlan plan =
            TaskPlanner::planGreedy(w, graph, "R1");

        assert(plan.feasible);
        assert(plan.taskIds.empty());
        assert(std::abs(plan.totalDistance) < 1e-9);

        std::cout << "Test 2 PASS: Unreachable task skipped\n";
    }

    // Test 3: Zero capacity can carry a zero-weight task.
    {
        Warehouse w;
        w.robots = {{"R1", "A", 0.0}};
        w.tasks = {
            {"T0", "B", 0.0, 1, TaskStatus::Pending}
        };

        Graph graph;
        graph.addConnection("A", "B", 3.0);

        const TaskPlan plan =
            TaskPlanner::planGreedy(w, graph, "R1");

        assert(plan.feasible);
        assert(plan.taskIds.size() == 1);
        assert(plan.taskIds[0] == "T0");
        assert(std::abs(plan.totalTaskWeightKg) < 1e-9);
        assert(std::abs(plan.totalDistance - 3.0) < 1e-9);

        std::cout << "Test 3 PASS: Zero-capacity boundary handled\n";
    }

    // Test 4: Invalid infinite robot capacity must fail.
    {
        Warehouse w;
        w.robots = {
            {"R1", "A", std::numeric_limits<double>::infinity()}
        };

        Graph graph;
        graph.addConnection("A", "B", 1.0);

        const TaskPlan plan =
            TaskPlanner::planGreedy(w, graph, "R1");

        assert(!plan.feasible);

        std::cout << "Test 4 PASS: Invalid capacity rejected\n";
    }

    std::cout << "\nAll edge-case tests passed.\n";
    return 0;
}
