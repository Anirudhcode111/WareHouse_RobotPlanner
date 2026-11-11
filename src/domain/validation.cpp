#include "domain/validation.hpp"
#include <cmath>
#include <set>
#include <string>
namespace warehouse {
namespace {
bool isBlank(const std::string& value) {
    return value.find_first_not_of(" \\t\\n\\r") == std::string::npos;
}
template <typename Range, typename IdGetter>
void checkUniqueIds(const Range& items, IdGetter getId,
                    const std::string& entityName, std::vector<std::string>& errors) {
    std::set<std::string> seen;
    for (const auto& item : items) {
        const std::string id = getId(item);
        if (isBlank(id)) errors.push_back(entityName + " has a blank ID.");
        else if (!seen.insert(id).second) errors.push_back(entityName + " has duplicate ID: " + id);
    }
}
}
std::vector<std::string> validateWarehouseData(const Warehouse& warehouse) {
    std::vector<std::string> errors;
    checkUniqueIds(warehouse.locations, [](const Location& x) { return x.id; }, "Location", errors);
    checkUniqueIds(warehouse.robots, [](const Robot& x) { return x.id; }, "Robot", errors);
    checkUniqueIds(warehouse.tasks, [](const Task& x) { return x.id; }, "Task", errors);

    std::set<std::string> locationIds;
    for (const auto& location : warehouse.locations)
        if (!isBlank(location.id)) locationIds.insert(location.id);

    for (const auto& robot : warehouse.robots) {
        if (isBlank(robot.currentLocationId) || !locationIds.count(robot.currentLocationId))
            errors.push_back("Robot " + robot.id + " references an unknown current location: " + robot.currentLocationId);
        if (!std::isfinite(robot.capacityKg) || robot.capacityKg < 0.0)
            errors.push_back("Robot " + robot.id + " has invalid capacityKg; expected a finite non-negative value.");
    }
    for (const auto& task : warehouse.tasks) {
        if (isBlank(task.pickupLocationId) || !locationIds.count(task.pickupLocationId))
            errors.push_back("Task " + task.id + " references an unknown pickup location: " + task.pickupLocationId);
        if (!std::isfinite(task.weightKg) || task.weightKg < 0.0)
            errors.push_back("Task " + task.id + " has invalid weightKg; expected a finite non-negative value.");
    }
    for (const auto& obstacle : warehouse.obstacles) {
        if (isBlank(obstacle.endpointAId) || !locationIds.count(obstacle.endpointAId))
            errors.push_back("Obstacle references unknown endpoint: " + obstacle.endpointAId);
        if (isBlank(obstacle.endpointBId) || !locationIds.count(obstacle.endpointBId))
            errors.push_back("Obstacle references unknown endpoint: " + obstacle.endpointBId);
        if (obstacle.endpointAId == obstacle.endpointBId)
            errors.push_back("Obstacle endpoints must be different locations.");
    }
    return errors;
}
} // namespace warehouse
