
#include "algorithms/Dijkstra.hpp"

#include <algorithm>
#include <functional>
#include <limits>
#include <queue>
#include <unordered_map>

namespace warehouse {

PathResult Dijkstra::shortestPath(
    const Graph& graph,
    const std::string& start,
    const std::string& destination
) {
    PathResult result;

    const auto& adjacency = graph.getAdjacencyList();

    // Check whether both locations exist.
    if (adjacency.find(start) == adjacency.end() ||
        adjacency.find(destination) == adjacency.end()) {
        return result;
    }

    // Distance and previous-location maps.
    const double INF = std::numeric_limits<double>::infinity();

    std::unordered_map<std::string, double> distances;
    std::unordered_map<std::string, std::string> previous;

    for (const auto& entry : adjacency) {
        distances[entry.first] = INF;
    }

    distances[start] = 0.0;

    // Min-priority queue: smallest distance is processed first.
    using QueueEntry = std::pair<double, std::string>;

    std::priority_queue<
        QueueEntry,
        std::vector<QueueEntry>,
        std::greater<QueueEntry>
    > priorityQueue;

    priorityQueue.push({0.0, start});

    while (!priorityQueue.empty()) {
        const auto current = priorityQueue.top();
        priorityQueue.pop();

        const double currentDistance = current.first;
        const std::string& currentLocation = current.second;

        // Ignore outdated queue entries.
        if (currentDistance > distances[currentLocation]) {
            continue;
        }

        // Destination reached with its shortest distance.
        if (currentLocation == destination) {
            break;
        }

        const auto& edges = adjacency.at(currentLocation);

        for (const GraphEdge& edge : edges) {
            const double newDistance =
                currentDistance + edge.distance;

            if (newDistance < distances[edge.destination]) {
                distances[edge.destination] = newDistance;
                previous[edge.destination] = currentLocation;

                priorityQueue.push({
                    newDistance,
                    edge.destination
                });
            }
        }
    }

    // No route exists.
    if (distances[destination] == INF) {
        return result;
    }

    // Reconstruct the path from destination to start.
    std::vector<std::string> reversedPath;
    std::string current = destination;

    while (current != start) {
        reversedPath.push_back(current);

        auto it = previous.find(current);
        if (it == previous.end()) {
            return PathResult{};
        }

        current = it->second;
    }

    reversedPath.push_back(start);

    std::reverse(reversedPath.begin(), reversedPath.end());

    result.reachable = true;
    result.totalDistance = distances[destination];
    result.path = reversedPath;

    return result;
}

} // namespace warehouse
