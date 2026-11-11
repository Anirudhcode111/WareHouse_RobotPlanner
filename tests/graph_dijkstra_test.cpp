
#include "algorithms/Graph.hpp"
#include "algorithms/Dijkstra.hpp"

#include <cassert>
#include <cmath>
#include <iostream>

int main() {
    warehouse::Graph graph;

    graph.addConnection("A", "B", 4.0);
    graph.addConnection("A", "C", 1.0);
    graph.addConnection("C", "B", 2.0);
    graph.addConnection("B", "D", 1.0);
    graph.addConnection("C", "D", 5.0);

    // Shortest route A -> C -> B -> D = 1 + 2 + 1 = 4.
    const auto result =
        warehouse::Dijkstra::shortestPath(graph, "A", "D");

    assert(result.reachable);
    assert(std::abs(result.totalDistance - 4.0) < 1e-9);
    assert(result.path.size() == 4);
    assert(result.path[0] == "A");
    assert(result.path[1] == "C");
    assert(result.path[2] == "B");
    assert(result.path[3] == "D");

    // Unreachable destination.
    graph.addLocation("Z");
    const auto unreachable =
        warehouse::Dijkstra::shortestPath(graph, "A", "Z");

    assert(!unreachable.reachable);

    std::cout << "Graph + Dijkstra tests passed!\n";
    return 0;
}
