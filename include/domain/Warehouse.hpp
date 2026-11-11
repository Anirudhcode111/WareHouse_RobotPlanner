

#pragma once

#include <vector>

#include "Location.hpp"
#include "Robot.hpp"
#include "Task.hpp"
#include "Obstacle.hpp"
#include "Connection.hpp"

namespace warehouse {

struct Warehouse {
    std::vector<Location> locations;
    std::vector<Robot> robots;
    std::vector<Task> tasks;
    std::vector<Obstacle> obstacles;
    std::vector<Connection> connections;
};

} // namespace warehouse
