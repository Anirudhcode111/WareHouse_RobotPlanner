
cmake_minimum_required(VERSION 3.16)

project(WarehouseRobotPlanner VERSION 1.0.0 LANGUAGES CXX)

# C++17 configuration
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler warnings
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Core project source files
set(WAREHOUSE_CORE_SOURCES
    # Sorting algorithms
    src/algorithms/sorting/MergeSort.cpp

    # Domain and I/O
    src/domain/validation.cpp
    src/io/CsvLoader.cpp

    # Existing algorithms
    src/algorithms/Graph.cpp
    src/algorithms/Dijkstra.cpp
    src/algorithms/WarehouseGraphBuilder.cpp
    src/algorithms/TaskPlanner.cpp
    src/algorithms/BranchAndBoundPlanner.cpp

    # Dynamic Programming
    src/algorithms/dp/Knapsack.cpp

    # Optimal warehouse task planning
    src/algorithms/OptimalTaskPlanner.cpp

    # Bipartite matching
    src/algorithms/matching/RobotTaskMatcher.cpp

    # Multi-robot task planning
    src/algorithms/MultiRobotTaskPlanner.cpp
)

# Core library
add_library(warehouse_core STATIC
    ${WAREHOUSE_CORE_SOURCES}
)

target_include_directories(warehouse_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Main application
add_executable(warehouse_planner
    src/main.cpp
)

target_link_libraries(warehouse_planner
    PRIVATE warehouse_core
)

# Enable testing
enable_testing()

# Function to create and register tests
function(add_warehouse_test target_name source_file)
    add_executable(${target_name} ${source_file})

    target_link_libraries(${target_name}
        PRIVATE warehouse_core
    )

    add_test(
        NAME ${target_name}
        COMMAND ${target_name}
    )
endfunction()

# Graph and Dijkstra tests
add_warehouse_test(
    graph_dijkstra_test
    tests/graph_dijkstra_test.cpp
)

# Warehouse graph builder tests
add_warehouse_test(
    warehouse_graph_builder_test
    tests/warehouse_graph_builder_test.cpp
)

# Greedy task planner tests
add_warehouse_test(
    task_planner_test
    tests/task_planner_test.cpp
)

# Task planner edge cases
add_warehouse_test(
    task_planner_edge_cases_test
    tests/task_planner_edge_cases_test.cpp
)

# Greedy vs Branch and Bound comparison
add_warehouse_test(
    planner_comparison_test
    tests/planner_comparison_test.cpp
)

# Optimal task planner integration test
add_warehouse_test(
    optimal_task_planner_test
    tests/optimal_task_planner_test.cpp
)

# Robot-task matching test
add_warehouse_test(
    robot_task_matcher_test
    tests/RobotTaskMatcherTest.cpp
)

# Multi-robot task planning test
add_warehouse_test(
    multi_robot_task_planner_test
    tests/MultiRobotTaskPlannerTest.cpp
)

# Benchmark executable
add_executable(
    planner_benchmark
    tests/planner_benchmark.cpp
)

target_link_libraries(
    planner_benchmark
    PRIVATE warehouse_core
)
