
#include "algorithms/shortest_path/FloydWarshall.hpp"

#include <limits>
#include <unordered_map>

namespace warehouse::algorithms {

FloydWarshallResult floydWarshall(
    const warehouse::Graph& graph
) {
    const auto& adjacency = graph.getAdjacencyList();

    FloydWarshallResult result;

    // Collect vertices and assign matrix indices.
    std::unordered_map<std::string, std::size_t> index;

    for (const auto& entry : adjacency) {
        index[entry.first] = result.vertices.size();
        result.vertices.push_back(entry.first);
    }

    const std::size_t n = result.vertices.size();

    const double INF = std::numeric_limits<double>::infinity();

    // Initialize distance matrix.
    result.distances.assign(
        n,
        std::vector<double>(n, INF)
    );

    for (std::size_t i = 0; i < n; ++i) {
        result.distances[i][i] = 0.0;
    }

    // Load direct edge weights.
    for (const auto& entry : adjacency) {
        std::size_t u = index.at(entry.first);

        for (const auto& edge : entry.second) {
            std::size_t v = index.at(edge.destination);

            if (edge.distance < result.distances[u][v]) {
                result.distances[u][v] = edge.distance;
            }
        }
    }

    // Floyd-Warshall dynamic programming.
    for (std::size_t k = 0; k < n; ++k) {
        for (std::size_t i = 0; i < n; ++i) {
            for (std::size_t j = 0; j < n; ++j) {

                if (result.distances[i][k] != INF &&
                    result.distances[k][j] != INF) {

                    double newDistance =
                        result.distances[i][k] +
                        result.distances[k][j];

                    if (newDistance < result.distances[i][j]) {
                        result.distances[i][j] = newDistance;
                    }
                }
            }
        }
    }

    // Negative diagonal indicates a negative cycle.
    for (std::size_t i = 0; i < n; ++i) {
        if (result.distances[i][i] < 0.0) {
            result.hasNegativeCycle = true;
            break;
        }
    }

    return result;
}

}
