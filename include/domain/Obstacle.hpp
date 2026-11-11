#pragma once
#include <string>
namespace warehouse {
// P1 records a blocked connection. The graph implementation arrives in P2.
struct Obstacle {
    std::string endpointAId;
    std::string endpointBId;
};
} // namespace warehouse
