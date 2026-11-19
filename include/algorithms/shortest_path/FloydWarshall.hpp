
#pragma once

#include "algorithms/Graph.hpp"

#include <string>
#include <unordered_map>
#include <vector>

namespace warehouse::algorithms {

struct FloydWarshallResult {
    std::vector<std::string> vertices;
    std::vector<std::vector<double>> distances;
    bool hasNegativeCycle = false;
};

FloydWarshallResult floydWarshall(
    const warehouse::Graph& graph
);

}
