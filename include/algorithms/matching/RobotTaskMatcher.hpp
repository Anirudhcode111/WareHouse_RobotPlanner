
#pragma once

#include "algorithms/Graph.hpp"
#include "domain/Warehouse.hpp"

#include <string>
#include <vector>

namespace warehouse {

struct RobotTaskAssignment {
    std::string robotId;
    std::string taskId;
    double travelDistance = 0.0;
};

struct RobotTaskMatchingResult {
    std::vector<RobotTaskAssignment> assignments;
    std::size_t unmatchedRobotCount = 0;
    std::size_t unmatchedTaskCount = 0;
};

class RobotTaskMatcher {
public:
    static RobotTaskMatchingResult match(
        const Warehouse& warehouse,
        const Graph& graph
    );
};

} // namespace warehouse
