
#include "algorithms/mst/Kruskal.hpp"

#include <algorithm>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>

namespace warehouse::algorithms {

namespace {

class DisjointSet {
private:
    std::unordered_map<std::string, std::string> parent;
    std::unordered_map<std::string, int> rank;

public:
    explicit DisjointSet(
        const std::vector<std::string>& vertices
    ) {
        for (const auto& vertex : vertices) {
            parent[vertex] = vertex;
            rank[vertex] = 0;
        }
    }

    std::string find(const std::string& vertex) {
        if (parent[vertex] != vertex) {
            parent[vertex] = find(parent[vertex]);
        }

        return parent[vertex];
    }

    bool unite(
        const std::string& first,
        const std::string& second
    ) {
        std::string rootA = find(first);
        std::string rootB = find(second);

        if (rootA == rootB) {
            return false;
        }

        if (rank[rootA] < rank[rootB]) {
            std::swap(rootA, rootB);
        }

        parent[rootB] = rootA;

        if (rank[rootA] == rank[rootB]) {
            rank[rootA]++;
        }

        return true;
    }
};

struct CandidateEdge {
    std::string source;
    std::string destination;
    double weight;
};

}

MSTResult kruskalMST(const warehouse::Graph& graph) {
    const auto& adjacency = graph.getAdjacencyList();

    MSTResult result;

    if (adjacency.empty()) {
        return result;
    }

    std::vector<std::string> vertices;
    std::vector<CandidateEdge> edges;

    for (const auto& entry : adjacency) {
        vertices.push_back(entry.first);

        for (const auto& edge : entry.second) {
            // Collect each undirected edge only once.
            if (entry.first < edge.destination) {
                edges.push_back({
                    entry.first,
                    edge.destination,
                    edge.distance
                });
            }
        }
    }

    std::sort(
        edges.begin(),
        edges.end(),
        [](const CandidateEdge& a, const CandidateEdge& b) {
            if (a.weight != b.weight) {
                return a.weight < b.weight;
            }

            if (a.source != b.source) {
                return a.source < b.source;
            }

            return a.destination < b.destination;
        }
    );

    DisjointSet dsu(vertices);

    for (const auto& edge : edges) {
        if (dsu.unite(edge.source, edge.destination)) {
            result.selectedEdges.push_back({
                edge.source,
                edge.destination,
                edge.weight
            });

            result.totalWeight += edge.weight;

            if (result.selectedEdges.size() == vertices.size() - 1) {
                break;
            }
        }
    }

    result.connected =
        (result.selectedEdges.size() == vertices.size() - 1);

    return result;
}

}
