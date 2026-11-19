#include "algorithms/indexing/WarehouseIndices.hpp"

#include <cassert>
#include <iostream>

using namespace warehouse;
using namespace warehouse::algorithms;

int main() {
    Warehouse warehouse;

    warehouse.locations.push_back({"L1"});
    warehouse.locations.push_back({"L2"});

    warehouse.robots.push_back({"R1", "L1", 50.0});
    warehouse.robots.push_back({"R2", "L2", 100.0});

    warehouse.tasks.push_back({"T1", "L1", 10.0, 2});
    warehouse.tasks.push_back({"T2", "L2", 20.0, 5});

    WarehouseIndices indices;

    assert(indices.build(warehouse));

    // Task lookup
    const Task* task = indices.findTask("T1");
    assert(task != nullptr);
    assert(task->weightKg == 10.0);

    // Robot lookup
    const Robot* robot = indices.findRobot("R2");
    assert(robot != nullptr);
    assert(robot->capacityKg == 100.0);

    // Location lookup
    const Location* location = indices.findLocation("L1");
    assert(location != nullptr);
    assert(location->id == "L1");

    // Non-existing IDs
    assert(indices.findTask("T99") == nullptr);
    assert(indices.findRobot("R99") == nullptr);
    assert(indices.findLocation("L99") == nullptr);

    // Duplicate ID validation
    Warehouse invalidWarehouse;
    invalidWarehouse.tasks.push_back({"T1", "L1", 5.0, 1});
    invalidWarehouse.tasks.push_back({"T1", "L2", 8.0, 2});

    assert(!indices.build(invalidWarehouse));

    // Previous valid index must remain usable
    assert(indices.findTask("T1") != nullptr);

    std::cout << "All Warehouse Indices tests passed!\n";

    return 0;
}