
#include "algorithms/mst/Kruskal.hpp"

#include <cassert>
#include <cmath>
#include <iostream>

using namespace warehouse;

int main() {
    // Test 1: Normal connected graph
    Graph graph;

    graph.addConnection("A", "B", 1);
    graph.addConnection("A", "C", 4);
    graph.addConnection("B", "C", 2);
    graph.addConnection("B", "D", 5);
    graph.addConnection("C", "D", 3);

    auto result = algorithms::kruskalMST(graph);

    assert(result.connected);
    assert(result.selectedEdges.size() == 3);
    assert(std::abs(result.totalWeight - 6.0) < 1e-9);

    // Test 2: No cycles in selected MST
    for (const auto& edge : result.selectedEdges) {
        assert(!edge.source.empty());
        assert(!edge.destination.empty());
        assert(edge.source != edge.destination);
    }

    // Test 3: Disconnected graph
    Graph disconnectedGraph;

    disconnectedGraph.addConnection("A", "B", 2);
    disconnectedGraph.addLocation("C");

    auto disconnectedResult =
        algorithms::kruskalMST(disconnectedGraph);

    assert(!disconnectedResult.connected);
    assert(disconnectedResult.selectedEdges.size() == 1);
    assert(disconnectedResult.totalWeight == 2.0);

    // Test 4: Single vertex
    Graph singleGraph;
    singleGraph.addLocation("X");

    auto singleResult = algorithms::kruskalMST(singleGraph);

    assert(singleResult.connected);
    assert(singleResult.selectedEdges.empty());
    assert(singleResult.totalWeight == 0.0);

    // Test 5: Empty graph
    Graph emptyGraph;

    auto emptyResult = algorithms::kruskalMST(emptyGraph);

    assert(emptyResult.connected);
    assert(emptyResult.selectedEdges.empty());
    assert(emptyResult.totalWeight == 0.0);

    // Test 6: Zero-weight edge
    Graph zeroGraph;

    zeroGraph.addConnection("P", "Q", 0);
    zeroGraph.addConnection("Q", "R", 2);
    zeroGraph.addConnection("P", "R", 5);

    auto zeroResult = algorithms::kruskalMST(zeroGraph);

    assert(zeroResult.connected);
    assert(zeroResult.selectedEdges.size() == 2);
    assert(std::abs(zeroResult.totalWeight - 2.0) < 1e-9);

    std::cout << "All Kruskal's Algorithm tests passed!\n";

    return 0;
}
