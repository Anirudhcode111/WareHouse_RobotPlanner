
#include "algorithms/shortest_path/BellmanFord.hpp"

#include <limits>
#include <stdexcept>
#include <vector>

namespace warehouse::algorithms {

BellmanFordResult bellmanFord(
    const warehouse::Graph& graph,
    const std::string& source
) {
    const auto& adjacency = graph.getAdjacencyList();

    if (adjacency.find(source) == adjacency.end()) {
        throw std::invalid_argument("Source location does not exist.");
    }

    const double INF = std::numeric_limits<double>::infinity();

    BellmanFordResult result;

    for (const auto& entry : adjacency) {
        result.distances[entry.first] = INF;
    }

    result.distances[source] = 0.0;

    // Collect every directed representation of each edge.
    // Graph stores undirected connections in both directions.
    struct Edge {
        std::string from;
        std::string to;
        double weight;
    };

    std::vector<Edge> edges;

    for (const auto& entry : adjacency) {
        for (const auto& edge : entry.second) {
            edges.push_back({
                entry.first,
                edge.destination,
                edge.distance
            });
        }
    }

    const std::size_t vertexCount = adjacency.size();

    // Relax all edges V-1 times.
    for (std::size_t i = 1; i < vertexCount; ++i) {
        bool updated = false;

        for (const auto& edge : edges) {
            if (result.distances[edge.from] != INF &&
                result.distances[edge.from] + edge.weight <
                    result.distances[edge.to]) {

                result.distances[edge.to] =
                    result.distances[edge.from] + edge.weight;

                updated = true;
            }
        }

        if (!updated) {
            break;
        }
    }

    // Check for a reachable negative-weight cycle.
    for (const auto& edge : edges) {
        if (result.distances[edge.from] != INF &&
            result.distances[edge.from] + edge.weight <
                result.distances[edge.to]) {

            result.hasNegativeCycle = true;
            break;
        }
    }

    return result;
}

}
