
#include "algorithms/Graph.hpp"

#include <stdexcept>

namespace warehouse {

void Graph::addLocation(const std::string& locationId) {
    if (locationId.empty()) {
        throw std::invalid_argument("Location ID cannot be empty.");
    }

    adjacencyList[locationId];
}

void Graph::addConnection(
    const std::string& locationA,
    const std::string& locationB,
    double distance
) {
    if (locationA.empty() || locationB.empty()) {
        throw std::invalid_argument("Location IDs cannot be empty.");
    }

    if (distance < 0.0) {
        throw std::invalid_argument("Distance cannot be negative.");
    }

    addLocation(locationA);
    addLocation(locationB);

    adjacencyList[locationA].push_back({locationB, distance});
    adjacencyList[locationB].push_back({locationA, distance});
}

const std::unordered_map<std::string, std::vector<GraphEdge>>&
Graph::getAdjacencyList() const {
    return adjacencyList;
}

} // namespace warehouse
