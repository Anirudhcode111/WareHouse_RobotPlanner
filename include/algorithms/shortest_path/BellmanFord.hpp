
#pragma once

#include "algorithms/Graph.hpp"

#include <string>
#include <unordered_map>

namespace warehouse::algorithms {

struct BellmanFordResult {
    std::unordered_map<std::string, double> distances;
    bool hasNegativeCycle = false;
};

BellmanFordResult bellmanFord(
    const warehouse::Graph& graph,
    const std::string& source
);

}
