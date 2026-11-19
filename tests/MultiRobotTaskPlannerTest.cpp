
#include "algorithms/MultiRobotTaskPlanner.hpp"

#include <cassert>
#include <iostream>
#include <set>
#include <stdexcept>

using namespace warehouse;

int main()
{
    // Test 1: Multiple tasks assigned to one robot.
    Warehouse warehouse;

    warehouse.locations = {
        {"A"}, {"C"}, {"D"}, {"E"}
    };

    warehouse.robots = {
        {"R1", "A", 5.0}
    };

    warehouse.tasks = {
        {"T1", "C", 2.0, 10, TaskStatus::Pending},
        {"T2", "D", 3.0, 9, TaskStatus::Pending},
        {"T3", "E", 1.0, 1, TaskStatus::Pending}
    };

    Graph graph;

    graph.addLocation("A");
    graph.addLocation("C");
    graph.addLocation("D");
    graph.addLocation("E");

    graph.addConnection("A", "C", 2.0);
    graph.addConnection("C", "D", 2.0);
    graph.addConnection("D", "E", 2.0);

    const auto result = MultiRobotTaskPlanner::plan(
        warehouse, graph
    );

    assert(result.robotPlans.size() == 1);
    assert(result.assignedTaskCount == 2);
    assert(result.unassignedTaskCount == 1);
    assert(result.unassignedTaskIds.size() == 1);
    assert(result.unassignedTaskIds[0] == "T3");
    assert(!result.feasible);

    const auto& robotPlan = result.robotPlans[0];

    assert(robotPlan.robotId == "R1");
    assert(robotPlan.plan.feasible);
    assert(robotPlan.plan.taskIds.size() == 2);
    assert(robotPlan.plan.totalTaskWeightKg <= 5.0);

    std::set<std::string> assigned(
        robotPlan.plan.taskIds.begin(),
        robotPlan.plan.taskIds.end()
    );

    assert(assigned.count("T1") == 1);
    assert(assigned.count("T2") == 1);
    assert(assigned.count("T3") == 0);

    // Test 2: No pending tasks.
    Warehouse noTasks;

    noTasks.robots = {
        {"R1", "A", 5.0}
    };

    noTasks.tasks = {
        {"T1", "C", 2.0, 10, TaskStatus::Completed}
    };

    const auto emptyResult =
        MultiRobotTaskPlanner::plan(noTasks, graph);

    assert(emptyResult.feasible);
    assert(emptyResult.assignedTaskCount == 0);
    assert(emptyResult.unassignedTaskCount == 0);
    assert(emptyResult.robotPlans.empty());

    // Test 3: No robots.
    Warehouse noRobots;

    noRobots.tasks = {
        {"T1", "C", 2.0, 10, TaskStatus::Pending}
    };

    const auto noRobotResult =
        MultiRobotTaskPlanner::plan(noRobots, graph);

    assert(noRobotResult.assignedTaskCount == 0);
    assert(noRobotResult.unassignedTaskCount == 1);
    assert(noRobotResult.unassignedTaskIds[0] == "T1");

    // Test 4: Invalid robot capacity.
    Warehouse invalidWarehouse;

    invalidWarehouse.robots = {
        {"R1", "A", -5.0}
    };

    bool exceptionThrown = false;

    try {
        MultiRobotTaskPlanner::plan(invalidWarehouse, graph);
    }
    catch (const std::invalid_argument&) {
        exceptionThrown = true;
    }

    assert(exceptionThrown);

    std::cout << "Multi-Robot Task Planner Tests: PASS\n";

    return 0;
}
