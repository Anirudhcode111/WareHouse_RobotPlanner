
#pragma once

#include "algorithms/Graph.hpp"
#include "domain/Warehouse.hpp"

#include <string>
#include <vector>

namespace warehouse {

struct TaskPlan {
    bool feasible = false;
    double totalDistance = 0.0;
    double totalTaskWeightKg = 0.0;

    std::vector<std::string> taskIds;
    std::vector<std::string> visitedLocations;

    std::string message;
};

class TaskPlanner {
public:
    static TaskPlan planGreedy(
        const Warehouse& warehouse,
        const Graph& graph,
        const std::string& robotId
    );
};

} // namespace warehouse
