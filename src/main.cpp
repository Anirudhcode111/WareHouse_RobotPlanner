

#include <iostream>

#include "domain/validation.hpp"
#include "algorithms/WarehouseGraphBuilder.hpp"
#include "algorithms/Dijkstra.hpp"
#include "algorithms/TaskPlanner.hpp"

int main() {
    using namespace warehouse;

    Warehouse sample;

    // Warehouse locations.
    sample.locations = {
        {"A"}, {"B"}, {"C"}, {"D"}
    };

    // Robot ID, starting location, carrying capacity.
    sample.robots = {
        {"R1", "A", 20.0}
    };

    // Task ID, pickup location, weight, priority, status.
    sample.tasks = {
        {"T1", "B", 5.0, 8, TaskStatus::Pending},
        {"T2", "D", 7.5, 5, TaskStatus::Pending}
    };

    // Connections and distances.
    sample.connections = {
        {"A", "B", 2.0},
        {"A", "C", 5.0},
        {"B", "C", 1.0},
        {"B", "D", 7.0},
        {"C", "D", 1.0}
    };

    // This connection must not be used.
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

    // Step 2: Build a graph while excluding blocked connections.
    const Graph graph = WarehouseGraphBuilder::build(sample);

    // Step 3: Demonstrate shortest-path calculation.
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

        std::cout << "\nDistance: " << path.totalDistance << '\n';
    }

    // Step 4: Create a greedy task plan for robot R1.
    const TaskPlan plan = TaskPlanner::planGreedy(
        sample, graph, "R1"
    );

    std::cout << "\n--- Greedy Task Plan: R1 ---\n";
    std::cout << "Status: "
              << (plan.feasible ? "FEASIBLE" : "FAILED")
              << '\n';
    std::cout << "Message: " << plan.message << '\n';

    if (!plan.feasible) {
        return 1;
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

    std::cout << "\nIntegration workflow finished.\n";

    return 0;
}
