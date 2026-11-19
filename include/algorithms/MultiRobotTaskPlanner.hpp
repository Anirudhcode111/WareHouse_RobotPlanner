
#pragma once

#include "algorithms/Graph.hpp"
#include "algorithms/TaskPlanner.hpp"
#include "domain/Warehouse.hpp"

#include <cstddef>
#include <string>
#include <vector>

namespace warehouse {

struct MultiRobotTaskPlan {
    std::string robotId;
    TaskPlan plan;
};

struct MultiRobotTaskPlanningResult {
    std::vector<MultiRobotTaskPlan> robotPlans;
    std::vector<std::string> unassignedTaskIds;

    std::size_t assignedTaskCount = 0;
    std::size_t unassignedTaskCount = 0;

    bool feasible = false;
    std::string message;
};

class MultiRobotTaskPlanner {
public:
    static MultiRobotTaskPlanningResult plan(
        const Warehouse& warehouse,
        const Graph& graph
    );
};

} // namespace warehouse
