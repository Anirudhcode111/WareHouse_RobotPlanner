
#include <iostream>
#include <string>

#include "domain/validation.hpp"
#include "algorithms/WarehouseGraphBuilder.hpp"
#include "algorithms/Dijkstra.hpp"
#include "algorithms/TaskPlanner.hpp"
#include "algorithms/matching/RobotTaskMatcher.hpp"

using namespace warehouse;

// Display single-robot task planning result.
void displayPlan(
    const TaskPlan& plan,
    const std::string& plannerName
) {
    std::cout << "\n--- " << plannerName << " Task Plan: R1 ---\n";

    std::cout << "Status: "
              << (plan.feasible ? "FEASIBLE" : "FAILED")
              << '\n';

    std::cout << "Message: " << plan.message << '\n';

    if (!plan.feasible) {
        return;
    }

    std::cout << "Selected tasks: ";

    if (plan.taskIds.empty()) {
        std::cout << "None";
    } else {
        for (std::size_t i = 0; i < plan.taskIds.size(); ++i) {
            if (i > 0) {
                std::cout << ", ";
            }

            std::cout << plan.taskIds[i];
        }
    }

    std::cout << "\nVisit sequence: ";

    for (std::size_t i = 0; i < plan.visitedLocations.size(); ++i) {
        if (i > 0) {
            std::cout << " -> ";
        }

        std::cout << plan.visitedLocations[i];
    }

    std::cout << "\nTotal travel distance: "
              << plan.totalDistance << '\n';

    std::cout << "Total task weight: "
              << plan.totalTaskWeightKg << " kg\n";
}

// Display multi-robot matching result.
void displayMatching(
    const RobotTaskMatchingResult& result
) {
    std::cout << "\n--- Multi-Robot Task Assignment ---\n";

    if (result.assignments.empty()) {
        std::cout << "No feasible assignments found.\n";
    } else {
        for (const auto& assignment : result.assignments) {
            std::cout
                << "Robot " << assignment.robotId
                << " -> Task " << assignment.taskId
                << " | Travel distance: "
                << assignment.travelDistance
                << '\n';
        }
    }

    std::cout << "\nTotal assignments: "
              << result.assignments.size() << '\n';

    std::cout << "Unmatched robots: "
              << result.unmatchedRobotCount << '\n';

    std::cout << "Unmatched tasks: "
              << result.unmatchedTaskCount << '\n';
}

int main()
{
    Warehouse sample;

    // Warehouse locations.
    sample.locations = {
        {"A"}, {"B"}, {"C"}, {"D"}, {"E"}
    };

    // Two robots with different starting locations and capacities.
    sample.robots = {
        {"R1", "A", 20.0},
        {"R2", "B", 10.0}
    };

    // Task ID, pickup location, weight, priority, status.
    sample.tasks = {
        {"T1", "C", 5.0, 8, TaskStatus::Pending},
        {"T2", "D", 7.5, 5, TaskStatus::Pending},
        {"T3", "E", 3.0, 10, TaskStatus::Pending}
    };

    // Warehouse connections and distances.
    sample.connections = {
        {"A", "B", 2.0},
        {"A", "C", 5.0},
        {"B", "C", 1.0},
        {"B", "D", 7.0},
        {"C", "D", 1.0},
        {"D", "E", 2.0}
    };

    // Blocked connection.
    sample.obstacles = {
        {"B", "C"}
    };

    std::cout
        << "========================================\n"
        << "     Warehouse Robot Path & Planner\n"
        << "========================================\n\n";

    std::cout << "Locations: " << sample.locations.size() << '\n';
    std::cout << "Robots:    " << sample.robots.size() << '\n';
    std::cout << "Tasks:     " << sample.tasks.size() << '\n';
    std::cout << "Obstacles: " << sample.obstacles.size() << "\n\n";

    // Step 1: Validate warehouse data.
    const auto errors = validateWarehouseData(sample);

    if (!errors.empty()) {
        std::cout << "Warehouse validation: FAIL\n";

        for (const auto& error : errors) {
            std::cout << " - " << error << '\n';
        }

        return 1;
    }

    std::cout << "Warehouse validation: PASS\n";

    // Step 2: Build graph excluding blocked connections.
    const Graph graph = WarehouseGraphBuilder::build(sample);

    // Step 3: Demonstrate Dijkstra shortest path.
    const PathResult path = Dijkstra::shortestPath(
        graph, "A", "D"
    );

    std::cout << "\n--- Shortest Path: A to D ---\n";

    if (!path.reachable) {
        std::cout << "Destination is unreachable.\n";
    } else {
        std::cout << "Route: ";

        for (std::size_t i = 0; i < path.path.size(); ++i) {
            if (i > 0) {
                std::cout << " -> ";
            }

            std::cout << path.path[i];
        }

        std::cout << "\nDistance: "
                  << path.totalDistance << '\n';
    }

    // Step 4: Planning menu.
    std::cout << "\n========================================\n";
    std::cout << "          TASK PLANNING MENU\n";
    std::cout << "========================================\n";
    std::cout << "1. Greedy Task Planning (Single Robot)\n";
    std::cout << "2. Optimal Task Planning (Single Robot)\n";
    std::cout << "3. Multi-Robot Task Assignment\n";
    std::cout << "Enter your choice: ";

    int choice;

    if (!(std::cin >> choice)) {
        std::cout << "Invalid input. Please enter 1, 2 or 3.\n";
        return 1;
    }

    // Step 5: Execute selected planning algorithm.
    switch (choice) {
        case 1: {
            const TaskPlan plan = TaskPlanner::planGreedy(
                sample, graph, "R1"
            );

            displayPlan(plan, "Greedy");

            if (!plan.feasible) {
                return 1;
            }

            break;
        }

        case 2: {
            const TaskPlan plan = TaskPlanner::planOptimal(
                sample, graph, "R1"
            );

            displayPlan(plan, "Optimal");

            if (!plan.feasible) {
                return 1;
            }

            break;
        }

        case 3: {
            const RobotTaskMatchingResult result =
                RobotTaskMatcher::match(sample, graph);

            displayMatching(result);
            break;
        }

        default:
            std::cout << "Invalid choice. Please select 1, 2 or 3.\n";
            return 1;
    }

    std::cout << "\nPlanning workflow finished.\n";

    return 0;
}
