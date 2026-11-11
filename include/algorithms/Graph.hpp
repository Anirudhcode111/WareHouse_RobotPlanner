
#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace warehouse {

struct GraphEdge {
    std::string destination;
    double distance = 0.0;
};

class Graph {
public:
    void addLocation(const std::string& locationId);

    void addConnection(
        const std::string& locationA,
        const std::string& locationB,
        double distance
    );

    const std::unordered_map<std::string, std::vector<GraphEdge>>&
    getAdjacencyList() const;

private:
    std::unordered_map<std::string, std::vector<GraphEdge>>
        adjacencyList;
};

} // namespace warehouse
