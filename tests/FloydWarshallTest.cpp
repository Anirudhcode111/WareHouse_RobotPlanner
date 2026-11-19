
#include "algorithms/shortest_path/FloydWarshall.hpp"

#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>

using namespace warehouse;

int main() {
    // Test 1: Normal weighted graph
    Graph graph;

    graph.addConnection("A", "B", 3);
    graph.addConnection("A", "C", 10);
    graph.addConnection("B", "C", 2);
    graph.addConnection("B", "D", 8);
    graph.addConnection("C", "D", 1);

    auto result = algorithms::floydWarshall(graph);

    assert(!result.hasNegativeCycle);

    auto indexOf = [&](const std::string& vertex) {
        for (std::size_t i = 0; i < result.vertices.size(); ++i) {
            if (result.vertices[i] == vertex) {
                return i;
            }
        }

        assert(false);
        return std::size_t{0};
    };

    auto A = indexOf("A");
    auto B = indexOf("B");
    auto C = indexOf("C");
    auto D = indexOf("D");

    assert(std::abs(result.distances[A][A] - 0.0) < 1e-9);
    assert(std::abs(result.distances[A][B] - 3.0) < 1e-9);
    assert(std::abs(result.distances[A][C] - 5.0) < 1e-9);
    assert(std::abs(result.distances[A][D] - 6.0) < 1e-9);

    // Test 2: Symmetry for undirected graph
    assert(std::abs(
        result.distances[A][D] - result.distances[D][A]
    ) < 1e-9);

    // Test 3: Disconnected vertex
    Graph disconnectedGraph;

    disconnectedGraph.addConnection("P", "Q", 4);
    disconnectedGraph.addLocation("R");

    auto disconnectedResult =
        algorithms::floydWarshall(disconnectedGraph);

    std::size_t p = 0, r = 0;

    for (std::size_t i = 0;
         i < disconnectedResult.vertices.size(); ++i) {
        if (disconnectedResult.vertices[i] == "P") p = i;
        if (disconnectedResult.vertices[i] == "R") r = i;
    }

    assert(std::isinf(disconnectedResult.distances[p][r]));

    // Test 4: Empty graph
    Graph emptyGraph;

    auto emptyResult = algorithms::floydWarshall(emptyGraph);

    assert(emptyResult.vertices.empty());
    assert(emptyResult.distances.empty());
    assert(!emptyResult.hasNegativeCycle);

    std::cout << "All Floyd-Warshall tests passed!\n";

    return 0;
}
