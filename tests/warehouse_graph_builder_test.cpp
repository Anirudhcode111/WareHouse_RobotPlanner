
#include "algorithms/WarehouseGraphBuilder.hpp"
#include "algorithms/Dijkstra.hpp"

#include <cassert>
#include <cmath>
#include <iostream>

int main() {
    warehouse::Warehouse sample;

    sample.locations = {
        {"A"}, {"B"}, {"C"}
    };

    sample.connections = {
        {"A", "B", 2.0},
        {"B", "C", 3.0},
        {"A", "C", 10.0}
    };

    // Block the B-C connection.
    sample.obstacles = {
        {"B", "C"}
    };

    const warehouse::Graph graph =
        warehouse::WarehouseGraphBuilder::build(sample);

    const auto result =
        warehouse::Dijkstra::shortestPath(graph, "A", "C");

    // B-C is blocked, so the direct A-C route should be used.
    assert(result.reachable);
    assert(std::abs(result.totalDistance - 10.0) < 1e-9);
    assert(result.path.size() == 2);
    assert(result.path[0] == "A");
    assert(result.path[1] == "C");

    // All locations should exist, including locations without connections.
    sample.locations.push_back({"Z"});

    const warehouse::Graph graphWithIsolatedLocation =
        warehouse::WarehouseGraphBuilder::build(sample);

    const auto unreachable =
        warehouse::Dijkstra::shortestPath(
            graphWithIsolatedLocation, "A", "Z"
        );

    assert(!unreachable.reachable);

    std::cout << "Warehouse graph builder tests passed!\n";
    return 0;
}
