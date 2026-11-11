
#pragma once

#include "algorithms/Graph.hpp"
#include "domain/Warehouse.hpp"

namespace warehouse {

class WarehouseGraphBuilder {
public:
    static Graph build(const Warehouse& warehouse);
};

} // namespace warehouse
