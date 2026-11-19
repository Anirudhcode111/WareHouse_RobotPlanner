
#pragma once

#include "algorithms/mst/Prim.hpp"
#include "algorithms/Graph.hpp"

namespace warehouse::algorithms {

MSTResult kruskalMST(const warehouse::Graph& graph);

}
