

#include "algorithms/TaskPlanner.hpp"
#include "algorithms/BranchAndBoundPlanner.hpp"
#include "algorithms/WarehouseGraphBuilder.hpp"

#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using namespace warehouse;

using Clock = std::chrono::steady_clock;

constexpr int RUNS = 10;
constexpr double TOLERANCE = 1e-9;

const char* CSV_FILENAME = "planner_benchmark_results.csv";

// Check that both planners produce valid plans for the same task set.
bool verifyPlans(
    const TaskPlan& greedy,
    const TaskPlan& exact,
    std::size_t expectedTaskCount
) {
    if (!greedy.feasible || !exact.feasible) {
        return false;
    }

    if (greedy.taskIds.size() != expectedTaskCount ||
        exact.taskIds.size() != expectedTaskCount) {
        return false;
    }

    if (std::abs(
            greedy.totalTaskWeightKg -
            exact.totalTaskWeightKg
        ) > TOLERANCE) {
        return false;
    }

    if (exact.totalDistance >
        greedy.totalDistance + TOLERANCE) {
        return false;
    }

    return true;
}

// Measure average Greedy execution time.
double benchmarkGreedy(
    const Warehouse& warehouse,
    const Graph& graph,
    TaskPlan& lastPlan
) {
    const auto start = Clock::now();

    for (int i = 0; i < RUNS; ++i) {
        lastPlan = TaskPlanner::planGreedy(
            warehouse, graph, "R1"
        );
    }

    const auto finish = Clock::now();

    return std::chrono::duration<double, std::micro>(
        finish - start
    ).count() / RUNS;
}

// Measure average Branch & Bound execution time.
double benchmarkExact(
    const Warehouse& warehouse,
    const Graph& graph,
    const std::vector<std::string>& taskIds,
    TaskPlan& lastPlan
) {
    const auto start = Clock::now();

    for (int i = 0; i < RUNS; ++i) {
        lastPlan = BranchAndBoundPlanner::planExact(
            warehouse, graph, "R1", taskIds
        );
    }

    const auto finish = Clock::now();

    return std::chrono::duration<double, std::micro>(
        finish - start
    ).count() / RUNS;
}

// Write benchmark results to a CSV file.
bool exportCsv(
    const Warehouse& warehouse,
    std::size_t taskCount,
    double greedyTime,
    const TaskPlan& greedy,
    double exactTime,
    const TaskPlan& exact
) {
    std::ofstream csv(CSV_FILENAME);

    if (!csv.is_open()) {
        std::cerr
            << "ERROR: Could not create CSV file: "
            << CSV_FILENAME << '\n';
        return false;
    }

    csv << "Algorithm,Locations,Tasks,Runs,"
           "AverageTimeMicroseconds,TotalDistance,"
           "TotalTaskWeightKg,Verification\n";

    csv << std::fixed << std::setprecision(6);

    csv << "Greedy,"
        << warehouse.locations.size() << ','
        << taskCount << ','
        << RUNS << ','
        << greedyTime << ','
        << greedy.totalDistance << ','
        << greedy.totalTaskWeightKg << ",PASS\n";

    csv << "BranchAndBound,"
        << warehouse.locations.size() << ','
        << taskCount << ','
        << RUNS << ','
        << exactTime << ','
        << exact.totalDistance << ','
        << exact.totalTaskWeightKg << ",PASS\n";

    csv.close();

    if (!csv) {
        std::cerr << "ERROR: Failed while writing CSV results.\n";
        return false;
    }

    return true;
}

int main() {
    Warehouse warehouse;

    // Warehouse locations.
    warehouse.locations = {
        {"A"}, {"B"}, {"C"}, {"D"},
        {"E"}, {"F"}, {"G"}, {"H"}
    };

    // Robot starts at A with a 20 kg capacity.
    warehouse.robots = {
        {"R1", "A", 20.0}
    };

    // Seven pending tasks, each weighing 1 kg.
    warehouse.tasks = {
        {"T1", "B", 1.0, 5, TaskStatus::Pending},
        {"T2", "C", 1.0, 5, TaskStatus::Pending},
        {"T3", "D", 1.0, 5, TaskStatus::Pending},
        {"T4", "E", 1.0, 5, TaskStatus::Pending},
        {"T5", "F", 1.0, 5, TaskStatus::Pending},
        {"T6", "G", 1.0, 5, TaskStatus::Pending},
        {"T7", "H", 1.0, 5, TaskStatus::Pending}
    };

    // Warehouse connections and their distances.
    warehouse.connections = {
        {"A", "B", 2.0},
        {"B", "C", 2.0},
        {"C", "D", 2.0},
        {"D", "E", 2.0},
        {"E", "F", 2.0},
        {"F", "G", 2.0},
        {"G", "H", 2.0},
        {"A", "D", 3.0},
        {"B", "F", 4.0},
        {"C", "H", 5.0}
    };

    // Build the graph from warehouse data.
    const Graph graph =
        WarehouseGraphBuilder::build(warehouse);

    // Prepare the same task list for the exact planner.
    std::vector<std::string> taskIds;

    for (const auto& task : warehouse.tasks) {
        taskIds.push_back(task.id);
    }

    // Verify both planners before measuring their performance.
    const TaskPlan initialGreedy =
        TaskPlanner::planGreedy(
            warehouse, graph, "R1"
        );

    const TaskPlan initialExact =
        BranchAndBoundPlanner::planExact(
            warehouse, graph, "R1", taskIds
        );

    if (!verifyPlans(
            initialGreedy,
            initialExact,
            taskIds.size()
        )) {
        std::cerr
            << "ERROR: Initial verification failed.\n"
            << "Greedy: " << initialGreedy.message << '\n'
            << "Exact: " << initialExact.message << '\n';

        return 1;
    }

    // Benchmark both algorithms.
    TaskPlan greedy;
    TaskPlan exact;

    const double greedyTime = benchmarkGreedy(
        warehouse, graph, greedy
    );

    const double exactTime = benchmarkExact(
        warehouse, graph, taskIds, exact
    );

    // Verify the final measured results.
    if (!verifyPlans(greedy, exact, taskIds.size())) {
        std::cerr
            << "ERROR: Final benchmark verification failed.\n";
        return 1;
    }

    // Export results.
    if (!exportCsv(
            warehouse,
            taskIds.size(),
            greedyTime,
            greedy,
            exactTime,
            exact
        )) {
        return 1;
    }

    // Print report.
    std::cout
        << "========================================\n"
        << " Warehouse Planner Benchmark\n"
        << "========================================\n";

    std::cout << "Locations: "
              << warehouse.locations.size() << '\n';

    std::cout << "Tasks: "
              << taskIds.size() << '\n';

    std::cout << "Timing runs: "
              << RUNS << "\n\n";

    std::cout << std::fixed << std::setprecision(3);

    std::cout << "Greedy Planner\n";
    std::cout << "Average time: "
              << greedyTime << " microseconds\n";
    std::cout << "Total distance: "
              << greedy.totalDistance << '\n';
    std::cout << "Total task weight: "
              << greedy.totalTaskWeightKg << " kg\n\n";

    std::cout << "Branch & Bound Planner\n";
    std::cout << "Average time: "
              << exactTime << " microseconds\n";
    std::cout << "Total distance: "
              << exact.totalDistance << '\n';
    std::cout << "Total task weight: "
              << exact.totalTaskWeightKg << " kg\n\n";

    std::cout << "Verification: PASS\n";
    std::cout << "CSV saved to: "
              << CSV_FILENAME << '\n';

    return 0;
}
