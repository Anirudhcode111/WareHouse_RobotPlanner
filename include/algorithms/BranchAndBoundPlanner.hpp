
#pragma once

#include "algorithms/Graph.hpp"
#include "domain/Warehouse.hpp"
#include "algorithms/TaskPlanner.hpp"

#include <string>
#include <vector>

namespace warehouse {

class BranchAndBoundPlanner {
public:
    // Finds the minimum-distance ordering for the specified tasks.
    // Task IDs must identify pending tasks in the warehouse.
    static TaskPlan planExact(
        const Warehouse& warehouse,
        const Graph& graph,
        const std::string& robotId,
        const std::vector<std::string>& taskIds
    );
};

} // namespace warehouse
