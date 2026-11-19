

#include "algorithms/mst/Prim.hpp"

#include <functional>
#include <queue>
#include <stdexcept>
#include <tuple>
#include <unordered_set>
#include <vector>

namespace warehouse::algorithms {

MSTResult primMST(
    const warehouse::Graph& graph,
    const std::string& startLocation
) {
    const auto& adjacency = graph.getAdjacencyList();

    if (adjacency.empty()) {
        return {};
    }

    if (adjacency.find(startLocation) == adjacency.end()) {
        throw std::invalid_argument("Starting location does not exist.");
    }

    using EdgeCandidate = std::tuple<
        double,
        std::string,
        std::string
    >;

    std::priority_queue<
        EdgeCandidate,
        std::vector<EdgeCandidate>,
        std::greater<EdgeCandidate>
    > minHeap;

    MSTResult result;
    std::unordered_set<std::string> visited;

    visited.insert(startLocation);

    for (const auto& edge : adjacency.at(startLocation)) {
        minHeap.push({
            edge.distance,
            startLocation,
            edge.destination
        });
    }

    while (!minHeap.empty()) {
        auto [weight, from, to] = minHeap.top();
        minHeap.pop();

        if (visited.count(to)) {
            continue;
        }

        visited.insert(to);

        result.selectedEdges.push_back({from, to, weight});
        result.totalWeight += weight;

        for (const auto& edge : adjacency.at(to)) {
            if (!visited.count(edge.destination)) {
                minHeap.push({
                    edge.distance,
                    to,
                    edge.destination
                });
            }
        }
    }

    result.connected = (visited.size() == adjacency.size());

    return result;
}

}
