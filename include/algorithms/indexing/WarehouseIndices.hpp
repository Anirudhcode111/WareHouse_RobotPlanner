
#pragma once

#include <cstddef>
#include <string>
#include <unordered_map>

#include "domain/Warehouse.hpp"

namespace warehouse::algorithms {

class WarehouseIndices {
private:
    const Warehouse* warehouse_ = nullptr;

    std::unordered_map<std::string, std::size_t> taskIndex_;
    std::unordered_map<std::string, std::size_t> robotIndex_;
    std::unordered_map<std::string, std::size_t> locationIndex_;

public:
    // Build all indices from warehouse data.
    // Returns false if duplicate IDs are found.
    bool build(const Warehouse& warehouse);

    // Find records using their unique IDs.
    const Task* findTask(const std::string& id) const;
    const Robot* findRobot(const std::string& id) const;
    const Location* findLocation(const std::string& id) const;
};

}
