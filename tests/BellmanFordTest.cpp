
#include "algorithms/shortest_path/BellmanFord.hpp"

#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>

using namespace warehouse;

int main() {
    // Test 1: Normal connected graph
    Graph graph;

    graph.addConnection("A", "B", 4);
    graph.addConnection("A", "C", 5);
    graph.addConnection("B", "C", 1);
    graph.addConnection("B", "D", 3);
    graph.addConnection("C", "D", 2);

    auto result = algorithms::bellmanFord(graph, "A");

    assert(!result.hasNegativeCycle);
    assert(std::abs(result.distances.at("A") - 0.0) < 1e-9);
    assert(std::abs(result.distances.at("B") - 4.0) < 1e-9);
    assert(std::abs(result.distances.at("C") - 5.0) < 1e-9);
    assert(std::abs(result.distances.at("D") - 7.0) < 1e-9);

    // Test 2: Disconnected vertex
    Graph disconnectedGraph;

    disconnectedGraph.addConnection("A", "B", 3);
    disconnectedGraph.addLocation("C");

    auto disconnectedResult =
        algorithms::bellmanFord(disconnectedGraph, "A");

    assert(!disconnectedResult.hasNegativeCycle);
    assert(std::abs(disconnectedResult.distances.at("B") - 3.0) < 1e-9);
    assert(std::isinf(disconnectedResult.distances.at("C")));

   // Test 3: Zero-weight edge
Graph zeroGraph;

zeroGraph.addConnection("A", "B", 0);
zeroGraph.addConnection("B", "C", 2);

auto zeroResult =
    algorithms::bellmanFord(zeroGraph, "A");

assert(!zeroResult.hasNegativeCycle);
assert(zeroResult.distances.at("A") == 0.0);
assert(zeroResult.distances.at("B") == 0.0);
assert(zeroResult.distances.at("C") == 2.0);
    // Test 4: Invalid source
    bool exceptionThrown = false;

    try {
        algorithms::bellmanFord(graph, "Z");
    }
    catch (const std::invalid_argument&) {
        exceptionThrown = true;
    }

    assert(exceptionThrown);

    // Test 5: Empty graph
    Graph emptyGraph;
    exceptionThrown = false;

    try {
        algorithms::bellmanFord(emptyGraph, "A");
    }
    catch (const std::invalid_argument&) {
        exceptionThrown = true;
    }

    assert(exceptionThrown);

    std::cout << "All Bellman-Ford tests passed!\n";

    return 0;
}
