#pragma once
#include <string>
namespace warehouse {
enum class TaskStatus { Pending, Completed };
struct Task {
    std::string id;
    std::string pickupLocationId;
    double weightKg = 0.0;
    int priority = 0;
    TaskStatus status = TaskStatus::Pending;
};
} // namespace warehouse

