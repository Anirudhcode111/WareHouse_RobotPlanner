#pragma once
#include <string>
namespace warehouse {
struct Robot {
    std::string id;
    std::string currentLocationId;
    double capacityKg = 0.0;
};
} // namespace warehouse
