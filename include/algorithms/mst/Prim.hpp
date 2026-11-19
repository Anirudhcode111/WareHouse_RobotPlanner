

#pragma once

#include "algorithms/Graph.hpp"

#include <string>
#include <vector>

namespace warehouse::algorithms {

struct MSTEdge {
    std::string source;
    std::string destination;
    double weight = 0.0;
};

struct MSTResult {
    std::vector<MSTEdge> selectedEdges;
    double totalWeight = 0.0;
    bool connected = true;
};

MSTResult primMST(
    const warehouse::Graph& graph,
    const std::string& startLocation
);

}
