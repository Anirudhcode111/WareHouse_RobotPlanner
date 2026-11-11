
#include "algorithms/WarehouseGraphBuilder.hpp"

namespace warehouse {

Graph WarehouseGraphBuilder::build(const Warehouse& warehouse) {
    Graph graph;

    // Add every location, including isolated locations.
    for (const Location& location : warehouse.locations) {
        graph.addLocation(location.id);
    }

    // Add connections unless an obstacle blocks that connection.
    for (const Connection& connection : warehouse.connections) {
        bool blocked = false;

        for (const Obstacle& obstacle : warehouse.obstacles) {
            const bool sameDirection =
                obstacle.endpointAId == connection.endpointAId &&
                obstacle.endpointBId == connection.endpointBId;

            const bool reverseDirection =
                obstacle.endpointAId == connection.endpointBId &&
                obstacle.endpointBId == connection.endpointAId;

            if (sameDirection || reverseDirection) {
                blocked = true;
                break;
            }
        }

        if (!blocked) {
            graph.addConnection(
                connection.endpointAId,
                connection.endpointBId,
                connection.distance
            );
        }
    }

    return graph;
}

} // namespace warehouse
