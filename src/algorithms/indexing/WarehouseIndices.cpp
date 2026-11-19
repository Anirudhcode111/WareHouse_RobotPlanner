
#include "algorithms/indexing/WarehouseIndices.hpp"

namespace warehouse::algorithms {

bool WarehouseIndices::build(const Warehouse& warehouse) {
    // Build temporary indices first.
    // This prevents partially updated indices if validation fails.
    std::unordered_map<std::string, std::size_t> newTaskIndex;
    std::unordered_map<std::string, std::size_t> newRobotIndex;
    std::unordered_map<std::string, std::size_t> newLocationIndex;

    // Build Task ID index.
    for (std::size_t i = 0; i < warehouse.tasks.size(); i++) {
        const std::string& id = warehouse.tasks[i].id;

        if (id.empty() || !newTaskIndex.emplace(id, i).second) {
            return false;
        }
    }

    // Build Robot ID index.
    for (std::size_t i = 0; i < warehouse.robots.size(); i++) {
        const std::string& id = warehouse.robots[i].id;

        if (id.empty() || !newRobotIndex.emplace(id, i).second) {
            return false;
        }
    }

    // Build Location ID index.
    for (std::size_t i = 0; i < warehouse.locations.size(); i++) {
        const std::string& id = warehouse.locations[i].id;

        if (id.empty() || !newLocationIndex.emplace(id, i).second) {
            return false;
        }
    }

    // Commit all indices after successful construction.
    taskIndex_ = std::move(newTaskIndex);
    robotIndex_ = std::move(newRobotIndex);
    locationIndex_ = std::move(newLocationIndex);

    warehouse_ = &warehouse;

    return true;
}

const Task* WarehouseIndices::findTask(const std::string& id) const {
    if (warehouse_ == nullptr) {
        return nullptr;
    }

    auto it = taskIndex_.find(id);

    if (it == taskIndex_.end()) {
        return nullptr;
    }

    return &warehouse_->tasks[it->second];
}

const Robot* WarehouseIndices::findRobot(const std::string& id) const {
    if (warehouse_ == nullptr) {
        return nullptr;
    }

    auto it = robotIndex_.find(id);

    if (it == robotIndex_.end()) {
        return nullptr;
    }

    return &warehouse_->robots[it->second];
}

const Location* WarehouseIndices::findLocation(const std::string& id) const {
    if (warehouse_ == nullptr) {
        return nullptr;
    }

    auto it = locationIndex_.find(id);

    if (it == locationIndex_.end()) {
        return nullptr;
    }

    return &warehouse_->locations[it->second];
}

}
