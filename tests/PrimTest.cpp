
#include "algorithms/mst/Prim.hpp"

#include <cassert>
#include <cmath>
#include <iostream>
#include <stdexcept>

using namespace warehouse;

int main() {
    Graph graph;

    graph.addConnection("A", "B", 1);
    graph.addConnection("A", "C", 4);
    graph.addConnection("B", "C", 2);
    graph.addConnection("B", "D", 5);
    graph.addConnection("C", "D", 3);

    // Test 1: MST correctness
    auto result = algorithms::primMST(graph, "A");

    assert(result.connected);
    assert(result.selectedEdges.size() == 3);
    assert(std::abs(result.totalWeight - 6.0) < 1e-9);

    // Test 2: Every selected edge has valid endpoints
    for (const auto& edge : result.selectedEdges) {
        assert(!edge.source.empty());
        assert(!edge.destination.empty());
        assert(edge.source != edge.destination);
    }

    // Test 3: Single vertex graph
    Graph singleGraph;
    singleGraph.addLocation("X");

    auto singleResult = algorithms::primMST(singleGraph, "X");

    assert(singleResult.connected);
    assert(singleResult.selectedEdges.empty());
    assert(singleResult.totalWeight == 0.0);

    // Test 4: Disconnected graph
    Graph disconnectedGraph;
    disconnectedGraph.addConnection("A", "B", 2);
    disconnectedGraph.addLocation("C");

    auto disconnectedResult =
        algorithms::primMST(disconnectedGraph, "A");

    assert(!disconnectedResult.connected);
    assert(disconnectedResult.selectedEdges.size() == 1);
    assert(disconnectedResult.totalWeight == 2.0);

    // Test 5: Invalid starting vertex
    bool exceptionThrown = false;

    try {
        algorithms::primMST(graph, "Z");
    }
    catch (const std::invalid_argument&) {
        exceptionThrown = true;
    }

    assert(exceptionThrown);

    std::cout << "All Prim's Algorithm tests passed!\n";

    return 0;
}
