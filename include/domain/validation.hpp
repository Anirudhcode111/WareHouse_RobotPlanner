#pragma once
#include <string>
#include <vector>
#include "domain/Warehouse.hpp"
namespace warehouse {
// Empty result means the P1 domain checks passed. Graph connectivity is checked in P2.
std::vector<std::string> validateWarehouseData(const Warehouse& warehouse);
} // namespace warehouse
