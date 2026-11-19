
#include "algorithms/matching/RobotTaskMatcher.hpp"

#include <cassert>
#include <cmath>
#include <iostream>
#include <set>
#include <stdexcept>

using namespace warehouse;

int main()
{
    Warehouse warehouse;

    warehouse.locations = {
        {"A"}, {"B"}, {"C"}, {"D"}, {"E"}
    };

    warehouse.robots = {
        {"R1", "A", 5.0},
        {"R2", "B", 10.0}
    };

    warehouse.tasks = {
        {"T1", "C", 5.0, 10, TaskStatus::Pending},
        {"T2", "D", 10.0, 20, TaskStatus::Pending},
        {"T3", "E", 2.0, 5, TaskStatus::Completed}
    };

    warehouse.connections = {
        {"A", "C", 2.0},
        {"B", "C", 3.0},
        {"B", "D", 4.0}
    };

    Graph graph;

    graph.addLocation("A");
    graph.addLocation("B");
    graph.addLocation("C");
    graph.addLocation("D");
    graph.addLocation("E");

    graph.addConnection("A", "C", 2.0);
    graph.addConnection("B", "C", 3.0);
    graph.addConnection("B", "D", 4.0);

    // Test 1: Maximum matching with reassignment.
    const auto result = RobotTaskMatcher::match(
        warehouse, graph
    );

    assert(result.assignments.size() == 2);
    assert(result.unmatchedRobotCount == 0);
    assert(result.unmatchedTaskCount == 0);

    std::set<std::string> matchedRobots;
    std::set<std::string> matchedTasks;

    for (const auto& assignment : result.assignments) {
        matchedRobots.insert(assignment.robotId);
        matchedTasks.insert(assignment.taskId);
    }

    assert(matchedRobots.size() == 2);
    assert(matchedTasks.size() == 2);

    // R1 can only handle T1; R2 must get T2.
    assert(matchedTasks.count("T1") == 1);
    assert(matchedTasks.count("T2") == 1);
    assert(matchedTasks.count("T3") == 0);

    // Verify travel distances.
    for (const auto& assignment : result.assignments) {
        if (assignment.robotId == "R1") {
            assert(assignment.taskId == "T1");
            assert(std::fabs(assignment.travelDistance - 2.0) < 1e-9);
        }

        if (assignment.robotId == "R2") {
            assert(assignment.taskId == "T2");
            assert(std::fabs(assignment.travelDistance - 4.0) < 1e-9);
        }
    }

    // Test 2: No robots.
    Warehouse noRobots;
    noRobots.tasks = {
        {"T1", "C", 2.0, 5, TaskStatus::Pending}
    };

    const auto emptyRobotResult =
        RobotTaskMatcher::match(noRobots, graph);

    assert(emptyRobotResult.assignments.empty());
    assert(emptyRobotResult.unmatchedRobotCount == 0);
    assert(emptyRobotResult.unmatchedTaskCount == 1);

    // Test 3: No pending tasks.
    Warehouse noTasks;
    noTasks.robots = {
        {"R1", "A", 5.0}
    };

    const auto emptyTaskResult =
        RobotTaskMatcher::match(noTasks, graph);

    assert(emptyTaskResult.assignments.empty());
    assert(emptyTaskResult.unmatchedRobotCount == 1);
    assert(emptyTaskResult.unmatchedTaskCount == 0);

    // Test 4: Invalid robot capacity.
    Warehouse invalidWarehouse;
    invalidWarehouse.robots = {
        {"R1", "A", -5.0}
    };

    bool exceptionThrown = false;

    try {
        RobotTaskMatcher::match(invalidWarehouse, graph);
    }
    catch (const std::invalid_argument&) {
        exceptionThrown = true;
    }

    assert(exceptionThrown);

    std::cout << "Robot Task Matcher Tests: PASS\n";

    return 0;
}
