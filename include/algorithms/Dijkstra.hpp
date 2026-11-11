
#pragma once

#include "algorithms/Graph.hpp"

#include <string>
#include <vector>

namespace warehouse {

struct PathResult {
    bool reachable = false;
    double totalDistance = 0.0;
    std::vector<std::string> path;
};

class Dijkstra {
public:
    static PathResult shortestPath(
        const Graph& graph,
        const std::string& start,
        const std::string& destination
    );
};

} // namespace warehouse
